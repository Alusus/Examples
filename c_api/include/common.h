#ifndef COMMON_H
#define COMMON_H


// API details
#define API_URL "https://api.openai.com/v1/chat/completions"

// JSON-C
typedef struct json_object JSON;
#define JSON_AT(json_obj, key) json_object_object_get(json_obj, key)
#define JSON_REF(json_obj, key, json_ref) json_object_object_get_ex(json_obj, key, &json_ref)
#define JSON_AT_IDX(json_obj, idx) json_object_array_get_idx(json_obj, idx)
#define JSON_A_LEN(json_obj) json_object_array_length(json_obj)
#define JSON_ADD(json_obj, key, value) json_object_object_add(json_obj, key, value)
#define JSON_A_ADD(json_obj, json_val) json_object_array_add(json_obj, json_val)
#define JSON2STR(json_obj) json_object_get_string(json_obj)
#define JSON2INT(json_obj) json_object_get_int(json_obj)

// Configs
#define MAX_LINE_LENGTH 256
typedef struct {
    char api_key[MAX_LINE_LENGTH];
    char docs_root_dir[MAX_LINE_LENGTH];
    char basic_docs_index_path[MAX_LINE_LENGTH];
    char webplatform_docs_index_path[MAX_LINE_LENGTH];
    char rag_index_path[MAX_LINE_LENGTH];
    char model_tag[MAX_LINE_LENGTH];
    char base_model_tag[MAX_LINE_LENGTH];
    char embedding_model_path[MAX_LINE_LENGTH];
} Config;

#endif // COMMON_H
