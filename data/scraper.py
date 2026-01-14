import re
import json
import requests
from tqdm import tqdm
from concurrent.futures import ThreadPoolExecutor, as_completed

from bs4.element import Tag
from bs4 import BeautifulSoup


with open("./config.json", encoding="utf-8") as f:
    cfg = json.load(f)


def download_json(url: str) -> dict:
    if not url:
        return None

    response = requests.get(url)
    response.raise_for_status()
    return response.json()


def fetch_text(url: str) -> str:
    try:
        r = requests.get(url)
        if r.status_code == 200:
            return r.text
    except Exception:
        pass
    return None


def get_default_branch(repo_url: str) -> str:
    # Extract owner and repo from URL
    match = re.search(r"github\.com/([^/]+)/([^/]+)", repo_url)
    if not match:
        raise ValueError("Invalid GitHub repository URL")

    owner, repo = match.group(1), match.group(2)

    api_url = f"https://api.github.com/repos/{owner}/{repo}"

    headers = {"Accept": "application/vnd.github+json"}
    token = cfg.get("GITHUB_TOKEN")
    if token:
        headers["Authorization"] = f"Bearer {token}"
    
    response = requests.get(api_url, headers=headers)

    if response.status_code != 200:
        raise RuntimeError(f"GitHub API error: {response.status_code} - {response.text}")

    data = response.json()
    return data.get("default_branch", "main")  # Fallback to `main`


def path_to_raw_url(path: str, repo_url: str) -> str:
    default_branch = get_default_branch(repo_url)
    view_url = repo_url.rstrip("/") + f"/blob/{default_branch}/" + path.lstrip("/")
    raw_url = view_url.replace("github.com", "raw.githubusercontent.com").replace("/blob/", "/")

    return raw_url


def get_readme_url(repo_url: str) -> str:
    return path_to_raw_url("/readme.md", repo_url)


def extract_subpage_links(md_text: str) -> list[tuple[str, str]]:
    pattern = r"\[\[([^\]]+)\]\]\(([^)]+)\)"
    matches = re.findall(pattern, md_text)
    return matches


def process_library(item: dict) -> dict:
    lib_name = item.get("name")
    repo_url = item.get("url")

    lib_meta = {
        "name": lib_name,
        "repo_url": repo_url,
        "description": "",
        "content": None,
        "subpages": {}
    }

    if not repo_url:
        return lib_meta

    # Find README
    readme_url = get_readme_url(repo_url)
    if not readme_url:
        return lib_meta

    readme_text = fetch_text(readme_url)
    lib_meta["content"] = readme_text

    if not readme_text:
        return lib_meta

    # Extract subpages
    subpages = extract_subpage_links(readme_text)

    for link_text, sub_url in subpages:
        if sub_url == "readme.ar.md":
            continue

        # Convert relative links to absolute
        if not sub_url.startswith("http"):
            sub_url = path_to_raw_url(sub_url, repo_url)

        content = fetch_text(sub_url)
        if content:
            lib_meta["subpages"][link_text] = {
                "url": sub_url,
                "description": "",
                "content": content,
            }

    return lib_meta


def is_inside(el, container):
    parent = el
    while parent:
        if parent is container:
            return True
        parent = parent.parent
    return False


def process_webpage_doc(url: str) -> dict:
    html = fetch_text(url)
    soup = BeautifulSoup(html, "html.parser")

    sidebar = soup.select_one(".col-md-3 .card.side-sticky .card-content")
    if not sidebar:
        raise ValueError("Sidebar not found in the HTML page")

    # Extract ordered list of (id, title) from sidebar
    nav_items = []
    for a in sidebar.select("a[href^='#']"):
        href = a.get("href")
        if href and href.startswith("#"):
            section_id = href[1:]  # remove '#'
            title = a.get_text(strip=True)
            nav_items.append((section_id, title))

    if not nav_items:
        return {}

    content_container = soup.select_one(".col-md-9") or soup.body

    sections = {}
    for i, (section_id, title) in enumerate(nav_items):
        start_el = soup.find(id=section_id)
        if not start_el:
            continue

        # Determine the next section ID (if any)
        next_id = nav_items[i + 1][0] if i + 1 < len(nav_items) else None

        content_parts = []
        started = False

        # Walk the DOM in document order, including text nodes
        for node in start_el.next_elements:
            # Skip the starting element itself
            if not started:
                started = True
                continue

            # If we left the content container, stop
            if isinstance(node, Tag) and not is_inside(node, content_container):
                break

            # Stop when we reach the next section anchor
            if isinstance(node, Tag) and next_id and node.get("id") == next_id:
                break

            # if this node contains the next node, skip
            if isinstance(node, Tag) and next_id:
                inner_next = node.find(id=next_id)
                if inner_next is not None:
                    continue

            content_parts.append(str(node))

        sections[title] = {
            "content": "".join(content_parts),
            "description": "", # to be filled later using AI models
        }

    return sections


def parallel_process_libraries(max_workers=5) -> dict:
    print("\nDownloading libraries info...")
    libraries = download_json(cfg.get("LIBRARIES_META_URL"))
    print("\nDone Downloading libraries info...")

    print("\nExtracting libraries meta information...")
    def process(lib_info):
        lib_name = lib_info.get("name")
        data = process_library(lib_info)
        return lib_name, data

    libs_meta = {}

    with ThreadPoolExecutor(max_workers=max_workers) as executor:
        futures = [executor.submit(process, lib_info) for lib_info in libraries]

        for future in tqdm(as_completed(futures), total=len(futures)):
            lib_name, data = future.result()
            libs_meta[lib_name] = data   # safe: done in main thread only

    return libs_meta


def process_basic() -> dict:
    basic_meta = {}

    print("\nExtracting website docs...")
    for doc_url in tqdm(cfg.get("WEBSITE_DOCS_URLS")):
        doc_sections = process_webpage_doc(doc_url)
        basic_meta.update(doc_sections)

    return basic_meta


def save_json(d: dict, output_path: str):
    with open(output_path, "w", encoding="utf-8") as f:
        json.dump(d, f, indent=2, ensure_ascii=False)

    print(f'Saved output to {output_path}')


def main():
    libs_meta = parallel_process_libraries()
    save_json(libs_meta, cfg.get("LIBRARIES_OUTPUT_PATH"))

    basic_meta = process_basic()
    save_json(basic_meta, cfg.get("BASIC_OUTPUT_PATH"))


if __name__ == "__main__":
    main()
