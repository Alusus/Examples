#include "AlususExpert.h"
#include "common.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pcre.h>


#define OVECCOUNT 30  // Must be a multiple of 3

// Remove <think> section and extract code between <code>...</code>
static char* clean_and_extract_code(const char *llm_output) {
    // Step 1: Remove <think> sections
    char *cleaned_output = remove_think_section(llm_output);
    if (!cleaned_output)
        return "Cannot answer this question";
    trim_whitespace(cleaned_output);

    // Regular expression to capture text between <code> and </code>
    const char *pattern = "<code>(.*?)</code>";
    const char *error;
    int erroffset;
    
    // Compile the pattern with PCRE_DOTALL to allow matching newline characters.
    pcre *re = pcre_compile(pattern, PCRE_DOTALL, &error, &erroffset, NULL);
    if (!re) {
        fprintf(stderr, "PCRE compilation failed at offset %d: %s\n", erroffset, error);
        return NULL;
    }
    
    int ovector[OVECCOUNT];
    int subject_length = (int)strlen(cleaned_output);
    int rc = pcre_exec(re, NULL, cleaned_output, subject_length, 0, 0, ovector, OVECCOUNT);
    
    char *code_str = NULL;
    if (rc >= 2) {
        // Group 1 (the captured content) indices
        int start = ovector[2];
        int end   = ovector[3];
        int len   = end - start;
        code_str = (char*)malloc(len + 1);
        if (code_str) {
            for (int i = start ; i < end ; ++i) {
                code_str[i-start] = cleaned_output[i];
            }
            // memcpy(code_str, cleaned_output + start, len);
            code_str[len] = '\0';
        }
    }
    
    pcre_free(re);
    return code_str;
}

static char* generate_prompt(const char *code, const JSON *docs) {
    const char *separator = "\n******\n";
    size_t sep_len = strlen(separator);
    int num_docs = JSON_A_LEN(docs);
    size_t inline_docs_len = 0;

    // Compute total length for joined documents.
    for (int i = 0; i < num_docs; i++) {
        const char *doc = JSON2STR(JSON_AT_IDX(docs, i));
        inline_docs_len += strlen(doc);
        if (i < num_docs - 1) {
            inline_docs_len += sep_len;
        }
    }

    // Allocate memory for the joined docs. +1 for the null-terminator.
    char *inline_docs = malloc(inline_docs_len + 1);
    if (!inline_docs) {
        return NULL;
    }
    inline_docs[0] = '\0';  // Start with an empty string

    // Concatenate all docs with the separator.
    for (int i = 0; i < num_docs; i++) {
        const char *doc = JSON2STR(JSON_AT_IDX(docs, i));
        strcat(inline_docs, doc);
        if (i < num_docs - 1) {
            strcat(inline_docs, separator);
        }
    }
    size_t prompt_size = strlen(inline_docs) + strlen(code) + 512;
    char *prompt = (char*)malloc(prompt_size);
    if (!prompt) {
        free(inline_docs);
        return NULL;
    }

    // Format the prompt
    snprintf(prompt, prompt_size, 
        "You are an expert in Alusus programming language.\n"
        "You should be able to use the docs to help you check the code for syntax error and correct them.\n"
        "If something is not mentioned in the docs leave it as it is, do not make assumptions on your own!\n"
        "Documents:\n%s\n"
        "Code:\n"
        "```%s```\n"
        "Do not modify the approach or the code logic even if it's wrong! just correct any syntax errors.\n"
        "Please think about what you should correct between <think></think> tags.\n"
        "Please put the full code after correction between <code></code> tags.\n"
        "The code will be treated as a final answer so make sure it's complete!\n"
        "Some syntax errors may be repeated across the code, make sure that everything is corrected!\n"
        "If a feature is described in docs, any other way of using that feature must be wrong!\n",
        inline_docs, code);

    free(inline_docs);
    return prompt;
}

const char* prompt_alusus_expert(const char *code, JSON *syntax_docs, const char *api_key) {
    const char *prompt = generate_prompt(code, syntax_docs);

    const char *response = send_request(api_key, prompt, "o3-mini");

    // printf("response:\n%s\n", response);

    const char *clean_response = clean_and_extract_code(response);

    // printf("code:\n%s\n", clean_response);
    return clean_response;
}
