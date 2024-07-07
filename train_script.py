from openai import OpenAI

api_key = input("Enter api key: ")
version = input("Enter the model version: ")

client = OpenAI(api_key=api_key)

train_file = client.files.create(
  file=open("rag_train_set.jsonl", "rb"),
  purpose="fine-tune"
)

test_file = client.files.create(
  file=open("rag_test_set.jsonl", "rb"),
  purpose="fine-tune"
)

client.fine_tuning.jobs.create(
  training_file=train_file.id,
  validation_file=test_file.id,
  suffix=f"rag_v{version}_Alusus",
  model="gpt-3.5-turbo",
  # hyperparameters={
  #   "n_epochs":3
  # }
)

