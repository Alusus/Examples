#include <stdio.h>

#define MAX 100

int visited[MAX];
int matrix[MAX][MAX];

int DFS(int v, int parent, int n) {
    visited[v] = 1;
    for(int i = 0; i < n; i++) {
        if(matrix[v][i] == 1) {
            if(!visited[i]) {
                if(DFS(i, v, n)) {
                    return 1;
                }
            } else if(i != parent) {
                return 1;
            }
        }
    }
    return 0;
}

int isTree(int n) {
    if(DFS(0, -1, n)) {
        return 0;
    }
    for(int i = 0; i < n; i++) {
        if(!visited[i]) {
            return 0;
        }
    }
    return 1;
}
