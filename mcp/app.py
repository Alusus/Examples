import json

from flask import Flask, request, jsonify

from copilot_server.agents import BasicExamplesRetriever

app = Flask(__name__)

global_id = 0
BASIC_VDB_PATH = 'vdb_basic.index'
BASIC_EXAMPLES_INDEX_PATH = 'index.json'

basic_examples_retriever = BasicExamplesRetriever(BASIC_VDB_PATH, BASIC_EXAMPLES_INDEX_PATH, debug=True)


with open("mcp/tools.json", "r", encoding="utf-8") as tools_file:
    TOOLS  = json.load(tools_file)


def initialize_mcp(id):
    return jsonify({
        "jsonrpc": "2.0",
        "id": id,
        "result": {
            "protocolVersion": "2025-06-18",
            "serverInfo": {"name": "alusus-docs", "version": "1.0.0"},
            "capabilities": {"tools": {"listChanged": True}, "resources": {}}
        }
    })


def retrieve_docs(query):
    docs = basic_examples_retriever.get_docs(query, k=10)
    
    return docs


def get_tool(tool_name):
    if tool_name == 'alusus_docs_retrieve':
        return retrieve_docs
    else:
        return None


@app.route('/mcp', methods=['POST'])
def mcp():
    payload = request.get_json(force=True)
    print(f'payload: {payload}')

    jsonrpc = payload.get("jsonrpc")
    method  = payload.get("method")
    id_     = payload.get("id")

    if method == 'initialize':
        return initialize_mcp(id_)
    
    if method == 'notifications/initialized':
        return jsonify({
            "jsonrpc": "2.0",
            "method": "notifications/initialized",
            "params": {}
        }, 200)

    if method == 'tools/list':
        return jsonify({
            "jsonrpc": "2.0",
            "id": id_,
            "result": {"tools": TOOLS}
        })
    
    if method == 'ping':
        return jsonify({
            "jsonrpc": "2.0",
            "id": id_,
            "result": {}
        })
    
    if method == 'tools/call':
        params = payload.get("params")
        if params is not None:
            tool_name = params.get("name")
            args = params.get("arguments")

            tool = get_tool(tool_name)

            if tool is None:
                return ("Bad Request, invalid tool name", 400)

            results = tool(**args)

            content = [{"type": "text", "text": item} for item in results]

            return jsonify({
                "jsonrpc": "2.0",
                "id": id_,
                "result": {
                    "content": content
                }
            })
        else:
            return ("Bad Request, include required parameters to invoke the tool", 400)


@app.route('/query', methods=['POST'])
def handle_query():
    data = request.json

    print(f'data:\n {data}')

    return jsonify({"response": "message reached the server!!"})


if __name__ == '__main__':
    app.run(debug=True)
