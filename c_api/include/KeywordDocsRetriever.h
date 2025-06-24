#ifndef KEYWORD_DOCS_RETRIEVER_H
#define KEYWORD_DOCS_RETRIEVER_H

#include <json.h>
#include "common.h"

// Function declarations
JSON* get_docs_by_keyword(
    const char* code, const char* docs_root_dir,
    const JSON *feature_mapper);

#endif // KEYWORD_DOCS_RETRIEVER_H
