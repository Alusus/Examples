#include "utils.h"
#include <curl/curl.h>
#include <pcre.h>


char* format_question(const char *q) {
    static char formatted_q[512];
    snprintf(
        formatted_q,
        sizeof(formatted_q),
        "%s\nPlease return only the code without any explanation and without comments inside the code",
        q
    );

    for (char *p = strstr(formatted_q, "enAlusus"); p; p = strstr(p, "enAlusus")) {
        strncpy(p, "c++     ", 8); // Ensures the strings have the same length to avoid overflow
    }
    return formatted_q;
}

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    char **response_ptr = (char **)userp;

    // Reallocate memory for response buffer to hold new contents
    *response_ptr = realloc(*response_ptr, strlen(*response_ptr) + total_size + 1);
    if (*response_ptr == NULL) {
        fprintf(stderr, "Failed to reallocate memory for response buffer\n");
        return 0;
    }

    strncat(*response_ptr, (char *)contents, total_size);
    return total_size;
}

// Function to send a request to the OpenAI API
char* send_request(const char *api_key, const char *question, const char *model_tag) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    char *response_buffer = calloc(1024*1024, sizeof(char));  // Allocate initial empty buffer
    char *content_buffer = calloc(512*1024, sizeof(char));  // Allocate initial empty buffer for content

    if (response_buffer == NULL || content_buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory for buffers\n");
        free(response_buffer);
        free(content_buffer);
        return NULL;
    }

    JSON *messages = json_object_new_array();
    JSON *message = json_object_new_object();
    JSON_ADD(message, "role", json_object_new_string("user"));
    JSON_ADD(message, "content", json_object_new_string(question));
    JSON_A_ADD(messages, message);

    JSON *post_data = json_object_new_object();
    JSON_ADD(post_data, "model", json_object_new_string(model_tag));
    JSON_ADD(post_data, "messages", messages);

    const char *post_fields = json_object_to_json_string_ext(post_data, JSON_C_TO_STRING_NOSLASHESCAPE);

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, API_URL);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);

        char auth_header[256];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);

        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, auth_header);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the write callback function to capture the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_buffer);

        // Debug output to see the URL and post data
        // printf("URL: %s\n", API_URL);
        // printf("Question: %s\n", question);
        // printf("Post Fields: %s\n", post_fields);

        // Perform the request and check for errors
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // printf("Request successful\n");
            // printf("Full JSON Response: %s\n", response_buffer);  // Log the full JSON response

            // Parse the response to extract the content
            JSON *response_json = json_tokener_parse(response_buffer);
            if (response_json == NULL) {
                fprintf(stderr, "Failed to parse JSON response\n");
            } else {
                JSON *choices;
                if (!json_object_object_get_ex(response_json, "choices", &choices)) {
                    fprintf(stderr, "Failed to get 'choices' from JSON response\n");
                } else {
                    JSON *first_choice = json_object_array_get_idx(choices, 0);
                    JSON *message_content;
                    if (!json_object_object_get_ex(first_choice, "message", &message_content)) {
                        fprintf(stderr, "Failed to get 'message' from JSON response\n");
                    } else {
                        const char *content = json_object_get_string(json_object_object_get(message_content, "content"));
                        content_buffer = realloc(content_buffer, strlen(content) + 1);
                        if (content_buffer == NULL) {
                            fprintf(stderr, "Failed to reallocate memory for content buffer\n");
                        } else {
                            strncpy(content_buffer, content, strlen(content) + 1);
                        }
                    }
                }
                json_object_put(response_json); // Free the JSON object
            }
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    } else {
        fprintf(stderr, "curl_easy_init() failed\n");
    }
    json_object_put(post_data);

    free(response_buffer);  // Free the response buffer
    return content_buffer;  // Return the content buffer, which should be freed by the caller
}

// Function to get C++ code from the OpenAI API
char* get_cpp_code(const char *api_key, const char *question, const char *model_tag) {
    char *formatted_question = format_question(question);
    return send_request(api_key, formatted_question, model_tag);
}

char* get_rag_question(const char* question, char *combined_docs) {
    // Format the final RAG question string
    const char* format = "please answer the following question"
                         "with the help of the given context about enAlusus programming language.\n"
                         "question: %s,\n"
                         "context: %s,\n"
                         "answer:\n";
    int result_length = snprintf(NULL, 0, format, question, combined_docs);
    char* result = (char*)malloc(result_length + 1);  // +1 for the null terminator
    if (result == NULL) {
        fprintf(stderr, "Failed to allocate memory for result\n");
        free(combined_docs);
        return NULL;
    }

    snprintf(result, result_length + 1, format, question, combined_docs);

    // Clean up
    free(combined_docs);

    return result;
}

char* read_doc(int doc_id, const char *root_dir) {
    char doc_path[512];
    snprintf(doc_path, sizeof(doc_path), "%s/%i.txt", root_dir, doc_id);

    printf("Reading doc [%s] ...\n", doc_path);
    FILE* doc_file = fopen(doc_path, "rb");  // Open file in binary mode
    if (doc_file == NULL) {
        perror("Failed to open document file");
        return NULL;
    }
    fseek(doc_file, 0, SEEK_END);
    long doc_length = ftell(doc_file);
    if (doc_length < 0) {
        perror("Failed to determine file size");
        fclose(doc_file);
        return NULL;
    }
    fseek(doc_file, 0, SEEK_SET);
    char* doc_content = malloc(doc_length + 1);
    if (doc_content == NULL) {
        perror("Failed to allocate memory for document content");
        fclose(doc_file);
        return NULL;
    }
    size_t read_size = fread(doc_content, 1, doc_length, doc_file);
    if (read_size != doc_length) {
        fprintf(stderr, "Failed to read entire file content: Expected %ld, Got %zu\n", doc_length, read_size);
        free(doc_content);
        fclose(doc_file);
        return NULL;
    }
    fclose(doc_file);
    doc_content[doc_length] = '\0';

    return doc_content;
}

// Function to convert a string to lowercase
char* str_to_lowercase(const char* str) {
    char* lower_str = (char*)malloc(strlen(str) + 1);
    if (lower_str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    for (int i = 0; str[i]; i++) {
        lower_str[i] = tolower((unsigned char)str[i]);
    }
    lower_str[strlen(str)] = '\0';
    return lower_str;
}

int parse_config(const char* filename, Config* config) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char key[MAX_LINE_LENGTH];
        char value[MAX_LINE_LENGTH];
        if (sscanf(line, "%s = %s", key, value) == 2) {
            if (strcmp(key, "api_key") == 0) strcpy(config->api_key, value);
            else if (strcmp(key, "docs_root_dir") == 0) strcpy(config->docs_root_dir, value);
            else if (strcmp(key, "basic_docs_index_path") == 0) strcpy(config->basic_docs_index_path, value);
            else if (strcmp(key, "webplatform_docs_index_path") == 0) strcpy(config->webplatform_docs_index_path, value);
            else if (strcmp(key, "rag_index_path") == 0) strcpy(config->rag_index_path, value);
            else if (strcmp(key, "model_tag") == 0) strcpy(config->model_tag, value);
            else if (strcmp(key, "base_model_tag") == 0) strcpy(config->base_model_tag, value);
            else if (strcmp(key, "embedding_model_path") == 0) strcpy(config->embedding_model_path, value);
        }
    }

    fclose(file);
    return 0;
}

JSON* read_json_file(const char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(length + 1);
    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);

    JSON *parsed_json;
    parsed_json = json_tokener_parse(data);

    free(data);

    return parsed_json;
}
