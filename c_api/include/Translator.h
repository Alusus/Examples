#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <json.h>
#include "common.h"

// Function declarations
const char* prompt_translator(
    const char *code,
    const char *lang,
    const char *ar_docs_dir,
    const char *api_key,
    JSON *used_libs);

#endif // TRANSLATOR_H
