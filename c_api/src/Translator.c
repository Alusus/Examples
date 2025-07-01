#include "Translator.h"
#include "common.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pcre.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dirent.h>
    #include <sys/types.h>
#endif


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
    size_t num_docs = JSON_A_LEN(docs);
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
    size_t prompt_size = strlen(inline_docs) + strlen(code) + 4096;
    char *prompt = (char*)malloc(prompt_size);
    if (!prompt) {
        free(inline_docs);
        return NULL;
    }
    
    // Format the prompt
    printf("\tFilling the prompt...\n");
    snprintf(prompt, prompt_size, 
        "Alusus is a programming language with syntax in English and Arabic.\n"
        "Your task will be to translate from the English syntax to the Arabic syntax\n"
        "Please follow this rules:\n"
        "1. Variables names and user-defined functions names should always be translated and must be consistent.\n"
        "2. Language reserved keywords and built-in functions should be translated only if present in the docs,"
            "otherwise, leave them as they are.\n"
        "3. You should only translate, do not change the approach or the code structure.\n"
        "4. comments and print strings literals should be translated and written in Arabic.\n"
        "5. Check your work twice before providing the final answer.\n"
        "6. Make sure to match anything in the docs with the code and translate it, sometimes there"
            "will be hierarchical structure so make sure to account for that.\n"
        "7. To translate effectively, move through the code line by line, and translate according to the docs.\n"
        "Documents:\n%s\n"
        "Code:\n"
        "```%s```\n"
        "Please think about what you should translate between <think></think> tags.\n"
        "Please put the full code after translation between <code></code> tags.\n",
        inline_docs, code);

    printf("\tCleaning up...\n");
    free(inline_docs);
    return prompt;
}

static JSON* list_dir(const char *path) {
    JSON *files_names = json_object_new_array();

    #ifdef _WIN32
        char search_path[MAX_PATH];
        snprintf(search_path, sizeof(search_path), "%s\\*", path);

        WIN32_FIND_DATAA fd;
        HANDLE hFind = FindFirstFileA(search_path, &fd);

        if (hFind == INVALID_HANDLE_VALUE) {
            fprintf(stderr, "Failed to open directory: %s\n", path);
            return NULL;
        }

        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                printf("\t\tFound file: %s\n", fd.cFileName);
                JSON_A_ADD(files_names, STR2JSON(fd.cFileName));
            }
        } while (FindNextFileA(hFind, &fd));
        FindClose(hFind);
    #else
        DIR *dir = opendir(path);
        if (!dir) {
            perror("opendir");
            return NULL;
        }

        struct dirent *entry;
        while ((entry = readdir(dir))) {
            if (entry->d_type != DT_DIR) {
                printf("\t\tFound file: %s\n", entry->d_name);
                JSON_A_ADD(files_names, STR2JSON(entry->d_name));
            }
        }
        closedir(dir);
    #endif

    return files_names;
}

static JSON* get_mapping_meta_data(const char *ar_docs_dir)
{
    char translator_mapping_path[PATH_MAX];
    snprintf(
        translator_mapping_path,
        sizeof(translator_mapping_path),
        "%s/translator_mapping.json",
        ar_docs_dir);
    JSON *translator_mapping = read_json_file(translator_mapping_path);

    JSON *direct_mapping = json_object_new_object();

    char basic_direct_mapping_path[PATH_MAX];
    snprintf(
        basic_direct_mapping_path,
        sizeof(basic_direct_mapping_path),
        "%s/direct_mapping.json",
        ar_docs_dir);
    JSON_ADD(direct_mapping, "basic", read_json_file(basic_direct_mapping_path));

    // add libraries meta data
    char libs_dir[PATH_MAX];
    snprintf(libs_dir, sizeof(libs_dir), "%s/libs", ar_docs_dir);

    JSON *features_names  = json_object_new_object();

    for (int lib_id = 0 ; lib_id < num_libraries ; lib_id++) {
        const char *lib_name = library_names[lib_id];
        printf("\tgettings meta data for library '%s'\n", lib_name);

        // Build full path to this lib folder
        char lib_path[PATH_MAX];
        snprintf(lib_path, sizeof(lib_path), "%s/%s", libs_dir, lib_name);

        JSON *lib_features_names = json_object_new_array();

        printf("\tListing library files\n");
        JSON *files_names = list_dir(lib_path);
        printf("\tStoring the features names\n");
        for (int f_id = 0 ; f_id < JSON_A_LEN(files_names) ; f_id++) {
            const char *filename = JSON2STR(JSON_AT_IDX(files_names, f_id));

            // skip our mapping file and hidden files
            if (strcmp(filename, "direct_mapping.json") == 0 ||
                filename[0] == '.') {
                continue;
            }

            /* strip extension */
            char *dot = strrchr(filename, '.');
            size_t len = dot ? (size_t)(dot - filename) : strlen(filename);
            char name_only[256];
            strncpy(name_only, filename, len);
            name_only[len] = '\0';

            JSON_A_ADD(lib_features_names, STR2JSON(name_only));
        }

        printf("\tadding library features to the meta data\n");
        // add to features_names JSON object
        JSON_ADD(features_names,
                 lib_name,
                 lib_features_names);
    
        // Load direct_mapping.json for this lib
        char mapping_path[PATH_MAX];
        snprintf(mapping_path, sizeof(mapping_path), "%s/direct_mapping.json", lib_path);
    
        printf("\tadding direct mapping of library '%s'\n", lib_name);
        JSON_ADD(direct_mapping, lib_name, read_json_file(mapping_path));
    }

    JSON *meta_data = json_object_new_object();
    JSON_ADD(meta_data, "translator_mapping", translator_mapping);
    JSON_ADD(meta_data, "direct_mapping", direct_mapping);
    JSON_ADD(meta_data, "features_names", features_names);

    return meta_data;
}

