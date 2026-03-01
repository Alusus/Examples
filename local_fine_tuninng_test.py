import os
from transformers import AutoTokenizer, AutoModelForCausalLM, BitsAndBytesConfig
from peft import PeftModel
from utils import get_rag_question, get_cpp_code, get_docs
import torch

DOCS_ROOT_DIR = './doc'
OFFLOAD_DIR = './offload_dir'

def get_inference_engine(question, **kwargs):

    if 'enalusus' in question.lower():
        return LocalBasicEngine(kwargs['model_dir'], kwargs['docs_root_dir'])
    else:
        raise ValueError('Question cannot be answered by current supported engines.')

class LocalEngine:
    def __init__(self, model_dir):
        os.makedirs(OFFLOAD_DIR, exist_ok=True)

        self.tokenizer = AutoTokenizer.from_pretrained(model_dir)
        self.tokenizer.pad_token = self.tokenizer.eos_token

        bnb_config = BitsAndBytesConfig(
            load_in_4bit=True,
            bnb_4bit_quant_type="nf4",
            bnb_4bit_use_double_quant=True
        )

        base_model = AutoModelForCausalLM.from_pretrained(
            model_dir,
            quantization_config=bnb_config,
            device_map="auto",
            offload_folder=OFFLOAD_DIR,
            trust_remote_code=True
        )

        self.model = PeftModel.from_pretrained(base_model, model_dir, device_map="auto")

    def run(self, prompt: str, max_new_tokens=300, temperature=0.7):
        inputs = self.tokenizer(prompt, return_tensors="pt").to(self.model.device)
        outputs = self.model.generate(
            **inputs,
            max_new_tokens=max_new_tokens,
            temperature=temperature,
            do_sample=True
        )
        return self.tokenizer.decode(outputs[0], skip_special_tokens=True)

class LocalBasicEngine(LocalEngine):
    def __init__(self, model_dir, docs_root_dir):
        super().__init__(model_dir)
        self.docs_root_dir = docs_root_dir

    def preprocess(self, question: str):
        return question, None

    def get_docs(self, question, data, n_docs=5):
     
        docs = []
        for file_name in os.listdir(self.docs_root_dir):
            if file_name.endswith(".txt"):
                path = os.path.join(self.docs_root_dir, file_name)
                with open(path, encoding='utf-8') as f:
                    docs.append(f.read())
        return docs

    def run(self, question):
        question, data = self.preprocess(question)
        docs = self.get_docs(question, data)
        prompt = get_rag_question(question, docs)
        print(f"prompt :  {prompt}")
        return super().run(prompt)
if __name__ == "__main__":
    question = input("Enter your question: ")
    engine = get_inference_engine(question, model_dir="./deepseek-coder-v1", docs_root_dir="./doc")
    answer = engine.run(question)
    print(answer)
