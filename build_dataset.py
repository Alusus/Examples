import json
import jsonlines
import random
import os

# TRAIN_RATIO = 0.9
LANGUAGES = [('c', 'en'), ('js', 'en'), ('enAlusus', 'en')]#, ('arAlusus', 'ar')]
TEST_QUESTIONS = [
    # ArrayRelatedOperations
    'check_any',
    'min_in_array',
    'reduce',
    'rfind',
    'std',
    # FileHandling
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


def debug(*msgs):
    print(f'\n{"="*10}\n')
    for msg in msgs:
        print(msg)
    print(f'\n{"="*10}\n')


data = json.load(open('index.json', encoding='utf-8'))
random.shuffle(data)


def generate(meta_data: dict, reverse=False):
    samples = []

    for code_lang, prompt_lang in LANGUAGES:
        if 'Alusus' not in code_lang:
            continue

        file_path = meta_data[f'{code_lang}Answer']
        with open(file_path, encoding='utf-8') as code_file:
            code = code_file.read()
        
        prompt = meta_data[f'{prompt_lang}Prompt']
        if reverse:
            if prompt_lang == 'ar':
                question = f'ما الذي يقوم به هذا الكود\n{code}'
                answer = f'{prompt}, باستعمال {code_lang}'
            else:
                question = f'What does this code do?\n{code}'
                answer = f'{prompt}, Using {code_lang}'
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


def generate_doc_data(meta_data):
    answer_file_path = f'doc/{meta_data["question_id"]}.txt'
    with open(answer_file_path) as answer_file:
        answer = answer_file.read()
    
    if meta_data["answer_prefix"]:
        answer = meta_data["answer_prefix"] + " " + answer
    
    question = meta_data["question"]

    return (question, answer)


def generate_fine_grained_data(meta_data):
    with open(meta_data["answer_path"], encoding='utf-8') as f:
        answer = f.read()
    
    question = meta_data["question"]

    return (question, answer)


def build(data, doc_data_path=None, fine_grained_data_path=None):
    train_samples = []
    test_samples = []

    for meta_data in data:
        filename = meta_data[f'cAnswer'].split('/')[-1]
        algo_type = filename.split('.')[0]
        if algo_type in TEST_QUESTIONS:
            test_samples.extend(generate(meta_data))
            test_samples.extend(generate(meta_data, reverse=True))
            # test_samples.extend(generate_translated(meta_data))
        else:
            train_samples.extend(generate(meta_data))
            train_samples.extend(generate(meta_data, reverse=True))
            # train_samples.extend(generate_translated(meta_data))

    if doc_data_path is not None:
        doc_data = json.load(open(doc_data_path, encoding='utf-8'))
        random.shuffle(doc_data)
        for meta_data in doc_data:
            train_samples.append(generate_doc_data(meta_data))

    if fine_grained_data_path is not None:
        index_file_path = os.path.join(fine_grained_data_path, 'index.json')
        fine_grained_data = json.load(open(index_file_path, encoding='utf-8'))
        random.shuffle(fine_grained_data)
        for meta_data in fine_grained_data:
            meta_data["answer_path"] = os.path.join(fine_grained_data_path, meta_data["answer_path"])
            train_samples.append(generate_fine_grained_data(meta_data))

    random.shuffle(train_samples)

    return train_samples, test_samples

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


train_samples, test_samples = build(
    data,
    #doc_data_path='doc_index.json',
    doc_data_path=None,
    fine_grained_data_path='fine_grained/')
train_samples = format(train_samples)
test_samples = format(test_samples)

debug(f'train size: {len(train_samples)}')
debug('train examples:', train_samples[:3])
debug(f'test size: {len(test_samples)}')
debug('test examples:', test_samples[:3])

def export(samples: list, filename: str):
    with jsonlines.open(f'{filename}.jsonl', mode='w') as writer:
        writer.write_all(samples)

export(train_samples, 'train_set')
export(test_samples, 'test_set')
