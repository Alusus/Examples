#include "KeywordDocsRetriever.h"
#include "common.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcre.h>


// Function to check if a pattern exists in the code using PCRE
static int feature_exists(JSON* feature, const char* code) {
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
            int rc = pcre_exec(re, NULL, code, strlen(code), 0, 0, ovector, 30);
            pcre_free(re);
            if (rc >= 0) {
                return 1;
            }
        }
    } else {
        for (int i = 0; i < JSON_A_LEN(patterns); i++) {
            const char* pattern = JSON2STR(JSON_AT_IDX(patterns, i));
            if (strstr(code, pattern) != NULL) {
                return 1;
            }
        }
    }
    return 0;
}

static JSON* get_docs_ids(const char* code, const JSON *features_mapper) {
    JSON* docs_ids = json_object_new_array();
    JSON* names = json_object_new_array();
    JSON* result = json_object_new_array();

    for (int i = 0; i < JSON_A_LEN(features_mapper); i++) {
        JSON* feature = JSON_AT_IDX(features_mapper, i);
        if (feature_exists(feature, code)) {
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

    return result;
}

JSON* get_docs_by_keyword(
    const char* code, const char* docs_root_dir,
    const JSON *feature_mapper)
{
    //printf("    getting docs ids ...\n");
    JSON* ids_and_names = get_docs_ids(code, feature_mapper);
    //printf("    getting docs ids is done\n");
    if (ids_and_names == NULL) {
        return NULL;
    }

    JSON* docs_ids = JSON_AT_IDX(ids_and_names, 0);
    JSON* docs = json_object_new_array();

    for (int i = 0; i < JSON_A_LEN(docs_ids); i++) {
        int id = JSON2INT(JSON_AT_IDX(docs_ids, i));
        
        //printf("    reading doc {%i} ...\n", id);
        char *doc_content = read_doc_by_id(id, docs_root_dir);
        //printf("    reading doc {%i} is done\n", id);

        // printf("##############\nDoc %s:\n%s\n##############\n", id, doc_content);
        //printf("    adding doc {%i} to json ...\n", id);
        JSON_A_ADD(docs, STR2JSON(doc_content));
        //printf("    adding doc {%i} to json is done\n", id);
        free(doc_content);
        //printf("    resources freed.\n");
    }

    json_object_put(ids_and_names);
    return docs;
}
