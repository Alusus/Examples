import json
import jsonlines
import random

TRAIN_RATIO = 0.9
LANGUAGES = [('c', 'en'), ('js', 'en'), ('enAlusus', 'en'), ('arAlusus', 'ar')]


def debug(*msgs):
    print(f'\n{"="*10}\n')
    for msg in msgs:
        print(msg)
    print(f'\n{"="*10}\n')


data = json.load(open('index.json', encoding='utf-8'))
random.shuffle(data)

train_size = int(len(data) * TRAIN_RATIO)
test_size = len(data) - train_size

train_data = data[:train_size]
test_data = data[train_size:]


def generate(meta_data: dict, reverse=False):
    samples = []

    for code_lang, prompt_lang in LANGUAGES:
        with open(meta_data[f'{code_lang}Answer'], encoding='utf-8') as code_file:
            code = code_file.read()
        
        prompt = meta_data[f'{prompt_lang}Prompt']
        if reverse:
            if prompt_lang == 'ar':
                question = f'ما الذي يقوم به هذا الكود\n{code}'
            else:
                question = f'What does this code do?\n{code}'
            answer = prompt
        else:
            if prompt_lang == 'ar':
                question = f'باستعمال {code_lang}, {prompt}'
            else:
                question = f'Using {code_lang}, {prompt}'
            answer = code
        samples.append((question, answer))
    
    return samples


def generate_translated(meta_data: dict):
    samples = []

    for from_lang, _ in LANGUAGES:
        for to_lang, _ in LANGUAGES:
            if from_lang == to_lang or 'Alusus' not in (from_lang+to_lang):
                continue

            with open(meta_data[f'{from_lang}Answer'], encoding='utf-8') as from_code_file:
                from_code = from_code_file.read()
            with open(meta_data[f'{to_lang}Answer'], encoding='utf-8') as to_code_file:
                to_code = to_code_file.read()

            question = f'translate the following code from "{from_lang}" to "{to_lang}":\n{from_code}'
            answer = to_code

            samples.append((question, answer))

    return samples


def build(data):

    samples = []
    for meta_data in data:
        samples.extend(generate(meta_data))
        samples.extend(generate(meta_data, reverse=True))
        samples.extend(generate_translated(meta_data))

    random.shuffle(samples)

    return samples

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


train_samples = format(build(train_data))
test_samples = format(build(test_data))

debug(f'train size: {len(train_samples)}')
debug('train examples:', train_samples[:3])
debug(f'test size: {len(test_samples)}')
debug('test examples:', test_samples[:3])

def export(samples: list, filename: str):
    with jsonlines.open(f'{filename}.jsonl', mode='w') as writer:
        writer.write_all(samples)

export(train_samples, 'train_set')
export(test_samples, 'test_set')
