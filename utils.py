import os
import json
import re
import jsonlines


def format(samples):
    formatted_samples = []

    for question, answer in samples:
        sample = {
            "messages": [
                {
                    "role": "user",
                    "content": question
                },
                {
                    "role": "assistant",
                    "content": answer
                }
            ]
        }

        formatted_samples.append(sample)
    
    return formatted_samples

def export(samples: list, filename: str):
    with jsonlines.open(f'{filename}.jsonl', mode='w') as writer:
        writer.write_all(samples)


def debug(*msgs):
    print(f'\n{"="*10}\n')
    for msg in msgs:
        print(msg)
    print(f'\n{"="*10}\n')

def format_question(q):
    q = q.replace("enAlusus", "c++")
    q += "\nPlease return only the code without any explanation and without comments inside the code"
    return [
        {
            "role": "user",
            "content": q,
        }
    ]


def get_cpp_code(client, question):
    question = "Using enAlusus, find the max number in an array."
    messages = format_question(question)
    response = client.chat.completions.create(
        model="gpt-3.5-turbo",
        messages=messages
    )
    return response.choices[0].message.content


def get_docs_ids(cpp_code):
    def exists(feature):
        if feature["regex"]:
            for pattern in feature["patterns"]:
                m = re.search(pattern, cpp_code, re.DOTALL | re.MULTILINE)
                if m is not None:
                    return True
        else:
            for pattern in feature["patterns"]:
                if cpp_code.find(pattern) != -1:
                    return True
        return False

    with open('docs2id.json') as f:
        c_features_mapper = json.load(f)
    
    docs = []
    names = []
    for feature in c_features_mapper:
        if exists(feature):
            docs.extend(feature["ids"])
            names.append(feature["name"])
    
    return docs, names


def get_docs(cpp_code, docs_root_dir: str):
    docs_ids, _ = get_docs_ids(cpp_code)

    docs = []
    for id in docs_ids:
        doc_path = os.path.join(docs_root_dir, f"{id}.txt")
        with open(doc_path) as f:
            doc = f.read()
            docs.append(doc)

    return docs


def get_rag_question(question, docs):
    docs = "\n".join(docs)
    return f"""
        please answer the following question with the help of the given context about enAlusus programming language.
        question: {question},
        context: {docs},
        answer:
    """
