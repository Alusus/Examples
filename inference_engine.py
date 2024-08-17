import os
import json

from openai import OpenAI
from utils import get_rag_question, get_cpp_code, get_docs


def get_inference_engine(question, **kwargs):
    if 'webplatform' in question.lower():
        return WebPlatformEngine(kwargs['api_key'], kwargs['model_tag'], kwargs['docs_index_path'])
    elif 'enalusus' in question.lower():
        return BasicEngine(kwargs['api_key'], kwargs['model_tag'], kwargs['base_model_tag'], kwargs['docs_root_dir'])
    else:
        raise ValueError('Question cannot be answered by current supported engines.')


class Engine:
    def __init__(self, api_key, model_tag):
        self.client = OpenAI(api_key=api_key)
        self.model_tag = model_tag

    def preprocess(self, question: str):
        return question, None
    
    def get_docs(self, question: str, data: dict):
        return []

    def run(self, question):
        question, data = self.preprocess(question)
        docs = self.get_docs(question, data)
        question = get_rag_question(question, docs)

        response = self.client.chat.completions.create(
            model=self.model_tag,
            messages=[
                {"role": "user", "content": question},
            ]
        )

        return response.choices[0].message.content


class BasicEngine(Engine):
    def __init__(self, api_key, model_tag, base_model_tag, docs_root_dir):
        super(BasicEngine, self).__init__(api_key, model_tag)
        self.docs_root_dir = docs_root_dir
        self.base_model_tag = base_model_tag
    
    def preprocess(self, question: str):
        cpp_code = get_cpp_code(self.client, question, self.base_model_tag)
        return question, {'cpp_code': cpp_code}
    
    def get_docs(self, question: str, data: dict):
        return get_docs(data['cpp_code'], self.docs_root_dir)


class WebPlatformEngine(Engine):
    def __init__(self, api_key, model_tag, docs_index_path):
        super(WebPlatformEngine, self).__init__(api_key, model_tag)
        # docs related
        with open(docs_index_path) as index_file:
            docs = json.load(index_file)
        self.docs_root_dir = os.path.join(docs['doc_folder'], docs['WebPlatform']['doc_folder'])
        self.docs = docs['WebPlatform']['docs']
        self.patterns = set([pattern.lower() for doc in self.docs for pattern in doc['patterns']])
    
    def preprocess(self, question):
        matched_patterns = [pattern for pattern in self.patterns if pattern in question.lower()]

        return question, {'matched_patterns': matched_patterns}

    def get_docs(self, question, data):
        def format_doc(doc):
            doc_path = os.path.join(self.docs_root_dir, f'{doc["id"]}.txt')
            with open(doc_path) as doc_file:
                doc_content = doc_file.read()
            doc_content = f"{doc['content']}\n{doc_content}"
            return doc_content

        matched_patterns = data['matched_patterns']
        matched_docs = []
        for doc in self.docs:
            for pattern in doc['patterns']:
                if pattern.lower() in matched_patterns:
                    matched_docs.append(doc)
                    break

        matched_docs = list(map(format_doc, matched_docs))
        return matched_docs
