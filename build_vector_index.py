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


def create_index(embeddings, save_path):
    # Create an IVF index
    dimension = embeddings.shape[1]
    nlist = 20  # Number of clusters
    quantizer = faiss.IndexFlatL2(dimension)
    index = faiss.IndexIVFFlat(quantizer, dimension, nlist, faiss.METRIC_L2)

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


def main():
    model = SentenceTransformer('sentence-transformers/all-MiniLM-L6-v2')

    print("[INFO] Getting samples...")
    samples = get_samples("libs_index.json", "WebPlatform")
    print("[INFO] Generate embeddings...")
    embeddings = []
    for i in range(0, len(samples), 16):
        e = get_embeddings(samples[i:i+16], model)
        embeddings.append(e)
    
    embeddings = np.concatenate(embeddings, axis=0)
    print(embeddings.shape)

    print("[INFO] Creating Index...")
    create_index(embeddings, "ivf_index_file.index")

    # Load the index from disk
    print("[INFO] Reading Index...")
    index = faiss.read_index("ivf_index_file.index")

    print("[INFO] Inference...")
    D, I = get_results(index, model, ["Submit button"])

    print(f'D: {D}')
    print('-'*30)
    print(f'I: {I}')


if __name__ == "__main__":
    main()