static int feature_exists(JSON* feature, const char* code) {
    JSON* regex;
    JSON* patterns;
    JSON_REF(feature, "regex", &regex);
    JSON_REF(feature, "patterns", &patterns);

    if (json_object_get_boolean(regex)) {
        for (int i = 0; i < JSON_A_LEN(patterns); i++) {
            const char* pattern = JSON2STR(JSON_AT_IDX(patterns, i));
            const char* error;
            int erroffset;
            pcre* re = pcre_compile(pattern, PCRE_DOTALL | PCRE_MULTILINE, &error, &erroffset, NULL);
            if (re == NULL) {
                continue;
            }
            int ovector[30];
            int rc = pcre_exec(re, NULL, code, strlen(code), 0, 0, ovector, 30);
            pcre_free(re);
            if (rc >= 0) {
                return 1;
            }
        }
    } else {
        for (int i = 0; i < JSON_A_LEN(patterns); i++) {
            const char* pattern = JSON2STR(JSON_AT_IDX(patterns, i));
            if (strstr(code, pattern) != NULL) {
                return 1;
            }
        }
    }
    return 0;
}

static char* get_direct_mapping_doc(JSON *direct_mapping, const char *code, const char* title) {
    JSON *docs = json_object_new_array();
    size_t docs_size = 0;

    printf("\t\tLooping through the keywords...\n");
    json_object_object_foreach(direct_mapping, key, val) {
        if (strstr(code, key) != NULL) {
            const char *ar_keyword = JSON2STR(val);
            size_t doc_size = strlen(key) + strlen(ar_keyword) + 32;
            char *doc = (char*)malloc(doc_size);
            snprintf(
                doc,
                doc_size,
                "'%s': '%s'",
                key,
                ar_keyword
            );
            printf("\t\t\t%s\n", doc);

            JSON_A_ADD(docs, STR2JSON(doc));
            docs_size += strlen(doc);

            free(doc);
        }
    }
    printf("\t\tEND\n");

    char *inline_docs = (char*)malloc(docs_size + strlen(title) + 1024);
    inline_docs[0] = '\0';

    strcat(inline_docs, title);
    
    int num_docs = JSON_A_LEN(docs);
    const char *separator = "\n";
    for (int i = 0 ; i < num_docs ; i++) {
        const char *doc = JSON2STR(JSON_AT_IDX(docs, i));

        strcat(inline_docs, separator);
        strcat(inline_docs, doc);
    }

    printf("\t\tCleaning up...\n");
    json_object_put(docs);

    return inline_docs;
}

