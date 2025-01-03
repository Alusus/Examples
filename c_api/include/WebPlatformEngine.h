#ifndef WEBPLATFORM_ENGINE_H
#define WEBPLATFORM_ENGINE_H


void free_resources();

char* webplatform_run(
  const char *api_key, const char *question,
  const char *docs_index_path, const char *rag_index_path,
  const char *model_tag, const char *model_path);

#endif // WEBPLATFORM_ENGINE_H
