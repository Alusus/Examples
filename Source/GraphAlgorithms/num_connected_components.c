#include <stdio.h>

#define MAX 100

int visited[MAX];
int matrix[MAX][MAX];

void DFS(int v, int n) {
    visited[v] = 1;
    for(int i = 0; i < n; i++) {
        if(matrix[v][i] == 1 && visited[i] == 0) {
            DFS(i, n);
        }
    }
}

int numberOfConnectedComponents(int n) {
    int components = 0;
    for(int i = 0; i < n; i++) {
        if(visited[i] == 0) {
            DFS(i, n);
            components++;
        }
    }
    return components;
}

int main() {
    int n;
    printf("Enter the number of vertices: ");
    scanf("%d", &n);

    printf("Enter the adjacency matrix:\n");
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }

    printf("Number of connected components: %d\n", numberOfConnectedComponents(n));

    return 0;
}
