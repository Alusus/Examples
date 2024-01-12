from openai import OpenAI

api_key = input("Enter api key: ")

question = input("Enter your question: ")

client = OpenAI(api_key=api_key)

response = client.chat.completions.create(
  model="ft:gpt-3.5-turbo-0613:alusus-software-ltd:v2-alusus:8fw5aYfd",
  messages=[
    {"role": "user", "content": question},
  ]
)
print(response.choices[0].message.content)
