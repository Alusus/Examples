import os

from flask import Flask, request, jsonify
from dotenv import load_dotenv

from inference_engine import BasicEngine, WebPlatformEngine
from utils import get_rag_question

app = Flask(__name__)

# Load the .env file
load_dotenv()

DOCS_ROOT_DIR = '../doc'
WEBPLATFORM_INDEX_PATH = '../libs_index.json'
BASIC_INDEX_PATH = '../docs2id.json'
RAG_INDEX_PATH = '../ivf_index_file.index'

API_KEY = os.getenv("API_KEY")
MODEL_TAG = os.getenv("MODEL_TAG")
BASE_MODEL_TAG = os.getenv("BASE_MODEL_TAG")


@app.route('/basic', methods=['POST'])
def handle_basic():
    data = request.json
    query = data.get('query')

    engine = BasicEngine(API_KEY, MODEL_TAG, BASE_MODEL_TAG, DOCS_ROOT_DIR, BASIC_INDEX_PATH)

    query, data = engine.preprocess(query)
    docs = engine.get_docs(query, data)
    query = get_rag_question(query, docs)

    return jsonify({"response": query})


@app.route('/web_platform', methods=['POST'])
def handle_web_platform():
    data = request.json
    query = data.get('query')

    engine = WebPlatformEngine(API_KEY, MODEL_TAG, WEBPLATFORM_INDEX_PATH, RAG_INDEX_PATH)

    docs = engine.get_docs(query, data)
    query = get_rag_question(query, docs)

    return jsonify({"response": query})


if __name__ == '__main__':
    app.run(debug=True)
