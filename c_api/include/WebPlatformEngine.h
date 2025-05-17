#ifndef WEBPLATFORM_ENGINE_H
#define WEBPLATFORM_ENGINE_H


void webplatform_free_resources();

char* webplatform_run(
  const char *api_key, const char *question,
  const char *docs_index_path, const char *rag_index_path,
  const char *model_tag, const char *model_path,
  const char *alusus_features_mapper_path, const char *docs_root_dir);

#endif // WEBPLATFORM_ENGINE_H
