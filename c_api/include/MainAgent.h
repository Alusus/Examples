#ifndef MAIN_AGENT_H
#define MAIN_AGENT_H

#include <json.h>
#include "common.h"

// Function declarations
const char* prompt_main_agent(const char *question, JSON *docs, const char *api_key);

#endif // MAIN_AGENT_H
