let graph = [];
let visited = [];
let n;

function isCyclic(i) {
    if (visited[i] === 2) {
        return true;
    }
    visited[i] = 1;
    for (let j = 0; j < graph[i].length; j++) {
        let node = graph[i][j];
        if (visited[node] === 1) return true;
        else if (visited[node] === 0 && isCyclic(node)) return true;
    }
    visited[i] = 2;
    return false;
}

function isDAG() {
    for (let i = 0; i < n; i++) {
        for (let j = 0; j < n; j++) {
            visited[j] = 0;
        }
        if (isCyclic(i)) {
            return false;
        }
    }
    return true;
}

function main() {
    n = 4;
    graph = [[1], [2], [3], []];
    console.log("Is DAG: " + (isDAG() ? "Yes" : "No"));
}

main();
