#include "WebPlatformEngine.h"
#include "common.h"
#include "utils.h"
#include "llama.h"
#include <c_api/Index_c.h>
#include <c_api/error_c.h>
#include <stdio.h>
#include <stdlib.h>


static struct llama_model * model = NULL;
static struct llama_context_params cparams;
static struct llama_context * ctx = NULL;

static FaissIndex* faissIndex = NULL;

void free_resources() {
    // faiss cleanup
    faiss_Index_free(faissIndex);

    // llama cleanip
    if (ctx != NULL) {
        llama_free(ctx);
        ctx = NULL;
    }
    if (model != NULL) {
        llama_free_model(model);
        model = NULL;
    }
    llama_backend_free();

    printf("LLama.cpp resources are freed.\n");
}


static float* get_embedding(const char *question, const char *model_path) {
    // Initialize llama backend once
    if (model == NULL) {
        llama_backend_init();
    
        struct llama_model_params mparams = llama_model_default_params();

        model = llama_load_model_from_file(model_path, mparams);
        cparams = llama_context_default_params();

        cparams.embeddings = true;
        cparams.n_batch = 4096;
        cparams.n_ubatch = cparams.n_batch;
        cparams.pooling_type = LLAMA_POOLING_TYPE_MEAN;  // LLAMA_POOLING_TYPE_NONE
        ctx = llama_new_context_with_model(model, cparams);

        if (model == NULL) {
            printf("%s: unable to load model\n", __func__);
            return 1;
        }
        else {
            printf("model loaded successfuly!\n");
        }
    }
    

    llama_token tokens[512];  // Token array, assuming max 512 tokens
    int n_tokens = llama_tokenize(model, question, strlen(question), tokens, 512, 1, 0);

    printf("******************\n");
    printf("number of tokens: %i\n", n_tokens);
    printf("tokens:\n");
    for (int i = 0 ; i < n_tokens ; ++i) {
        printf("%i", tokens[i]);
        if (i + 1 < n_tokens) {
            printf(", ");
        }
    }
    printf("\n");
    printf("******************\n");

    // Create batch
    struct llama_batch batch = llama_batch_init(cparams.n_batch, 0, 1);
    int n_embd_count = 1;  // n_tokens;

    llama_kv_cache_clear(ctx);

    for (int i = 0 ; i < n_tokens ; ++i) {
        batch.token[batch.n_tokens] = tokens[i];
        batch.pos[batch.n_tokens] = i;
        batch.n_seq_id[batch.n_tokens] = 1;
        batch.seq_id[batch.n_tokens][0] = 0;  // we only have 1 seq
        batch.logits[batch.n_tokens] = true;
        batch.n_tokens++;
    }

    if (llama_model_has_encoder(model) && !llama_model_has_decoder(model)) {
        // encoder-only model
        if (llama_encode(ctx, batch) < 0) {
            printf("%s : failed to encode\n", __func__);
        }
    } else if (!llama_model_has_encoder(model) && llama_model_has_decoder(model)) {
        // decoder-only model
        if (llama_decode(ctx, batch) < 0) {
            printf("%s : failed to decode\n", __func__);
        }
    }

    // Allocate memory for embeddings
    const int n_embd = llama_n_embd(model);
    printf("n_embd: %i\n", n_embd);
    float *embeddings = (float *)malloc(n_embd_count * n_embd * sizeof(float));
    // initialize
    for (int i = 0 ; i < n_embd_count * n_embd ; ++i) {
        embeddings[i] = 0;
    }

    // Extract embeddings
    printf("Extract embeddings...\n");
    memcpy(embeddings, llama_get_embeddings_seq(ctx, 0), n_embd * sizeof(float));
    printf("Embedding: ");
    for (int i = 0 ; i < 10 ; ++i) {
        printf("%.2f, ", embeddings[i]);
    }
    printf("\n");
    printf("****Done extracting embeddings***\n");

    return embeddings;
}

static char* get_docs(idx_t *indices, int k, const char *docs_index_path) {
    JSON *docs_index = read_json_file(docs_index_path);

    char docs_root_dir[512];
    snprintf(
        docs_root_dir,
        sizeof(docs_root_dir),
        "%s/%s",
        JSON2STR(JSON_AT(docs_index, "doc_folder")),
        JSON2STR(JSON_AT(JSON_AT(docs_index, "WebPlatform"), "doc_folder"))
    );

    printf("docs root directory: %s\n", docs_root_dir);

    JSON *docs;
    JSON_REF(JSON_AT(docs_index, "WebPlatform"), "docs", docs);

    char* combined_docs = (char*)malloc(1);
    if (combined_docs == NULL) {
        fprintf(stderr, "Failed to allocate memory for combined docs\n");
        return NULL;
    }
    combined_docs[0] = '\0';  // Initialize the combined_docs string

    for (int i = 0 ; i < k; ++i) {
        int doc_id = indices[i];
        printf("Processing doc %i\n", doc_id);
        if (doc_id == -1) {
            continue;
        }

        char *content = JSON2STR(JSON_AT(JSON_AT_IDX(docs, doc_id), "content"));
        char *doc = read_doc(doc_id, docs_root_dir);

        if (doc) {
            combined_docs = (char *)realloc(
                combined_docs,
                strlen(combined_docs) + strlen(content) + strlen(doc) + 3
            );
            strcat(combined_docs, content);
            strcat(combined_docs, "\n");
            strcat(combined_docs, doc);
            strcat(combined_docs, "\n");

            printf("Doc %i added successfuly\n", doc_id);
        }
        else {
            printf("Failed to add Doc %i\n", doc_id);
        }

        // printf(
        //     "************\nDoc %i\ncontent:%s\nFull doc:\n%s\n************\n",
        //     doc_id, content, read_doc(doc_id, docs_root_dir)
        // );

        // Cleanup
        free(doc);
    }

    return combined_docs;
}

char* webplatform_run(
  const char *api_key, const char *question,
  const char *docs_index_path, const char *rag_index_path,
  const char *model_tag, const char *model_path) {
    // read the index file once
    if (faissIndex == NULL) {
        FaissErrorCode err = faiss_read_index_fname(rag_index_path, 0, &faissIndex);
        if (err != 0) {
            printf("Error reading index from file: %s\n", faiss_get_last_error());
            return -1;
        }
    }
    
    const int d_embeddings = faiss_Index_d(faissIndex);  // dimension of the vectors

    // Remove the prefix from the question
    char prefix[] = "Using enAlusus with WebPlatform, ";
    memmove(question, question + strlen(prefix), strlen(question) - strlen(prefix) + 1);

    // get question embeddings
    float *embeddings = get_embedding(question, model_path);

    // get docs
    int k = 4; // number of nearest neighbors
    int nq = 1; // number of queries
    float *distances = (float*)malloc(nq * k * sizeof(float));
    idx_t *indices = (idx_t*)malloc(nq * k * sizeof(idx_t));

    faiss_Index_search(faissIndex, nq, embeddings, k, distances, indices);
    printf("Distances:\n");
    for (int i = 0; i < k; i++) {
        printf("%f ", distances[i]);
    }
    printf("\nIndices:\n");
    for (int i = 0; i < k; i++) {
        printf("%lld ", indices[i]);
    }
    printf("\n");

    char *combined_docs = get_docs(indices, k, docs_index_path);
    printf("*********\nCombiend docs:\n%s\n*********\n", combined_docs);

    // Cleanup
    free(embeddings);
    free(distances);
    free(indices);

    // construct RAG query
    char *rag_question = get_rag_question(question, combined_docs);

    return send_request(api_key, rag_question, model_tag);
}
