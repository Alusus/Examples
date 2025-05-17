import os
import json

from sentence_transformers import SentenceTransformer
import faiss
import numpy as np


def get_embeddings(samples, model):    
    embeddings = np.array(model.encode(samples))

    return embeddings


def get_samples(examples_index_path, lib_name):
    with open(examples_index_path) as f:
        examples_index = json.load(f)

    examples_meta = examples_index[lib_name]

    samples = [doc["content"] for doc in examples_meta["docs"]]

    return samples


def get_basic_samples(examples_index_path):
    with open(examples_index_path, encoding='utf-8') as f:
        examples_index = json.load(f)
    
    samples = [example['enPrompt'] for example in examples_index]

    return samples


def create_index(embeddings, save_path, n_clusters=20):
    # Create an IVF index
    dimension = embeddings.shape[1]
    quantizer = faiss.IndexFlatL2(dimension)
    index = faiss.IndexIVFFlat(quantizer, dimension, n_clusters, faiss.METRIC_L2)

    # Train the index
    index.train(np.array(embeddings))

    # Add embeddings to the index
    index.add(np.array(embeddings))

    # Save the index to disk
    faiss.write_index(index, save_path)


def get_results(index, model, queries, k=5):
    # Query the index
    query_embedding = np.array(model.encode(queries))

    D, I = index.search(query_embedding, k)
    return D, I


def main(lib_name: str, n_clusters: int, k: int, query: str):
    model = SentenceTransformer('sentence-transformers/all-MiniLM-L6-v2')

    print("[INFO] Getting samples...")
    if lib_name == 'basic':
        samples = get_basic_samples("index.json")
    else:
        #samples = get_samples("libs_index.json", "WebPlatform")
        samples = get_samples("libs_index.json", lib_name)

    print("[INFO] Generate embeddings...")
    embeddings = []
    for i in range(0, len(samples), 16):
        e = get_embeddings(samples[i:i+16], model)
        embeddings.append(e)
    
    embeddings = np.concatenate(embeddings, axis=0)
    print(embeddings.shape)

    print("[INFO] Creating Index...")
    # create_index(embeddings, "ivf_index_file.index")
    index_filename = f"vdb_{lib_name}.index"
    create_index(embeddings, index_filename, n_clusters)

    # Load the index from disk
    print("[INFO] Reading Index...")
    index = faiss.read_index(index_filename)

    print("[INFO] Inference...")
    D, I = get_results(index, model, [query], k)

    print(f'D: {D}')
    print('-'*30)
    print(f'I: {I}')



if __name__ == "__main__":
    main(
        'basic',
        n_clusters=10,
        k=20,
        query="write a code to check whether an array contains a number that's a multiple of 3 or not"
    )
