let visited = [];

function DFS(v, adjList) {
    visited[v] = true;
    for(let i = 0; i < adjList[v].length; i++) {
        if(!visited[adjList[v][i]]) {
            DFS(adjList[v][i], adjList);
        }
    }
}

function numberOfConnectedComponents(adjList) {
    let components = 0;
    for(let i = 0; i < adjList.length; i++) {
        if(!visited[i]) {
            DFS(i, adjList);
            components++;
        }
    }
    return components;
}
