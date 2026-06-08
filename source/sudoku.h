#ifndef SUDOKU_H
#define SUDOKU_H

#define RETURN_SUCCESS 0
#define RETURN_NULL -1
#define RETURN_INVALID -2
#define RETURN_ILLEGAL -4

// Amount of numbers in a row, column or square.
#define SIZE 9
// Amount of squares across or down the puzzle.
#define SIZE_S 3

// What an empty field in the puzzle is represented as.
#define EMPTY 0

#define COMPLETE_RANGE ((1 << SIZE)-1)

#define MODE_ROW 0
#define MODE_COL 1
#define MODE_SQU 2

#define FIXED 1
#define FREE 0

typedef struct {
    unsigned sBoard[SIZE][SIZE];
    unsigned fixMask[SIZE][SIZE];
} sudoku;

typedef struct {
    unsigned rRange[SIZE];
    unsigned bitmask;
} range;

/*
 * Returns pointer to an empty sudoku board. 
 */
sudoku* constrBoard();

int clear(sudoku* board);

int copy(sudoku* source, sudoku* dest);

/*
 * Frees the memory used up by this sudoku board.
 */
void deleteBoard(sudoku* board);

/*
 * Returns pointer to an empty range, used to store either a single row, column or square of a puzzle.
 */
range* constrRange();

/*
 * Get either row, column or square, depending on the specified mode.
 */
int getRange(range* range, sudoku* board, unsigned nr, unsigned mode);

/*
 * Get a single row of this puzzle.
 */
int getRow(range* range, sudoku* board, unsigned nr);

/*
 * Get a single column of this puzzle.
 */
int getColumn(range* range, sudoku* board, unsigned nr);

/*
 * Get a single square of this puzzle.
 */
int getSquare(range* range, sudoku* board, unsigned nr);

/*
 * Frees the memory used up by this range.
 */
void deleteRange(range* range);

int fixEntry(unsigned val, unsigned x, unsigned y, sudoku* board);

/*
* Adding a value to a position on the board.
*/
int addEntry(unsigned val, unsigned x, unsigned y, sudoku* board);

/*
* Setting a value at a position to NULL.
*/
int delEntry(unsigned x, unsigned y, sudoku* board);

#endif