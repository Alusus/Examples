let visited = [];

function DFS(v, parent, adjList) {
    visited[v] = true;
    for(let i = 0; i < adjList[v].length; i++) {
        if(!visited[adjList[v][i]]) {
            if(DFS(adjList[v][i], v, adjList)) {
                return true;
            }
        } else if(adjList[v][i] != parent) {
            return true;
        }
    }
    return false;
}

function isTree(adjList) {
    visited = new Array(adjList.length).fill(false);
    if(DFS(0, -1, adjList)) {
        return false;
    }
    for(let i = 0; i < adjList.length; i++) {
        if(!visited[i]) {
            return false;
        }
    }
    return true;
}

// Usage:
let adjList = [[1], [0, 2, 3], [1], [1, 4, 5], [3], [3]];
console.log("Is the graph a tree? ", isTree(adjList));
