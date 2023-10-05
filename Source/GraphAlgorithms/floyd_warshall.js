const V = 4;

function floydWarshall(graph) {
    let dist = [];
    for (let i = 0; i < V; i++) {
        dist[i] = [];
        for (let j = 0; j < V; j++) {
            dist[i][j] = graph[i][j];
        }
    }

    for (let k = 0; k < V; k++) {
        for (let i = 0; i < V; i++) {
            for (let j = 0; j < V; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    return dist;
}
