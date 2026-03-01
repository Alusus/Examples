import os
import json
from pprint import pprint
from concurrent.futures import ThreadPoolExecutor, as_completed

import faiss
import numpy as np
from tqdm import tqdm
from openai import OpenAI


with open("./config.json", encoding="utf-8") as f:
    cfg = json.load(f)

client = OpenAI(api_key=cfg.get("OPENAI_API_KEY"))


def embed_texts(texts):
    response = client.embeddings.create(
        model=cfg.get("EMBEDDING_MODEL"),
        input=texts
    )
    return [item.embedding for item in response.data]


def build_faiss_index(vectors, output_path):
    dim = len(vectors[0])
    index = faiss.IndexFlatL2(dim)

    vectors_np = np.array(vectors).astype("float32")
    index.add(vectors_np)

    faiss.write_index(index, output_path)


def build_basic_vector_db(basic_meta, output_dir):
    descriptions = []
    mapping = []  # list of keys

    for key, item in tqdm(basic_meta.items()):
        desc = item.get("description")
        if not desc:
            continue

        descriptions.append(desc)
        mapping.append(key)

    if not descriptions:
        return

    vectors = embed_texts(descriptions)

    os.makedirs(output_dir, exist_ok=True)
    build_faiss_index(vectors, os.path.join(output_dir, "basic.index"))

    with open(os.path.join(output_dir, "basic.mapping.json"), "w", encoding="utf-8") as f:
        json.dump(mapping, f, ensure_ascii=False, indent=2)


def build_library_highlevel_db(libs_meta, output_dir):
    descriptions = []
    mapping = []

    for lib_name, lib in tqdm(libs_meta.items()):
        desc = lib.get("description")
        if not desc:
            continue

        descriptions.append(desc)
        mapping.append(lib_name)

    if not descriptions:
        return

    vectors = embed_texts(descriptions)

    os.makedirs(output_dir, exist_ok=True)
    build_faiss_index(vectors, os.path.join(output_dir, "libraries.index"))

    with open(os.path.join(output_dir, "libraries.mapping.json"), "w", encoding="utf-8") as f:
        json.dump(mapping, f, ensure_ascii=False, indent=2)


def build_library_subpage_dbs(libs_meta, output_dir):
    os.makedirs(output_dir, exist_ok=True)

    for lib_name, lib in tqdm(libs_meta.items()):
        subpages = lib.get("subpages", {})
        if not subpages:
            continue  # skip libraries without subpages

        descriptions = []
        mapping = []  # (library_name, subpage_name)

        for page_name, page in subpages.items():
            desc = page.get("description")
            if desc:
                descriptions.append(desc)
                mapping.append((lib_name, page_name))

        if not descriptions:
            continue

        vectors = embed_texts(descriptions)

        index_path = os.path.join(output_dir, f"{lib_name}.subpages.index")
        build_faiss_index(vectors, index_path)

        with open(os.path.join(output_dir, f"{lib_name}.subpages.mapping.json"), "w", encoding="utf-8") as f:
            json.dump(mapping, f, ensure_ascii=False, indent=2)


def retrieve_basic(query, basic_meta, output_dir, k=5):
    index = faiss.read_index(os.path.join(output_dir, "basic.index"))

    with open(os.path.join(output_dir, "basic.mapping.json"), encoding="utf-8") as f:
        mapping = json.load(f)

    vector = embed_texts([query])[0]
    vector_np = np.array([vector]).astype("float32")

    distances, indices = index.search(vector_np, k)

    results = []
    for idx in indices[0]:
        key = mapping[idx]
        results.append((key, basic_meta[key]))

    return results


def retrieve_library_highlevel(query, libs_meta, output_dir, k=5):
    index = faiss.read_index(os.path.join(output_dir, "libraries.index"))

    with open(os.path.join(output_dir, "libraries.mapping.json"), encoding="utf-8") as f:
        mapping = json.load(f)

    vector = embed_texts([query])[0]
    vector_np = np.array([vector]).astype("float32")

    distances, indices = index.search(vector_np, k)

    results = []
    for idx in indices[0]:
        lib_name = mapping[idx]
        results.append(libs_meta[lib_name])

    return results


