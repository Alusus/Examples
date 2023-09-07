
#include <stdio.h>

#define N 4

void printSolution(int board[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int isSafe(int board[N][N], int row, int col) {
    int i, j;

    // Check the row on the left side
    for (i = 0; i < col; i++) {
        if (board[row][i]) {
            return 0;
        }
    }

    // Check the upper diagonal on the left side
    for (i = row, j = col; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j]) {
            return 0;
        }
    }

    // Check the lower diagonal on the left side
    for (i = row, j = col; j >= 0 && i < N; i++, j--) {
        if (board[i][j]) {
            return 0;
        }
    }

    return 1;
}

int solveNQueensUtil(int board[N][N], int col) {
    if (col == N) {
        printSolution(board);
        return 1;
    }

    int res = 0;
    for (int i = 0; i < N; i++) {
        if (isSafe(board, i, col)) {
            board[i][col] = 1;
            res = solveNQueensUtil(board, col + 1) || res;
            board[i][col] = 0; // backtrack
        }
    }

    return res;
}

void solveNQueens() {
    int board[N][N] = {0};

    if (!solveNQueensUtil(board, 0)) {
        printf("No solution exists.\n");
    }
}

int main() {
    solveNQueens();

    return 0;
}
