import json
from concurrent.futures import ThreadPoolExecutor, as_completed
from pprint import pprint

from openai import OpenAI
from tqdm import tqdm


with open("./config.json", encoding="utf-8") as f:
    cfg = json.load(f)

with open(cfg.get("SUMMARY_PROMPT_PATH"), encoding="utf-8") as f:
    summary_prompt = f.read()

with open(cfg.get("FUNC_CLS_EXCTRACTION_PROMPT_PATH"), encoding="utf-8") as f:
    func_cls_extract_prompt = f.read()

client = OpenAI(api_key=cfg.get("OPENAI_API_KEY"))


def parallel_summarize(meta_dict, max_workers=5):
    items = list(meta_dict.values())

    with ThreadPoolExecutor(max_workers=max_workers) as executor:
        futures = {executor.submit(summarize_hierarchy, item): item for item in items}

        for _ in tqdm(as_completed(futures), total=len(futures)):
            pass  # progress bar only

    return meta_dict


def summarize_text(text: str) -> str:
    prompt = summary_prompt.replace("{{CONTENT}}", text)

    response = client.chat.completions.create(
        model=cfg.get("OPENAI_SUMMARISATION_MODEL"),
        messages=[{"role": "user", "content": prompt}]
    )

    return response.choices[0].message.content.strip()


def summarize_hierarchy(node):
    """
    node can be:
    - a leaf: library without subpages or a subpage
    - a library with subpages
    """

    # Leaf node
    # either supbages key does not exist or its value is an empty dict
    if not node.get("subpages"):
        node["description"] = summarize_text(node["content"])
        return node["description"]

    # Non-leaf: summarize subpages first
    subpages_summaries = []
    for subpage_title, subpage_meta in node.get("subpages").items():
        summary = summarize_hierarchy(subpage_meta)
        subpages_summaries.append(f"{subpage_title}: {summary}")

    combined = "\n".join(subpages_summaries)
    node["description"] = summarize_text(combined)

    return node["description"]


def save_json(d: dict, output_path: str):
    with open(output_path, "w", encoding="utf-8") as f:
        json.dump(d, f, indent=2, ensure_ascii=False)

    print(f'Saved output to {output_path}')


def build_library_content(library_meta):
    parts = []

    # README content
    if "content" in library_meta:
        parts.append(library_meta["content"])

    # Subpages
    if "subpages" in library_meta:
        for sub in library_meta["subpages"].values():
            if "content" in sub:
                parts.append(sub["content"])

    return "\n\n".join(parts)


def extract_api_items(library_meta: dict):
    full_content = build_library_content(library_meta)

    prompt = func_cls_extract_prompt.replace("{{CONTENT}}", full_content)

    response = client.chat.completions.create(
        model=cfg.get("OPENAI_FUNC_CLS_EXTRACTION_MODEL"),
        messages=[{"role": "user", "content": prompt}]
    )

    text = response.choices[0].message.content.strip()

    # Parse JSON safely
    try:
        data = json.loads(text)
        library_meta["notable_functions_and_classes"] = data
    except Exception:
        print("Model returned invalid JSON, raw output:")
        print(text)
        library_meta["notable_functions_and_classes"] = []


def parallel_func_cls_extraction(meta_dict, max_workers=5):
    items = list(meta_dict.values())

    with ThreadPoolExecutor(max_workers=max_workers) as executor:
        futures = {executor.submit(extract_api_items, item): item for item in items}

        for _ in tqdm(as_completed(futures), total=len(futures)):
            pass  # progress bar only

    return meta_dict


def main():
    print('\nReading basic meta file...')
    with open(cfg.get("BASIC_OUTPUT_PATH"), encoding="utf-8") as f:
       basic_meta = json.load(f)
    
    print('\nGenerating summaries...')
    parallel_summarize(basic_meta)
    save_json(basic_meta, cfg.get("BASIC_OUTPUT_PATH"))

    print('\nReading libraries meta file...')
    with open(cfg.get("LIBRARIES_OUTPUT_PATH"), encoding="utf-8") as f:
        libs_meta = json.load(f)

    print('\nGenerating summaries...')
    parallel_summarize(libs_meta)
    print('\nExtract notable functions and classes...')
    parallel_func_cls_extraction(libs_meta)
    save_json(libs_meta, cfg.get("LIBRARIES_OUTPUT_PATH"))


if __name__ == "__main__":
    main()
