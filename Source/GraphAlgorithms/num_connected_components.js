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

// Usage:
let adjList = [[1], [0, 2], [1], [4], [3, 5], [4]];
visited = new Array(adjList.length).fill(false);
console.log("Number of connected components: ", numberOfConnectedComponents(adjList));