static JSON* get_docs(const char *code, JSON *used_libs, JSON *meta_data, const char *ar_docs_dir)
{
    JSON *docs = json_object_new_array();

    // Extract related basic docs
    JSON *translator_mapping;
    JSON_REF(meta_data, "translator_mapping", &translator_mapping);

    printf("\tAdding basic features...\n");
    for (int entry_id = 0 ; entry_id < JSON_A_LEN(translator_mapping) ; entry_id++) {
        JSON *entry = JSON_AT_IDX(translator_mapping, entry_id);
        if (feature_exists(entry, code)) {
            printf("\t\tAdding feature %s...\n", JSON2STR(JSON_AT(entry, "filename")));
            char doc_path[PATH_MAX];
            snprintf(
                doc_path,
                sizeof(doc_path),
                "%s/%s",
                ar_docs_dir,
                JSON2STR(JSON_AT(entry, "filename")));
            
            const char *doc = read_doc_by_path(doc_path);

            size_t doc_size = strlen(doc) + 256;
            char *formatted_doc = (char*)malloc(doc_size);
            snprintf(
                formatted_doc,
                doc_size,
                "doc about '%s':\n%s",
                JSON2STR(JSON_AT(entry, "name")),
                doc
            );

            JSON_A_ADD(docs, STR2JSON(formatted_doc));

            free(formatted_doc);
            free(doc);
        }
    }

    JSON *direct_mapping;
    JSON_REF(meta_data, "direct_mapping", &direct_mapping);
    JSON *basic_direct_mapping;
    JSON_REF(direct_mapping, "basic", &basic_direct_mapping);

    printf("\tAdding basic keyword mapping...\n");
    const char *basic_direct_mapping_doc = get_direct_mapping_doc(basic_direct_mapping, code, "Keyword mapping dictionary:");
    //printf("\t**basic dmap: %s***\n", basic_direct_mapping_doc);
    JSON_A_ADD(docs, STR2JSON(basic_direct_mapping_doc));

    // Extract related libraries docs
    if (used_libs == NULL) {
        printf("\tNo library used!\n");
        return docs;
    }

    JSON *features_names;
    JSON_REF(meta_data, "features_names", &features_names);

    for (int lib_idx = 0 ; lib_idx < JSON_A_LEN(used_libs) ; lib_idx++) {
        const char *lib_name = JSON2STR(JSON_AT_IDX(used_libs, lib_idx));

        JSON *lib_features_names;
        JSON_REF(features_names, lib_name, &lib_features_names);

        for (int i = 0 ; i < JSON_A_LEN(lib_features_names); i++) {
            const char *feature_name = JSON2STR(JSON_AT_IDX(lib_features_names, i));
            if (strstr(code, feature_name) != NULL) {
                char doc_path[PATH_MAX];
                snprintf(
                    doc_path,
                    sizeof(doc_path),
                    "%s/libs/%s/%s.txt",
                    ar_docs_dir,
                    lib_name,
                    feature_name);
                
                const char *doc = read_doc_by_path(doc_path);

                size_t formatted_doc_size = strlen(doc) + 256;
                char *formatted_doc = (char*)malloc(formatted_doc_size);
                snprintf(
                    formatted_doc,
                    formatted_doc_size,
                    "doc about '%s':\n%s",
                    feature_name,
                    doc
                );

                JSON_A_ADD(docs, STR2JSON(formatted_doc));

                free(formatted_doc);
                free(doc);
            }
        }

        JSON *lib_direct_mapping;
        JSON_REF(direct_mapping, lib_name, &lib_direct_mapping);

        char *title[256];
        snprintf(title, sizeof(title), "%s library dictionary:", lib_name);

        const char *lib_direct_mapping_doc = get_direct_mapping_doc(lib_direct_mapping, code, title);
        JSON_A_ADD(docs, STR2JSON(lib_direct_mapping_doc));
    }

    return docs;
}

const char* prompt_translator(
    const char *code,
    const char *lang,
    const char *ar_docs_dir,
    const char *api_key,
    JSON *used_libs)
{
    // no need for translation
    if (strcmp(lang, "en") == 0) {
        return code;
    }

    printf("Getting meta data...\n");
    JSON *meta_data = get_mapping_meta_data(ar_docs_dir);
    printf("Fetching docs...\n");
    JSON *docs = get_docs(code, used_libs, meta_data, ar_docs_dir);

    if (docs == NULL) {
        printf("No docs were fetched!\n");
    }

    // Note: this size if chosen heuristically, better use dynamic size
    char *code_sketch = (char*)malloc(strlen(code)*4);
    strcpy(code_sketch, code);

    for (int i = 0 ; i < 3 ; ++i) {
        printf("#%d Refinement\n", i);
        printf("Preparing the prompt...\n");
        const char *prompt = generate_prompt(code_sketch, docs);
        printf("Asking AI...\n");
        const char *response = send_request(api_key, prompt, "o3-mini");
        printf("Formatting the output...\n");
        const char *clean_response = clean_and_extract_code(response);

        // refine the code sketch
        strcpy(code_sketch, clean_response);

        // clean up
        free(prompt);
        free(response);
        free(clean_response);
    }

    return code_sketch;
}
