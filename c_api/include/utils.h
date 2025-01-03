#ifndef UTILS_H
#define UTILS_H

#include <json.h>
#include "common.h"


// Function declarations
char* format_question(const char *q);
char* send_request(const char *api_key, const char *question, const char *model_tag);
char* get_cpp_code(const char *api_key, const char *question, const char *model_tag);
char* get_rag_question(const char* question, char *combined_docs);
char* read_doc(int doc_id, const char *root_dir);
JSON* read_json_file(const char *path);
int parse_config(const char* filename, Config* config);
char* str_to_lowercase(const char* str);

#endif // UTILS_H
