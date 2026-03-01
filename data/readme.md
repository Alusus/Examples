# 📘 Documentation Scraper & Vector Database Builder

This folder contains the full pipeline for:

- Scraping documentation from the Alusus website and GitHub repos
- Extracting metadata for basic docs and libraries  
- Generating descriptions using OpenAI models
- Extracting notable functions and classes for each library using OpenAI models.
- Creating FAISS vector databases for retrieval  

The pipeline consists of three main scripts:

1. `scraper.py`  
2. `generate_description.py`  
3. `create_vdbs.py`  

Each script depends on a shared `config.json` file.

---

## 📦 Requirements

Before running the pipeline, ensure that:

- You have installed the full dependencies of the main repository  
- Additionally, **BeautifulSoup (bs4)** must be installed:

```bash
pip install beautifulsoup4
```

or

```bash
conda install conda-forge::beautifulsoup4
```

This is required for HTML parsing during documentation scraping.

---

## ⚙️ Configuration (`config.json`)

You must create a `config.json` file in this folder before running any script.

Use the following structure:

```json
{
    "WEBSITE_DOCS_URLS": [
        "https://alusus.org/Documents/lang-reference.en.html",
        "https://alusus.org/Documents/srt-reference.en.html"
    ],
    "BASIC_OUTPUT_PATH": "./basic_meta.json",
    "LIBRARIES_META_URL": "https://alusus.org/Releases/libraries.json",
    "LIBRARIES_OUTPUT_PATH": "./libs_meta.json",
    "GITHUB_TOKEN": "...",
    "OPENAI_API_KEY": "...",
    "OPENAI_SUMMARISATION_MODEL": "gpt-4o-mini",
    "SUMMARY_PROMPT_PATH": "./summary_gen_prompt.txt",
    "OPENAI_FUNC_CLS_EXTRACTION_MODEL": "gpt-5.2-chat-latest",
    "FUNC_CLS_EXCTRACTION_PROMPT_PATH": "./extract_func_cls_prompt.txt",
    "EMBEDDING_MODEL": "text-embedding-3-large",
    "VDBS_OUTPUT_DIR": "./vdbs/"
}
```

### 🔑 Required Keys

- **GITHUB_TOKEN**  
  Used to query GitHub’s API for default branch names without hitting rate limits.

- **OPENAI_API_KEY**  
  Required for generating summaries, extracting function/class metadata, and creating embeddings.

---

## 🔐 Creating a GitHub Token

GitHub requires authentication for higher API rate limits.  
To create a token:

1. Visit:  
   `https://github.com/settings/tokens?type=beta`
2. Click **“Generate new token”**
3. Choose **Fine-grained token**
4. Give it a name (e.g., `alusus-docs-scraper`)
5. Under **Repository access**, choose:
   - **Public repositories** → Read-only
6. Generate the token
7. Copy it and paste it into your `config.json` under `"GITHUB_TOKEN"`

This token allows the scraper to query repository metadata without hitting the unauthenticated rate limit.

---

## ▶️ Running the Pipeline

Before running the scripts, activate the full environment of the main repository.

Then navigate to this folder and run the scripts in order:

### 1. Scrape documentation and library metadata

```bash
python scraper.py
```

This generates:

- `basic_meta.json`  
- `libs_meta.json`  

---

### 2. Generate descriptions using OpenAI

```bash
python generate_description.py
```

This enriches the metadata with:

- Summaries  
- Function/class descriptions  
- Cleaned content  

---

### 3. Create vector databases (FAISS)

```bash
python create_vdbs.py
```

This produces:

- A vector DB for basic docs  
- A vector DB for library‑level descriptions  
- A vector DB for each library with subpages  

All stored under the directory defined in:

```json
"VDBS_OUTPUT_DIR": "./vdbs/"
```

---

## 📁 Output Structure

After running all scripts, you will have:

```
basic_meta.json
libs_meta.json
vdbs/
    basic.index
    basic.mapping.json
    libraries.index
    libraries.mapping.json
    <libname>.subpages.index
    <libname>.subpages.mapping.json
```
