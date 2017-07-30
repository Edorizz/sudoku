/* C library */
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

void sudoku_read(FILE *fp, uint8_t *puzzle);
void sudoku_print(uint8_t *grid);
uint8_t sudoku_solve(uint8_t *grid);

int
main(int argc, char **argv)
{
	FILE *fp;
	uint8_t puzzle[9 * 9];
	
	fp = argc == 1 ? stdin : fopen(argv[1], "r");
	if (fp == NULL) {
		perror("fopen");
		return 1;
	}

	sudoku_read(fp, puzzle);
	sudoku_solve(puzzle);
	sudoku_print(puzzle);

	return 0;
}

void
sudoku_read(FILE *fp, uint8_t *puzzle)
{
	int c, i;

	i = 0;
	while ((c = fgetc(fp)) && i != 9 * 9) {
		if (isdigit(c)) {
			puzzle[i++] = c - '0';
		}
	}
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
