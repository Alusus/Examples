#include "DocsExpert.h"
#include "common.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pcre.h>


static char* generate_prompt(const char* query, const char* doc, const char* cpp_code) {
    const char* part1 =
        "You are an expert in checking whether the documents are related to queries.\n"
        "The document should help answer the query to be relevant.\n"
        "Even if the document could help a little, it should get at least 0.5 score.\n"
        "To give a score, you should think about features needed in the code which will answer the question.\n";
    const char* query_format = "Query: %s\n";
    const char* document_format = "Document: %s\n";
    const char* cpp_header = "C++ Code:\n%s\n";
    const char* cpp_instructions =
        "Use the provided C++ code as a way to help you check if a feature is needed.\n";
    const char* conclusion =
        "Score the relevance of the document to the query on a scale of 0 to 1, "
        "where 0 means not relevant at all and 1 means highly relevant.\n"
        "Provide the score between <score></score> tags.\n";

    size_t len = 0;
    len += strlen(part1);

    // Calculate length needed for the formatted query and document lines
    int query_size = snprintf(NULL, 0, query_format, query);
    len += query_size;
    int document_size = snprintf(NULL, 0, document_format, doc);
    len += document_size;

    // If cpp_code is provided, add lengths for its formatted text
    if (cpp_code != NULL) {
        int cpp_size = snprintf(NULL, 0, cpp_header, cpp_code);
        len += cpp_size;
        len += strlen(cpp_instructions);
    }
    len += strlen(conclusion);

    // Allocate memory for the final prompt (+1 for the null terminator)
    char* prompt = (char*)malloc(len + 1);
    if (!prompt)
        return NULL;

    size_t offset = 0;
    offset += snprintf(prompt + offset, len + 1 - offset, "%s", part1);
    offset += snprintf(prompt + offset, len + 1 - offset, query_format, query);
    offset += snprintf(prompt + offset, len + 1 - offset, document_format, doc);
    
    if (cpp_code != NULL) {
        offset += snprintf(prompt + offset, len + 1 - offset, cpp_header, cpp_code);
        offset += snprintf(prompt + offset, len + 1 - offset, "%s", cpp_instructions);
    }
    snprintf(prompt + offset, len + 1 - offset, "%s", conclusion);

    return prompt;
}

#define OVECCOUNT 30  // Must be a multiple of 3

// Remove <think> section and extract score between <score>...</score>
static float clean_and_extract_score(const char *llm_output) {
    // Step 1: Remove <think> sections
    char *cleaned_output = remove_think_section(llm_output);
    if (!cleaned_output)
        return 0.0;
    trim_whitespace(cleaned_output);

    // Step 2: Compile regex to extract <score>(.*?)</score>
    const char *score_pattern = "<score>(.*?)</score>";
    const char *error;
    int erroffset;
    pcre *re = pcre_compile(score_pattern, 0, &error, &erroffset, NULL);
    if (!re) {
        fprintf(stderr, "PCRE compilation failed at offset %d: %s\n", erroffset, error);
        free(cleaned_output);
        return 0.0;
    }

    int ovector[OVECCOUNT];
    int subject_length = (int)strlen(cleaned_output);
    double score = 0.0;
    int rc = pcre_exec(re, NULL, cleaned_output, subject_length, 0, 0, ovector, OVECCOUNT);
    if (rc >= 2) {  // Ensure that group 1 (the captured score) exists
        int start = ovector[2];
        int end   = ovector[3];
        int len   = end - start;
        char *score_str = malloc(len + 1);
        if (score_str) {
            memcpy(score_str, cleaned_output + start, len);
            score_str[len] = '\0';
            score = strtod(score_str, NULL);
            free(score_str);
        }
    } else {
        // Default to 0.0 if no <score> tags are found
        score = 0.0;
    }

    pcre_free(re);
    free(cleaned_output);
    return score;
}

static float compute_score(const char *query, const char *doc, const char *cpp_code, const char *api_key) {
    const char *prompt = generate_prompt(query, doc, cpp_code);

    const char *response = send_request(api_key, prompt, "gpt-4o-mini");

    return clean_and_extract_score(response);
}

// Structure holding a document and its score.
typedef struct {
    char *doc;
    float score;
} RefinedDoc;

// Comparison function for qsort: sorts ascending by score.
static int compare_refined_doc(const void *a, const void *b) {
    const RefinedDoc *docA = (const RefinedDoc *)a;
    const RefinedDoc *docB = (const RefinedDoc *)b;
    if (docA->score < docB->score)
        return -1;
    else if (docA->score > docB->score)
        return 1;
    return 0;
}

const char* format_scored_doc(const char *doc, float score) {
    size_t total_size = strlen(doc) + 128;
    char *formatted_doc = (char*)malloc(total_size);

    snprintf(
        formatted_doc,
        total_size,
        "doc (relevant score: %.2f):\n%s\n",
        score,
        doc
    );

    return formatted_doc;
}

JSON* refine_docs(const char *query, const JSON *docs, const char *cpp_code, const char *api_key) {
    int total_docs = JSON_A_LEN(docs);
    
    // Allocate an array for storing refined docs (maximum possible size = total_docs)
    RefinedDoc *refinedArr = malloc(total_docs * sizeof(RefinedDoc));
    if (!refinedArr) {
        // Handle allocation error (for example, you might return NULL)
        return NULL;
    }
    int count = 0;

    // Process each document: compute its score and store it if it passes the threshold.
    for (int i = 0; i < total_docs; i++) {
        const char *doc = JSON2STR(JSON_AT_IDX(docs, i));
        float score = compute_score(query, doc, cpp_code, api_key);

        if (score < 0.5) {
            continue;
        }

        // Use strdup to copy the document string (you may change this depending on ownership)
        refinedArr[count].doc = strdup(doc);
        refinedArr[count].score = score;
        count++;
    }

    // Sort the refined documents array by score (lowest to highest)
    qsort(refinedArr, count, sizeof(RefinedDoc), compare_refined_doc);

    // Create a new JSON array and add the sorted documents.
    JSON *sorted_docs = json_object_new_array();
    for (int i = 0; i < count; i++) {
        // Assuming json_object_get_string creates a JSON string object from a C string.
        const char *formatted_doc = format_scored_doc(refinedArr[i].doc, refinedArr[i].score);
        JSON_A_ADD(sorted_docs, STR2JSON(formatted_doc));
        free(refinedArr[i].doc);  // Free the duplicated string when done.
        free(formatted_doc);
    }
    free(refinedArr);

    return sorted_docs;
}
