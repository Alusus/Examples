from abc import ABC, abstractmethod

from openai import OpenAI
from huggingface_hub import InferenceClient


class LLM(ABC):
    @abstractmethod
    def __call__(self, messages):
        pass


class OpenAILLM(LLM):
    def __init__(self, api_key: str, model_tag: str):
        self.client = OpenAI(api_key=api_key)
        self.model_tag = model_tag

    def __call__(self, messages):
        response = self.client.chat.completions.create(
            model=self.model_tag,
            messages=messages
        )

        return response.choices[0].message.content
    

class DeepSeekLLM(LLM):
    def __init__(self, api_key: str):
        self.client = InferenceClient(
            model="deepseek-ai/DeepSeek-R1-Distill-Qwen-32B",
            api_key=api_key
        )
    
    def __call__(self, messages):
        stream = self.client.chat.completions.create(
            messages=messages,
            max_tokens=1<<12,
            stream=True
        )

        response = ""
        for chunk in stream:
            response += chunk.choices[0].delta.content

        return response.strip()
