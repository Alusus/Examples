function dfs(node, visited, G) {
    visited[node] = true;
    console.log(node);

    for(let i = 0; i < G[node].length; i++) {
        if(!visited[G[node][i]]) {
            dfs(G[node][i], visited, G);
        }
    }
}
