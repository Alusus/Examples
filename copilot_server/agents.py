import os
import re
import json
from concurrent.futures import ThreadPoolExecutor, as_completed


from sentence_transformers import SentenceTransformer
import faiss
import numpy as np

from copilot_server.LLMs import LLM



DEBUG_FILE = './agents.log'

def write_logs(logs: list[str]):
    with open(DEBUG_FILE, 'a', encoding='utf-8') as debug_file:
        for log in logs:
            debug_file.write(log)


if os.path.exists(DEBUG_FILE):
    os.remove(DEBUG_FILE)


class BasicExamplesRetriever:
    def __init__(self, vdb_path: str, index_path: str ='index.json', debug=False):
        self.vdb = faiss.read_index(vdb_path)
        self.embedding_model = SentenceTransformer('sentence-transformers/all-MiniLM-L6-v2')
        self.debug = debug

        self.index_path = index_path
        with open(index_path, encoding='utf-8') as index_file:
            self.index_meta = json.load(index_file)

    def get_docs(self, query, k=20):
        query_embedding = np.array(self.embedding_model.encode([query]))
        D, I = self.vdb.search(query_embedding, k)

        docs = [self.__format_sample(self.index_meta[idx]) for idx in I[0] if idx != -1]

        if self.debug:
            write_logs([
                "############Basic_Examples_Retriever_START\n",
                "\n*******\n".join(docs),
                "############Basic_Examples_Retriever_END\n",
            ])

        return docs

    def __format_sample(self, sample_meta):
        answer_file_path = os.path.join(
            os.path.dirname(self.index_path),
            sample_meta['enAlususAnswer']
        )
        with open(answer_file_path, encoding='utf-8') as answer_file:
            answer = answer_file.read()

        sample = f"""
        prompt: {sample_meta['enPrompt']}\n
        Answer:\n
        {answer}\n
        """

        return sample


class DocsExpert:
    """Score the docs and refine them to align better with the query"""
    def __init__(self, llm: LLM, debug=False):
        self.llm = llm
        self.debug = debug

    def compute_score(self, query, doc, cpp_code):
        prompt = self.__generate_prompt(query, doc, cpp_code)

        messages = [
            {
                "role": "user",
                "content": prompt
            }
        ]

        response = self.llm(messages)

        return self.__clean_and_extract_score(response)

    def refine(self, query, docs, cpp_code=None):
        if self.debug:
            write_logs([
                "############DOCS_EXPERT_START\n"
                f"Refining the docs for\nquery: {query}\n"
                "*******************\n"
            ])

        refined_docs = []

        with ThreadPoolExecutor() as executor:
            futures = [
                executor.submit(lambda doc: (self.compute_score(query, doc, cpp_code), doc), doc)
                for doc in docs
            ]

            for future in as_completed(futures):
                result = future.result()

                if not result:
                    continue

                score, doc = result

                if self.debug:
                    write_logs([
                        f"Doc (score: {score}):\n{doc}\n",
                        "---------------------\n"
                    ])

                if score < 0.5:
                    continue

                refined_docs.append(result)

        # Sort the docs in reverse order by score
        refined_docs.sort(reverse=True, key=lambda x: x[0])

        if self.debug:
            write_logs([
                f"Doc (score: {score}):\n{doc}\n---------------------\n"
                for score, doc in refined_docs 
            ])

        if self.debug:
            write_logs([
                "############DOCS_EXPERT_END\n"
            ])

        return refined_docs
 
    def __generate_prompt(self, query, doc, cpp_code=None):
        prompt = (
            "You are an expert in checking whether the documents are related to queries.\n"
            "The document should help answer the query to be relevant.\n"
            "Even if the document could help a little, it should get at least 0.5 score.\n"
            "To give a score, you should think about features needed in the code which will answer the question.\n"
            f"Query: {query}\n"
            f"Document: {doc}\n"
        )

        if cpp_code is not None:
            prompt += f"C++ Code:\n{cpp_code}\n"
            prompt += (
                "Use the provided C++ code as a way to help you check if a feature is needed.\n"
            )

        prompt += (
            "Score the relevance of the document to the query on a scale of 0 to 1, "
            "where 0 means not relevant at all and 1 means highly relevant.\n"
            "Provide the score between <score></score> tags.\n"
        )

        return prompt

    def __clean_and_extract_score(self, llm_output):
        """Remove <think> section and extract the score"""
        # Remove the <think> section if it exists
        cleaned_output = re.sub(r"<think>.*?</think>", "", llm_output, flags=re.DOTALL).strip()

        # Extract the score from <score> tags
        match = re.search(r"<score>(.*?)</score>", cleaned_output)
        if match:
            try:
                score = float(match.group(1))  # Convert the score to a float
            except ValueError:
                score = 0.0  # Default to 0 if the score is invalid
        else:
            score = 0.0  # Default to 0 if <score> tags are not found

        return score


