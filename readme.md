# Examples
Miscellaneous examples written in Alusus language with their counterparts in other languages, like C.


# Webplatform support

To support questions about the `WebPlatform` library **RAG** technique is used.
Briefly, the GPT model that we have trained on Alusus language is used as the main model, and we augment it
with code snippets and documentation relevent to the question asked by the user.

## Step 1: Creating the vector database

Information about WebPlatform library (code snippets and documentation) should be stored as embeddings vectors
since we need to be able to measure their similarity to the user question.
This is done in `build_vector_index.py` script.

This script generate the database from the examples which their metadata is stored in `libs_index.json`.

## Step 2: Inference

### Inference Engine

A customized inference engine is implemented in `inference_engine.py` to support the `WebPlatform` library.

You should provide this Engine with:

* `api_key`: OpenAI API key.
* `model_tag`: The model to be used for prediction. In our case it should be the GPT model trained on Alusus examples.
* `docs_index_path`: The path to the libraries metadata JSON file. In our case it's `./libs_index.json`.
* `rag_index_path`: The path of the vector database file. In our case it's `./ivf_index_file.index`.

### Inference Script

This just a script for configuring different options for the engines. It uses a function to automatically
determine the right engine to be used.

In this script, the `base_model_tag` is not used, since it's related to the way we used RAG to train the
main Alusus GPT model. In that model we used the original GPT for generating a intermediate C++ code.
