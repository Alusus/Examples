from utils import get_cpp_code, get_docs, get_rag_question

from openai import OpenAI


api_key = input("Enter api key: ")
model = input("Enter model tag: ")
question = input("Enter your question: ")
client = OpenAI(api_key=api_key)

DOCS_DIR = "./doc"

cpp_code = get_cpp_code(client, question)

docs = get_docs(cpp_code, DOCS_DIR)

rag_question = get_rag_question(question, docs)

response = client.chat.completions.create(
        model=model,
        messages=[
            {
                "role": "user",
                "content": rag_question,
            }
        ]
    )

print(response.choices[0].message.content)
