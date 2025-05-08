#include "BasicEngine.h"
#include "DocsExpert.h"
#include "MainAgent.h"
#include "AlususExpert.h"
#include "KeywordDocsRetriever.h"
#include "common.h"
#include "utils.h"
#include <c_api/Index_c.h>
#include <c_api/error_c.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pcre.h>
#include "llama.h"


static struct llama_model * model = NULL;
static struct llama_context_params cparams;
static struct llama_context * ctx = NULL;

static FaissIndex* faissIndex = NULL;

void basic_free_resources() {
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

    // printf("******************\n");
    // printf("number of tokens: %i\n", n_tokens);
    // printf("tokens:\n");
    // for (int i = 0 ; i < n_tokens ; ++i) {
    //     printf("%i", tokens[i]);
    //     if (i + 1 < n_tokens) {
    //         printf(", ");
    //     }
    // }
    // printf("\n");
    // printf("******************\n");

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
    //printf("n_embd: %i\n", n_embd);
    float *embeddings = (float *)malloc(n_embd_count * n_embd * sizeof(float));
    // initialize
    for (int i = 0 ; i < n_embd_count * n_embd ; ++i) {
        embeddings[i] = 0;
    }

    // Extract embeddings
    printf("Extract embeddings...\n");
    memcpy(embeddings, llama_get_embeddings_seq(ctx, 0), n_embd * sizeof(float));
    // printf("Embedding: ");
    // for (int i = 0 ; i < 10 ; ++i) {
    //     printf("%.2f, ", embeddings[i]);
    // }
    // printf("\n");
    printf("****Done extracting embeddings***\n");

    return embeddings;
}

static char* combine_docs(JSON *docs) {
    // Convert the json_object array to a combined string with newline separators
    int num_docs = JSON_A_LEN(docs);
    int docs_length = 0;

    for (int i = 0; i < num_docs; i++) {
        const char* doc = JSON2STR(JSON_AT_IDX(docs, i));
        docs_length += strlen(doc) + 1;  // +1 for the newline character
    }

    char* combined_docs = (char*)malloc(docs_length + 1);  // +1 for the null terminator
    if (combined_docs == NULL) {
        fprintf(stderr, "Failed to allocate memory for combined docs\n");
        return NULL;
    }

    combined_docs[0] = '\0';  // Initialize the combined_docs string

    for (int i = 0; i < num_docs; i++) {
        const char* doc = JSON2STR(JSON_AT_IDX(docs, i));
        strcat(combined_docs, doc);
        strcat(combined_docs, "\n");
    }

    return combined_docs;
}

static JSON* get_docs_from_code(
  const char* code, const char* docs_root_dir,
  const char *features_mapper_path)
{
    JSON* features_mapper = read_json_file(features_mapper_path);
    
    return get_docs_by_keyword(code, docs_root_dir, features_mapper);
}

static char* format_example(int doc_id, const char *prompt, const char *doc) {
    size_t example_size = strlen(prompt) + strlen(doc) + 128;
    char *example = (char*)malloc(example_size);
    if (!example) {
        fprintf(stderr, "Failed to allocate memory for doc %i\n", doc_id);
        free(doc);
        return "";
    }
    snprintf(
        example,
        example_size,
        "Prompt: %s\nAnswer:\n%s\n",
        prompt,
        doc
    );

    return example;
}

static JSON* get_examples(idx_t *indices, int k, const char *examples_index_path) {
    JSON *examples_index = read_json_file(examples_index_path);
    char *docs_root_dir = get_root_dir(examples_index_path);
    //printf("docs root directory: %s\n", docs_root_dir);

    JSON *docs_array = json_object_new_array();
    for (int i = 0; i < k; ++i) {
        int doc_id = indices[i];
        //printf("Processing doc %i\n", doc_id);
        if (doc_id == -1)
            continue;

        char *prompt = JSON2STR(JSON_AT(JSON_AT_IDX(examples_index, doc_id), "enPrompt"));
        char doc_path[512];
        snprintf(
            doc_path,
            sizeof(doc_path),
            "%s/%s",
            docs_root_dir,
            JSON2STR(JSON_AT(JSON_AT_IDX(examples_index, doc_id), "enAlususAnswer"))
        );
        char *doc = read_doc_by_path(doc_path);
        //printf("Adding doc ...\n");
        if (doc) {
            char *example = format_example(doc_id, prompt, doc);
            JSON_A_ADD(docs_array, STR2JSON(example));
            free(example);

            //printf("Doc %i added successfully\n", doc_id);
        } else {
            printf("Failed to add Doc %i\n", doc_id);
        }
        free(doc);
    }
    return docs_array;
}

