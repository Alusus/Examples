void dfs(int node, int visited[], int G[][N]) {
    visited[node] = 1;
    printf("%d ", node);

    for(int i = 0; i < N; i++) {
        if(G[node][i] == 1 && visited[i] == 0) {
            dfs(i, visited, G);
        }
    }
}
