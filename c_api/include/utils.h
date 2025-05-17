#ifndef UTILS_H
#define UTILS_H

#include <json.h>
#include "common.h"


// Function declarations
char* format_question(const char *q);
char* send_request(const char *api_key, const char *question, const char *model_tag);
char* get_cpp_code(const char *api_key, const char *question, const char *model_tag);
char* get_rag_question(const char* question, char *combined_docs);
char* read_doc_by_path(const char *doc_path);
char* read_doc_by_id(int doc_id, const char *root_dir);
JSON* read_json_file(const char *path);
int parse_config(const char* filename, Config* config);
char* str_to_lowercase(const char* str);
char* get_root_dir(const char *filepath);
char *remove_think_section(const char *input);
void trim_whitespace(char *str);
char* format_final_response(const char *code);

#endif // UTILS_H
