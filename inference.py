from inference_engine import get_inference_engine

# TODO: use env file for these configurations
DOCS_ROOT_DIR = './doc'
WEBPLATFORM_INDEX_PATH = './libs_index.json'
RAG_INDEX_PATH = './ivf_index_file.index'

api_key = input("Enter api key: ")
model_tag = input("Enter the model tag to be use for inference")
base_model_tag = input("Enter the base model tag that used in the main model")
question = input("Enter your question: ")


inf_engine = get_inference_engine(
    question,
    api_key=api_key,
    model_tag=model_tag,
    base_model_tag=base_model_tag,
    docs_root_dir=DOCS_ROOT_DIR,
    docs_index_path=WEBPLATFORM_INDEX_PATH,
    rag_index_path=RAG_INDEX_PATH
)

answer = inf_engine.run(question)
print(answer)
