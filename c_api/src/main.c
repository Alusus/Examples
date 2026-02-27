#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "BasicEngine.h"
#include "WebPlatformEngine.h"


void show_result(const char *result) {
    if (result) {
        printf("***************\nresult:\n%s\n**************\n", result);
        free(result);  // Free the response buffer
    } else {
        printf("Failed to get the result.\n");
    }
}

void log_result(const char *result, const char *question, const char *log_file_path) {
    if (result) {
        FILE *file = fopen(log_file_path, "w");
        if (file) {
            fprintf(file, "======\n%s\n======\n***************\nresult:\n%s\n**************\n", question, result);
            fclose(file);
            printf("Result written to output.log\n");
        } else {
            perror("Failed to open output.log");
        }
        free(result);  // Free the response buffer
    } else {
        printf("Failed to get the result.\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <config_file> <question> <lang> <log_file_path>\n", argv[0]);
        return 1;
    }

    Config cfg;
    if (parse_config(argv[1], &cfg) != 0) {
        return 1;
    }
    const char *question = argv[2];
    const char *lang = argv[3];
    const char *log_file_path = argv[4];

    // dispatch the job to the correct engine
    char *lower_question = str_to_lowercase(question);

    if (strstr(lower_question, "webplatform") != NULL) {
        atexit(webplatform_free_resources);
        char *result =  webplatform_run(
            cfg.api_key, question, lang,
            cfg.webplatform_docs_index_path, cfg.rag_index_path, cfg.ar_docs_dir,
            cfg.model_tag, cfg.embedding_model_path,
            cfg.alusus_features_mapper_path, cfg.docs_root_dir
        );
        // show_result(result);
        log_result(result, question, log_file_path);
    }
    else if (strstr(lower_question, "enalusus") != NULL) {
        atexit(basic_free_resources);
        char *result = basic_run(
            cfg.api_key, question, lang,
            cfg.docs_root_dir, cfg.basic_docs_index_path, cfg.ar_docs_dir,
            cfg.base_model_tag, cfg.model_tag,
            cfg.basic_vdb_path, cfg.basic_index_path,
            cfg.alusus_features_mapper_path, cfg.embedding_model_path
        );
        printf("Exited basic_run\n");
        // show_result(result);
        log_result(result, question, log_file_path);
    }
    else {
        fprintf(stderr, "Error: Question cannot be answered by current supported engines.\n");
        return 1;
    }

    free(lower_question);

    return 0;
}