char* basic_run(
  const char *api_key, const char *question,
  const char* docs_root_dir, const char *docs_index_path,
  const char *base_model_tag, const char *model_tag,
  const char *vdb_path, const char *index_path,
  const char *alusus_features_mapper_path, const char *embedding_model_path) {

    char *cpp_code = get_cpp_code(api_key, question, base_model_tag);
    //printf("***************\ncpp_code:\n %s\n**************\n", cpp_code);
    JSON *docs = get_docs_from_code(cpp_code, docs_root_dir, docs_index_path);
    // get the examples
    // read the index file once
    if (faissIndex == NULL) {
        FaissErrorCode err = faiss_read_index_fname(vdb_path, 0, &faissIndex);
        if (err != 0) {
            printf("Error reading index from file: %s\n", faiss_get_last_error());
            return -1;
        }
    }
    
    const int d_embeddings = faiss_Index_d(faissIndex);  // dimension of the vectors

    // get question embeddings
    float *embeddings = get_embedding(question, embedding_model_path);

    // get docs
    int k = 5; // number of nearest neighbors
    int nq = 1; // number of queries
    float *distances = (float*)malloc(nq * k * sizeof(float));
    idx_t *indices = (idx_t*)malloc(nq * k * sizeof(idx_t));

    faiss_Index_search(faissIndex, nq, embeddings, k, distances, indices);
    // printf("Distances:\n");
    // for (int i = 0; i < k; i++) {
    //     printf("%f ", distances[i]);
    // }
    // printf("\nIndices:\n");
    // for (int i = 0; i < k; i++) {
    //     printf("%lld ", indices[i]);
    // }
    // printf("\n");

    printf("Reading exmpales...\n");
    JSON *examples = get_examples(indices, k, index_path);
    printf("\nReading exmpales is done successfully\n");
    
    // Cleanup
    free(embeddings);
    free(distances);
    free(indices);
    
    // refine docs
    printf("\nRefining exmpales ...\n");
    JSON *refined_examples = refine_docs(question, examples, cpp_code, api_key);
    printf("\nRefining exmpales is done successfully\n");

    for (int i = 0 ; i < JSON_A_LEN(docs) ; ++i) {
        const char *formatted_doc = format_scored_doc(JSON2STR(JSON_AT_IDX(docs, i)), 1.0);
        JSON_A_ADD(refined_examples, STR2JSON(formatted_doc));
        free(formatted_doc);
    }

    // get initial answer
    printf("\nPrompting Main Agent ...\n");
    const char *initial_code = prompt_main_agent(question, refined_examples, api_key);
    printf("exited prompt_main_agent function\n");
    if(!initial_code) {
        printf("Invalid pointer!\n");
    }
    printf("initial code:\n%s\n", initial_code);
    printf("\nPrompting Main Agent is done successfully\n");

    // add keyword docs
    printf("\nGetting keyword docs ...\n");
    JSON *keyword_docs = get_docs_from_code(initial_code, docs_root_dir, alusus_features_mapper_path);
    printf("\nGetting keyword docs is done successfully\n");

    // get refined answer
    printf("\nPrompting Alusus Expert ...\n");
    const char *final_answer = prompt_alusus_expert(initial_code, keyword_docs, api_key);
    printf("\nPrompting Alusus Expert is done successfully\n");

    char *formatted_response = format_final_response(final_answer);
    //printf("formatted_response:\n%s\n", formatted_response);
    return formatted_response;
}
