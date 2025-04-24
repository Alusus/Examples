#ifndef BASIC_ENGINE_H
#define BASIC_ENGINE_H


// Function declarations
char* basic_run(
    const char *api_key, const char *question,
    const char* docs_root_dir, const char *docs_index_path,
    const char *base_model_tag, const char *model_tag);

#endif // BASIC_ENGINE_H
