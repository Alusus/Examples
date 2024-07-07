import json
import random

from utils import format, debug, export
from utils import get_cpp_code, get_docs, get_rag_question

from openai import OpenAI


api_key = input("Enter api key: ")
client = OpenAI(api_key=api_key)

DOCS_DIR = r"./doc"

TEST_QUESTIONS = [
    # ArrayRelatedOperations
    'check_any',
    'min_in_array',
    'reduce',
    'rfind',
    'std',
    # Geometry
    'farthest_point',
    'matrix_vector_multiplication',
    'triangle_area',
    # GraphAlgorithms
    'is_tree',
    'selection_sort',
    # StringRelatedOperations
    'ends_with',
    'find_char',
    'to_upper'
]


def build(index_path):
    def read_meta(meta_data):
        file_path = meta_data['enAlususAnswer']
        with open(file_path, encoding='utf-8') as code_file:
            code = code_file.read()
        
        question = f"Using enAlusus, {meta_data['enPrompt']}"

        return question, code
    
    def add_context(question):
        cpp_code = get_cpp_code(client, question)
        docs = get_docs(cpp_code, DOCS_DIR)
        rag_question = get_rag_question(question, docs)

        return rag_question
    
    def get_sample(meta_data):
        question, code = read_meta(meta_data)
        question = add_context(question)
        return question, code

    data = json.load(open(index_path, encoding='utf-8'))
    random.shuffle(data)

    train_samples = []
    test_samples = []

    for meta_data in data:
        filename = meta_data[f'cAnswer'].split('/')[-1]
        algo_type = filename.split('.')[0]
        if algo_type in TEST_QUESTIONS:
            test_samples.append(get_sample(meta_data))
        else:
            train_samples.append(get_sample(meta_data))
    
    return train_samples, test_samples

train_samples, test_samples = build('./index.json')
train_samples = format(train_samples)
test_samples = format(test_samples)

debug(f'train size: {len(train_samples)}')
debug('train examples:', train_samples[:3])
debug(f'test size: {len(test_samples)}')
debug('test examples:', test_samples[:3])

export(train_samples, 'rag_train_set')
export(test_samples, 'rag_test_set')
