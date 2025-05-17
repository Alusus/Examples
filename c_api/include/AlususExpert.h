#ifndef ALUSUS_EXPERT_H
#define ALUSUS_EXPERT_H

#include <json.h>
#include "common.h"

// Function declarations
const char* prompt_alusus_expert(const char *code, JSON *syntax_docs, const char *api_key);

#endif // ALUSUS_EXPERT_H