class MainAgent:
    """Give an initial answer"""
    def __init__(self, llm: LLM, debug=False):
        self.llm = llm
        self.debug = debug
    
    def __call__(self, query, docs):
        prompt = self.__generate_prompt(query, docs)

        messages = [
            {
                "role": "user",
                "content": prompt
            }
        ]

        response = self.llm(messages)

        if self.debug:
            write_logs([
                "############MAIN_AGENT_START\n"
                f"Raw output:\n{response}\n"
                "############MAIN_AGENT_END\n"
            ])

        return self.__clean_and_extract_code(response)

    def __generate_prompt(self, query, docs):
        inline_docs = "\n******\n".join([f"doc (relevant score: {score}):\n{doc}" for score, doc in docs])
        prompt = (
            "You are an expert in programming.\n"
            "Even though you don't know a lot about Alusus programming language,"
            "you should be able to use the docs to help you answer the query.\n"
            "Documents:\n"
            f"{inline_docs}\n"
            f"Query: {query}\n"
            "Answer the query given the documents.\n"
            "You should take into account the relevance of the documents,"
            "where 0 means not relevant at all and 1 means highly relevant.\n"
            "Provide the code between <code></code> tags"
        )
        return prompt

    def __clean_and_extract_code(self, llm_output):
        """Remove <think> section and extract the code"""
        # Remove the <think> section if it exists
        cleaned_output = re.sub(r"<think>.*?</think>", "", llm_output, flags=re.DOTALL).strip()

        # Extract the code from <code> tags
        match = re.search(r"<code>(.*?)</code>", cleaned_output, re.DOTALL)
        if match:
            code = match.group(1)
        else:
            # Default to 0 if <code> tags are not found
            code = "Cannot answer this question"

        return code



class KeywordDocsRetriever:
    def __init__(self, alusus_features_mapper_path: str, docs_root_dir: str):
        self.alusus_features_mapper_path = alusus_features_mapper_path
        self.docs_root_dir = docs_root_dir

        with open(self.alusus_features_mapper_path) as f:
            self.alusus_features_mapper = json.load(f)

    def __is_feature_used(self, code, feature):
        if feature["regex"]:
            for pattern in feature["patterns"]:
                m = re.search(pattern, code, re.DOTALL | re.MULTILINE)
                if m is not None:
                    return True
        else:
            for pattern in feature["patterns"]:
                if code.find(pattern.lower()) != -1:
                    return True
        return False

    def __get_docs_ids(self, code):
        docs = []
        names = []

        lowercase_code = code.lower()
        for feature in self.alusus_features_mapper:
            if self.__is_feature_used(lowercase_code, feature):
                docs.extend(feature["ids"])
                names.append(feature["name"])

        return docs, names

    def get_docs(self, code):
        docs_ids, _ = self.__get_docs_ids(code)

        docs = []
        for id in docs_ids:
            doc_path = os.path.join(self.docs_root_dir, f"{id}.txt")
            with open(doc_path) as f:
                doc = f.read()
                docs.append(doc)

        return docs


class AlususExpert:
    def __init__(self, llm: LLM, debug=False):
        self.llm = llm
        self.debug = debug

    def __call__(self, code, docs):
        prompt = self.__generate_prompt(code, docs)

        messages=[
            {"role": "user", "content": prompt},
        ]

        response = self.llm(messages)

        if self.debug:
            write_logs([
                "############ALUSUS_EXPERT_START\n"
                f"Raw output:\n{response}\n"
                "############ALUSUS_EXPERT_END\n"
            ])


        return self.__clean_and_extract_code(response)

    def __generate_prompt(self, code, docs):
        inline_docs = "\n******\n".join(docs)
        prompt = (
            "You are an expert in Alusus programming language.\n"
            "You should be able to use the docs to help you check the code for syntax error and correct them.\n"
            "If something is not mentioned in the docs leave it as it is, do not make assumptions on your own!\n"
            "Documents:\n"
            f"{inline_docs}\n"
            "Code:\n"
            "```"
            f"{code}"
            "```"
            "Do not modify the approach or the code logic even if it's wrong! just correct any syntax errors.\n"
            "Please think about what you should correct between <think></think> tags.\n"
            "Please put the full code after correction between <code></code> tags.\n"
            "The code will be treated as a final answer so make sure it's complete!\n"
            "Some syntax errors may be repeated across the code, make sure that everything is corrected!\n"
            "If a feature is described in docs, any other way of using that feature must be wrong!\n"
        )
        return prompt

    def __clean_and_extract_code(self, llm_output):
        """Remove <think> section and extract the code"""
        # Remove the <think> section if it exists
        cleaned_output = re.sub(r"<think>.*?</think>", "", llm_output, flags=re.DOTALL).strip()

        write_logs([
            f"cleaned_output:\n{cleaned_output}\n----------\n"
        ])

        # Extract the code from <code> tags
        match = re.search(r"<code>(.*?)</code>", cleaned_output, re.DOTALL)
        if match:
            code = match.group(1)
        else:
            # Default to 0 if <code> tags are not found
            code = "Cannot answer this question"

        return code
