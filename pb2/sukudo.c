#include <stdio.h>
#include <stdbool.h>

bool isValid(int board[], int row, int col, int num);
bool solveSudoku(int board[], int row, int col);

int main() {
	// int board[] = {-1, 1, 4, -1, -1, 3, -1, 2, -1, -1, 3, -1, -1, -1, -1, -1};
	int board[] = {2, -1, -1, -1, -1, -1, 1, -1, -1, 2, -1, -1, -1, -1, -1, 4};
	// int board[] = { -1, 2, 3, -1, 1, -1, -1, 4, 2, -1, -1, 3, -1, 1, 4, -1 };
	if (solveSudoku(board, 0, 0)) {
		printf("Solution:\n");
		for (int i = 0; i < 16; i++) {
			printf("%d ", board[i]);
			if ((i + 1) % 4 == 0) {
				printf("\n");
			}
		}
	}
	else {
		printf("No solution found.\n");
	}
	return 0;
}

bool solveSudoku(int board[], int row, int col) {
	if (row == 4) {
		return true;
	}

	int nextRow = row;
	int nextCol = col + 1;
	if (nextCol == 4) {
		nextRow = row + 1;
		nextCol = 0;
	}

	if (board[row * 4 + col] != -1) {
		return solveSudoku(board, nextRow, nextCol);
	}

	for (int num = 1; num <= 4; num++) {
		if (isValid(board, row, col, num)) {
			board[row * 4 + col] = num;
			if (solveSudoku(board, nextRow, nextCol)) {
				return true;
			}
			board[row * 4 + col] = -1;
		}
	}

	return false;
}

bool isValid(int board[], int row, int col, int num) {
	for (int i = 0; i < 4; i++) {
		if (board[row * 4 + i] == num) {
			return false;
		}
		if (board[i * 4 + col] == num) {
			return false;
		}
	}

	int baseRow = (row / 2) * 2;
	int baseCol = (col / 2) * 2;

	for (int i = 0; i < 4; i++) {
		if (board[(baseRow + i / 2) * 4 + (baseCol + i % 2)] == num) {
			return false;
		}
	}

	// for (int i = baseRow; i <= baseRow + 1; i++) {
	// 	for (int j = baseCol; j <= baseCol + 1; j++) {
	// 		if (board[i * 4 + j] == num) {
	// 			return false;
	// 		}
	// 	}
	// }

	return true;
}