/* C library */
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#define SOLVE		0
#define GENERATE	1

#define BIT(n)		(1 << (n))

/* Sudoku functions */
void sudoku_read(FILE *fp, uint8_t *puzzle);
void sudoku_print(uint8_t *puzzle);
uint8_t sudoku_solve(uint8_t *puzzle);

void usage(const char *pname);

int
main(int argc, char **argv)
{
	FILE *fp;
	uint8_t puzzle[9 * 9], flags;
	const char *puzzle_name;
	int i;

	flags = 0;
	puzzle_name = NULL;
	for (i = 1; i != argc; ++i) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'g':
				flags |= BIT(GENERATE);
				break;

			case 's':
				flags |= BIT(SOLVE);
				puzzle_name = argv[i + 1];
				break;

			case 'h':
			default:
				usage(argv[0]);
				return 0;
			}
		}
	}

	if (flags & BIT(SOLVE)) {
		fp = puzzle_name ? fopen(puzzle_name, "r") : stdin;
		if (fp == NULL) {
			perror("fopen");
			return 1;
		}
		
		sudoku_read(fp, puzzle);
		sudoku_solve(puzzle);
		sudoku_print(puzzle);

	} else if (flags & BIT(GENERATE)) {
		
	}

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
sudoku_solve(uint8_t *puzzle)
{
	uint8_t p[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };	/* Array of possible numbers that can go in the current square */
	int i, j, k, x, y, nx, ny;			/* [Variable usage]
							 * i: Current square index (puzzle[i])
							 * j, k: Loop variables
							 * x, y: Current square coordinates
							 * nx, ny: Normalized square coordinates (top left of current 3x3 block)
							 */

	/* Find empty slot */
	for (i = 0; i != 9 * 9 && puzzle[i]; ++i)
		;

	/* Return if puzzle is already solved */
	if (i == 9 * 9) {
		return 1;
	}

	/* Save empty slot coordinates */
	y = i / 9;
	x = i % 9;

	/* Remove used numbers (row/col) */
	for (j = 0; j != 9; ++j) {
		if (puzzle[y * 9 + j]) {
			p[puzzle[y * 9 + j] - 1] = 0;
		}

		if (puzzle[j * 9 + x]) {
			p[puzzle[j * 9 + x] - 1] = 0;
		}
	}

	/* Remove used numbers (square) */
	nx = x / 3 * 3;
	ny = y / 3 * 3;
	for (j = 0; j != 3; ++j) {
		for (k = 0; k != 3; ++k) {
			if (puzzle[(ny + j) * 9 + nx + k]) {
				p[puzzle[(ny + j) * 9 + nx + k] - 1] = 0;
			}
		}
	}

	/* Solve the puzzle recursively */
	for (j = 0; j != 9; ++j) {
		if (p[j]) {
			puzzle[i] = j + 1;
			
			if (sudoku_solve(puzzle)) {
				return 1;
			}
		}
	}

	/* Reset the current slot and return false if a solution wasn't found */
	puzzle[i] = 0;
	return 0;
}

void
sudoku_print(uint8_t *puzzle)
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

			printf(" %c", puzzle[i * 9 + j] ? puzzle[i * 9 + j] + '0' : ' ');
		}
		putchar('\n');
	}
}

void
usage(const char *pname)
{
	printf("usage: %s [-s puzzle] [-g [seed]]\n"
	       "-s: solve 'puzzle'\n"
	       "-g: generate a new puzzle using an optional 'seed'\n", pname);
}
