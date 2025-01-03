# RAG C API

## Project Overview
This project includes integrations with FAISS, Intel MKL, gflags, llama.cpp, OpenAI API, libcurl, and json-c.

## Requirements
To build and run this project, you need to have the following libraries installed:

* [**FAISS**](https://github.com/facebookresearch/faiss)

* [**Intel oneAPI Math Kernel Library (MKL)**](https://software.intel.com/content/www/us/en/develop/tools/oneapi/components/onemkl.html)

* [**gflags**](https://github.com/gflags/gflags)

* [**llama.cpp**](https://github.com/ggerganov/llama.cpp)

* [**libcurl**](https://github.com/curl/curl)

* [**json-c**](https://github.com/json-c/json-c)


## Building the Project
To build the project, follow these steps:

1. Ensure all required libraries are installed and paths are correctly set in the `CMakeLists.txt` file.
2. Run the following commands in your terminal:
   ```sh
   mkdir build
   cd build
   cmake ..
   cmake --build .

## Configuration
The project requires a configuration file to specify paths and settings. Below is an example configuration file:

```plaintext
api_key = YOUR_API_KEY
docs_root_dir = D:/Alusus/AI/repo/Examples/doc
basic_docs_index_path = D:/Alusus/AI/repo/Examples/docs2id.json
webplatform_docs_index_path = D:/Alusus/AI/repo/Examples/libs_index.json
rag_index_path = D:/Alusus/AI/repo/Examples/ivf_index_file.index
model_tag = ft:gpt-4o-mini-2024-07-18:alusus-software-ltd:rag-v2-alusus:9x6CIKDr
base_model_tag = gpt-4o-mini
embedding_model_path = D:/Alusus/AI/models/all-minilm-l6-v2_f16.gguf
```

### Configuration File Explanation
- **api_key**: Your API key for accessing the OpenAI API.
- **docs_root_dir**: The absolute path to the doc folder in the repo.
- **basic_docs_index_path**: The absolute path to the docs2id.json file in the repo.
- **webplatform_docs_index_path**: The absolute path to the libs_index.json file in the repo.
- **rag_index_path**: he absolute path to the faiss vector database index file.
- **model_tag**: The tag of the specific model to use.
- **base_model_tag**: The tag of the base model. This will be used to answer the intermediate C++ questions in BasicEngine mode. 
- **embedding_model_path**: The path to the embedding model file. This will be used to embed the query before matching it with the vector database. For example, you can use the following model: [all-MiniLM-L6-v2-GGUF](https://huggingface.co/LLukas22/all-MiniLM-L6-v2-GGUF/blob/main/all-minilm-l6-v2_f16.gguf)

