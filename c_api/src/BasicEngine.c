#include "BasicEngine.h"
#include "common.h"
#include "utils.h"
#include <c_api/Index_c.h>
#include <c_api/error_c.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pcre.h>


// Function to check if a pattern exists in the code using PCRE
static int exists(JSON* feature, const char* cpp_code) {
    JSON* regex;
    JSON* patterns;
    JSON_REF(feature, "regex", &regex);
    JSON_REF(feature, "patterns", &patterns);

    if (json_object_get_boolean(regex)) {
        for (int i = 0; i < JSON_A_LEN(patterns); i++) {
            const char* pattern = JSON2STR(JSON_AT_IDX(patterns, i));
            const char* error;
            int erroffset;
            pcre* re = pcre_compile(pattern, PCRE_DOTALL | PCRE_MULTILINE, &error, &erroffset, NULL);
            if (re == NULL) {
                continue;
            }
            int ovector[30];
            int rc = pcre_exec(re, NULL, cpp_code, strlen(cpp_code), 0, 0, ovector, 30);
            pcre_free(re);
            if (rc >= 0) {
                return 1;
            }
        }
    } else {
        for (int i = 0; i < JSON_A_LEN(patterns); i++) {
            const char* pattern = JSON2STR(JSON_AT_IDX(patterns, i));
            if (strstr(cpp_code, pattern) != NULL) {
                return 1;
            }
        }
    }
    return 0;
}

// Function to get document IDs and names based on C++ code
static JSON* get_docs_ids(const char* cpp_code, const char *docs_index_path) {
    JSON* c_features_mapper = read_json_file(docs_index_path);

    JSON* docs_ids = json_object_new_array();
    JSON* names = json_object_new_array();
    JSON* result = json_object_new_array();

    for (int i = 0; i < JSON_A_LEN(c_features_mapper); i++) {
        JSON* feature = JSON_AT_IDX(c_features_mapper, i);
        if (exists(feature, cpp_code)) {
            JSON* ids;
            JSON* name;
            JSON_REF(feature, "ids", &ids);
            JSON_REF(feature, "name", &name);
            for (int j = 0; j < JSON_A_LEN(ids); j++) {
                JSON_A_ADD(docs_ids, JSON_AT_IDX(ids, j));
            }
            JSON_A_ADD(names, name);
        }
    }

    JSON_A_ADD(result, docs_ids);
    JSON_A_ADD(result, names);

    json_object_put(c_features_mapper);
    return result;
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

// Function to get document contents based on C++ code
static JSON* get_docs(
  const char* cpp_code, const char* docs_root_dir,
  const char *docs_index_path) {

    JSON* ids_and_names = get_docs_ids(cpp_code, docs_index_path);
    if (ids_and_names == NULL) {
        return NULL;
    }

    JSON* docs_ids = JSON_AT_IDX(ids_and_names, 0);
    JSON* docs = json_object_new_array();

    for (int i = 0; i < JSON_A_LEN(docs_ids); i++) {
        int id = JSON2INT(JSON_AT_IDX(docs_ids, i));

        char *doc_content = read_doc(id, docs_root_dir);

        // printf("##############\nDoc %s:\n%s\n##############\n", id, doc_content);
        JSON_A_ADD(docs, json_object_new_string(doc_content));
        free(doc_content);
    }

    json_object_put(ids_and_names);
    return docs;
}

char* basic_run(
  const char *api_key, const char *question,
  const char* docs_root_dir, const char *docs_index_path,
  const char *base_model_tag, const char *model_tag) {

    char *cpp_code = get_cpp_code(api_key, question, base_model_tag);
    printf("***************\ncpp_code:\n %s\n**************\n", cpp_code);
    JSON* docs = get_docs(cpp_code, docs_root_dir, docs_index_path);

    const char *rag_question = get_rag_question(question, combine_docs(docs));

    return send_request(api_key, rag_question, model_tag);
}
