#ifndef DOCS_EXPERT_H
#define DOCS_EXPERT_H

#include <json.h>
#include "common.h"

// Function declarations
const char* format_scored_doc(const char *doc, float score);
JSON* refine_docs(const char *query, const JSON *docs, const char *cpp_code, const char *api_key);

#endif // DOCS_EXPERT_H
