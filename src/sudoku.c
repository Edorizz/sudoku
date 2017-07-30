#include <stdio.h>
#include <stdint.h>

uint8_t *sudoku_read(const char *path);
uint8_t  sudoku_solve(uint8_t *grid);
void sudoku_print(uint8_t *grid);

int
main(int argc, char **argv)
{
	uint8_t puzzle[9 * 9] = { 9, 5, 0,    0, 0, 1,    3, 4, 0,
				  0, 0, 0,    9, 0, 0,    1, 0, 7,
				  3, 6, 0,    5, 7, 0,    0, 0, 0,
				  
				  8, 7, 5,    0, 6, 0,    0, 0, 0,
				  0, 4, 0,    7, 0, 3,    0, 8, 0,
				  0, 0, 0,    0, 2, 0,    7, 6, 4,
				  
				  0, 0, 0,    0, 4, 7,    0, 1, 8,
				  7, 0, 4,    0, 0, 8,    0, 0, 0,
				  0, 3, 8,    1, 0, 0,    0, 7, 5 };

	sudoku_solve(puzzle);
	sudoku_print(puzzle);

	return 0;
}

uint8_t*
sudoku_read(const char *path)
{
}

uint8_t
sudoku_solve(uint8_t *grid)
{
	uint8_t p[9];
	int i, j, k, x, y, nx, ny;

	/* Find empty slot */
	for (i = 0; i != 9 * 9 && grid[i]; ++i)
		;

	if (i == 9 * 9) {
		return 1;
	}

	/* Set empty coordinates */
	y = i / 9;
	x = i % 9;

	/* Fill possible number array */
	for (j = 0; j != 9; ++j) {
		p[j] = j + 1;
	}

	/* Remove used numbers (row) */
	for (j = 0; j != 9; ++j) {
		if (grid[y * 9 + j]) {
			p[grid[y * 9 + j] - 1] = 0;
		}
	}

	/* Remove used numbers (col) */
	for (j = 0; j != 9; ++j) {
		if (grid[j * 9 + x]) {
			p[grid[j * 9 + x] - 1] = 0;
		}
	}

	/* Remove used numbers (square) */
	nx = x / 3 * 3;
	ny = y / 3 * 3;
	for (j = 0; j != 3; ++j) {
		for (k = 0; k != 3; ++k) {
			if (grid[(ny + j) * 9 + nx + k]) {
				p[grid[(ny + j) * 9 + nx + k] - 1] = 0;
			}
		}
	}

	/* Solve the puzzle recursively */
	for (j = 0; j != 9; ++j) {
		if (p[j]) {
			grid[i] = j + 1;
			
			if (sudoku_solve(grid)) {
				break;
			}
		}
	}

	if (j == 9) {
		grid[i] = 0;
		return 0;
	} else {
		return 1;
	}
}

void
sudoku_print(uint8_t *grid)
{
	int i, j;

	for (i = 0; i != 9; ++i) {
		if (i && i % 3 == 0) {
			printf(" ------+-------+------\n");
		}

		for (j = 0; j != 9; ++j) {
			if (j && j % 3 == 0) {
				printf(" |");
			}

			printf(" %c", grid[i * 9 + j] ? grid[i * 9 + j] + '0' : ' ');
		}
		putchar('\n');
	}
}
