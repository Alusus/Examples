#include "MainAgent.h"
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
    //printf("Removing think section ...\n");
    char *cleaned_output = remove_think_section(llm_output);
    //printf("Removing think section is done\n");
    if (!cleaned_output)
        return "Cannot answer this question";
    //printf("trimming white space ...\n");
    trim_whitespace(cleaned_output);
    if (!cleaned_output) {
        printf("cleaned output is empty!!");
    }
    //printf("trimming white space is done\n");

    //printf("cleaned output:\n%s\n", cleaned_output);

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
    //printf("Executing the pcre ...\n");
    int rc = pcre_exec(re, NULL, cleaned_output, subject_length, 0, 0, ovector, OVECCOUNT);
    //printf("Executing the pcre is done\n");
    
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
        else {
            printf("Failed to allocate space for the extracted code!\n");
        }
    }
    else {
        printf("Failed to find the code!\n");
    }
    pcre_free(re);
    //printf("Resources freed\n");
    return code_str;
}

static char *generate_prompt(const char *query, const JSON *docs) {
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

    // Define fixed parts of the prompt.
    const char *p1 = "You are an expert in programming.\n";
    const char *p2 = "Even though you don't know a lot about Alusus programming language,"
                     "you should be able to use the docs to help you answer the query.\n";
    const char *p3 = "Documents:\n";
    const char *p4 = "\n";
    const char *p5 = "Query: ";
    const char *p6 = "\n";
    const char *p7 = "Answer the query given the documents.\n";
    const char *p8 = "You should take into account the relevance of the documents,"
                     "where 0 means not relevant at all and 1 means highly relevant.\n";
    const char *p9 = "Provide the code between <code></code> tags";

    // Calculate final prompt length.
    size_t final_length = strlen(p1) + strlen(p2) + strlen(p3) +
        strlen(inline_docs) + strlen(p4) + strlen(p5) +
        strlen(query) + strlen(p6) + strlen(p7) + strlen(p8) + strlen(p9) + 1;

    // Allocate memory for the final prompt.
    char *prompt = malloc(final_length);
    if (!prompt) {
        free(inline_docs);
        return NULL;
    }

    // Build the final prompt.
    sprintf(prompt, "%s%s%s%s%s%s%s%s%s%s%s",
            p1, p2, p3, inline_docs, p4, p5, query, p6, p7, p8, p9);

    free(inline_docs);
    return prompt;
}


const char* prompt_main_agent(const char *question, JSON *docs, const char *api_key) {
    const char *prompt = generate_prompt(question, docs);
    //printf("prompt:\n%s\n", prompt);

    const char *response = send_request(api_key, prompt, "o3-mini");
    //printf("response:\n%s\n", response);

    const char *code = clean_and_extract_code(response);

    //printf("code:\n%s\n", code);
    return code;
}
