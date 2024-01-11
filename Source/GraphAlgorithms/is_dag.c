#include <stdio.h>
#define MAX 100

int graph[MAX][MAX];
int visited[MAX];
int n;

int isCyclic(int i) {
    if (visited[i] == 2) {
        return 1;
    }
    visited[i] = 1;
    for (int j = 0; j < n; j++) {
        if (graph[i][j]) {
            if (visited[j] == 1) return 1;
            else if (visited[j] == 0 && isCyclic(j)) return 1;
        }
    }
    visited[i] = 2;
    return 0;
}

int isDAG() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            visited[j] = 0;
        }
        if (isCyclic(i)) {
            return 0;
        }
    }
    return 1;
}
