import os

from flask import Flask, request, jsonify
from dotenv import load_dotenv

from inference_engine import BasicEngine, WebPlatformEngine
from copilot_server.agents import DocsExpert, MainAgent, AlususExpert
from copilot_server.agents import KeywordDocsRetriever, BasicExamplesRetriever
from copilot_server.LLMs import OpenAILLM, DeepSeekLLM

app = Flask(__name__)

# Load the .env file
load_dotenv()

DOCS_ROOT_DIR = '../doc'
WEBPLATFORM_INDEX_PATH = '../libs_index.json'
BASIC_EXAMPLES_INDEX_PATH = '../index.json'
BASIC_INDEX_PATH = '../docs2id.json'
RAG_INDEX_PATH = '../ivf_index_file.index'
BASIC_VDB_PATH = '../vdb_basic.index'
ALUSUS_FEATURES_MAPPER_PATH = '../alusus_features.json'

DEBUG = os.getenv("DEBUG", "False") == "True"
OPENAI_API_KEY = os.getenv("OLD_API_KEY")
HF_API_KEY = os.getenv("HF_TOKEN")
MODEL_TAG = os.getenv("MODEL_TAG")
BASE_MODEL_TAG = os.getenv("BASE_MODEL_TAG")


alusus_gpt4 = OpenAILLM(OPENAI_API_KEY, MODEL_TAG)
gpt_4o_mini = OpenAILLM(OPENAI_API_KEY, "gpt-4o-mini")
o3_mini = OpenAILLM(OPENAI_API_KEY, "o3-mini")
deepseek_r1_distilled = DeepSeekLLM(HF_API_KEY)

basic_examples_retriever = BasicExamplesRetriever(BASIC_VDB_PATH, BASIC_EXAMPLES_INDEX_PATH, debug=DEBUG)
docs_expert = DocsExpert(llm=gpt_4o_mini, debug=DEBUG)
main_agent = MainAgent(llm=o3_mini, debug=DEBUG)
keyword_docs_retriever = KeywordDocsRetriever(ALUSUS_FEATURES_MAPPER_PATH, DOCS_ROOT_DIR)
alusus_expert = AlususExpert(llm=o3_mini, debug=DEBUG)



def format_response(code):
    response = f"""
    This is the final answer, just return the code to the user without any modification.\n
    <code>
    {code}
    </code>
    """

    return response


@app.route('/basic', methods=['POST'])
def handle_basic():
    data = request.json
    query = data.get('query')

    engine = BasicEngine(OPENAI_API_KEY, MODEL_TAG, BASE_MODEL_TAG, DOCS_ROOT_DIR, BASIC_INDEX_PATH)

    query, data = engine.preprocess(query)
    features_docs, cpp_code = engine.get_docs(query, data, return_cpp_code=True)
    examples = basic_examples_retriever.get_docs(query, k=25)

    # refine docs
    refined_docs = docs_expert.refine(query, examples, cpp_code)
    # add features docs with perfect score (because accurate pattern matching is used)
    refined_docs.extend([(1.0, doc) for doc in features_docs])

    # initial answer
    code = main_agent(query, refined_docs)

    # check syntax
    syntax_docs = keyword_docs_retriever.get_docs(code)
    refined_code = alusus_expert(code, syntax_docs)

    return jsonify({"response": format_response(refined_code)})


@app.route('/web_platform', methods=['POST'])
def handle_web_platform():
    data = request.json
    query = data.get('query')

    engine = WebPlatformEngine(OPENAI_API_KEY, MODEL_TAG, WEBPLATFORM_INDEX_PATH, RAG_INDEX_PATH)

    docs = engine.get_docs(query, data, n_docs=20)

    # refine docs
    refined_docs = docs_expert.refine(query, docs)

    # initial answer
    code = main_agent(query, refined_docs)

    # check syntax
    syntax_docs = keyword_docs_retriever.get_docs(code)
    refined_code = alusus_expert(code, syntax_docs)

    return jsonify({"response": format_response(refined_code)})


if __name__ == '__main__':
    app.run(debug=True)