def retrieve_library_subpages(query, lib_name, libs_meta, output_dir, k=5):
    index = faiss.read_index(os.path.join(output_dir, f"{lib_name}.subpages.index"))

    with open(os.path.join(output_dir, f"{lib_name}.subpages.mapping.json"), encoding="utf-8") as f:
        mapping = json.load(f)

    vector = embed_texts([query])[0]
    vector_np = np.array([vector]).astype("float32")

    distances, indices = index.search(vector_np, k)

    results = []
    for idx in indices[0]:
        lib, page = mapping[idx]
        results.append((page, libs_meta[lib]["subpages"][page]))

    return results


def build_all_vector_dbs(basic_meta: dict, libs_meta: dict):
    vdbs_output_dir = cfg.get("VDBS_OUTPUT_DIR")

    print("Building basic vector DB...")
    build_basic_vector_db(basic_meta, vdbs_output_dir)
    print("Building library high-level vector DB...")
    build_library_highlevel_db(libs_meta, vdbs_output_dir)
    print("Building per-library subpage vector DBs...")
    build_library_subpage_dbs(libs_meta, vdbs_output_dir)

    print("Build complete.")


if __name__ == "__main__":
    import logging

    LOG_PATH = "vector_db_test.log"

    logging.basicConfig(
        filename=LOG_PATH,
        filemode="w",
        level=logging.INFO,
        format="%(asctime)s - %(levelname)s - %(message)s"
    )

    with open(cfg.get("BASIC_OUTPUT_PATH"), encoding="utf-8") as f:
        basic_meta = json.load(f)

    with open(cfg.get("LIBRARIES_OUTPUT_PATH"), encoding="utf-8") as f:
        libs_meta = json.load(f)

    build_all_vector_dbs(basic_meta, libs_meta)

    # ---------------------------------------------------------
    # TESTS
    # ---------------------------------------------------------
    logging.info("=== Starting Vector DB Retrieval Tests ===")

    # -----------------------------
    # Test 1: Basic Docs Retrieval
    # -----------------------------
    query1 = "function"
    results1 = retrieve_basic(query1, basic_meta, cfg.get("VDBS_OUTPUT_DIR"), k=3)

    logging.info("=== Test 1: Basic Docs Retrieval ===")
    logging.info(f"Query: {query1}")

    for i, (key, item) in enumerate(results1, 1):
        logging.info(f"\nResult {i}:")
        logging.info(f"Key: {key}")
        logging.info(f"Description: {item.get('description')}")

    # -----------------------------
    # Test 2: Library High-Level Retrieval
    # -----------------------------
    query2 = "How to handle Google authentication in Alusus?"
    results2 = retrieve_library_highlevel(query2, libs_meta, cfg.get("VDBS_OUTPUT_DIR"), k=3)

    logging.info("\n=== Test 2: Library High-Level Retrieval ===")
    logging.info(f"Query: {query2}")

    for i, item in enumerate(results2, 1):
        logging.info(f"\nResult {i}:")
        logging.info(f"Library: {item.get('name')}")
        logging.info(f"Description: {item.get('description')}")

    # -----------------------------
    # Test 3: Subpage Retrieval for a Specific Library
    # -----------------------------
    TEST_LIBRARY = "WebPlatform"

    if TEST_LIBRARY in libs_meta and libs_meta[TEST_LIBRARY].get("subpages"):
        query3 = "How to create a webpage with a picture in Alusus?"
        results3 = retrieve_library_subpages(query3, TEST_LIBRARY, libs_meta, cfg.get("VDBS_OUTPUT_DIR"), k=3)

        logging.info(f"\n=== Test 3: Subpage Retrieval for '{TEST_LIBRARY}' ===")
        logging.info(f"Query: {query3}")

        for i, (page, item) in enumerate(results3, 1):
            logging.info(f"\nResult {i}:")
            logging.info(f"Subpage: {page}")
            logging.info(f"Description: {item.get('description')}")
    else:
        logging.info(f"\nSkipping Test 3: Library '{TEST_LIBRARY}' has no subpages.")

    logging.info("=== Vector DB Retrieval Tests Completed ===")
