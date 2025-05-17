#ifndef BASIC_ENGINE_H
#define BASIC_ENGINE_H


void basic_free_resources();

// Function declarations
char* basic_run(
    const char *api_key, const char *question,
    const char* docs_root_dir, const char *docs_index_path,
    const char *base_model_tag, const char *model_tag,
    const char *vdb_path, const char *index_path,
    const char *alusus_features_mapper_path, const char *embedding_model_path);

#endif // BASIC_ENGINE_H
