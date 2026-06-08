#include "sudoku.h"
#include <stdlib.h>
#include <stdbool.h>

#define COMBOS_BASE 6
#define COMBOS_ALL 46656

//"Difficulty" value for the puzzle. (Target # numbers to remove)
#define TARGET 64

const unsigned BASIS[COMBOS_BASE][SIZE_S] = { {1, 2, 3},{1,3,2},{2,1,3},{2,3,1},{3,1,2},{3,2,1} };

unsigned comboBaseDigit(unsigned num, unsigned digit) {
    unsigned m = num % COMBOS_BASE;
    unsigned q = num;
    for (unsigned i = 0; i < digit; i++) {
        q = q / COMBOS_BASE;
        m = q % COMBOS_BASE;
    }
    return m;
}

void swapRandom(unsigned* permutation, unsigned split, unsigned amount) {
    if (split >= amount - 1) {
        return;
    }
    unsigned swap = permutation[split];
    unsigned random = amount - 1 - (rand() % (amount - split - 1));
    permutation[split] = permutation[random];
    permutation[random] = swap;
}

bool compatible(unsigned combo1, unsigned combo2) {
    if (combo1 >= COMBOS_ALL || combo2 >= COMBOS_ALL) {
        return false;
    }
    for (unsigned i = 0; i < SIZE_S; i++) {
        unsigned row1 = comboBaseDigit(combo1, i);
        unsigned col1 = comboBaseDigit(combo1, i + SIZE_S);
        unsigned row2 = comboBaseDigit(combo2, i);
        unsigned col2 = comboBaseDigit(combo2, i + SIZE_S);

        for (int j = 0; j < SIZE_S; j++) {
            if (BASIS[row1][j] == BASIS[row2][j] && BASIS[col1][j] == BASIS[col2][j]) {
                return false;
            }
        }
    }
    return true;
}

bool tryAdd(sudoku* board, unsigned combo, unsigned val) {
    if (combo >= COMBOS_ALL) {
        return false;
    }
    unsigned marked[SIZE * 2] = { 0 };
    for (int i = 0; i < SIZE_S; i++) {
        unsigned row = comboBaseDigit(combo, i);

        for (int j = 0; j < SIZE_S; j++) {
            unsigned col = comboBaseDigit(combo, j + SIZE_S);
            unsigned rowIndex = BASIS[row][j] + (i * SIZE_S) - 1;
            unsigned colIndex = BASIS[col][i] + (j * SIZE_S) - 1;

            marked[2 * (SIZE_S * i + j)] = rowIndex;
            marked[2 * (SIZE_S * i + j) + 1] = colIndex;
            if (board->sBoard[rowIndex][colIndex] != FREE) {
                return false;
            }
        }
    }
    for (int i = 0; i < SIZE; i++) {
        fixEntry(val, marked[2 * i], marked[2 * i + 1], board);
    }
    return true;
}

bool tryGenerate(sudoku* board, unsigned* permutation, unsigned index, unsigned layer) {
    if (index == COMBOS_ALL) {
        return false;
    }
    if (layer == SIZE) {
        return true;
    }

    bool success = false;
    while (!success && index < COMBOS_ALL) {
        swapRandom(permutation, index, COMBOS_ALL);
        index++;
        success = tryAdd(board, permutation[index], layer + 1);
    }
    return tryGenerate(board, permutation, index, layer + 1);
}

void generateSolution(sudoku* board, unsigned seed) {
    if (seed != 0) {
        srand(seed);
    }

    sudoku* base = constrBoard();
    unsigned permutation[COMBOS_ALL] = { 0 };
    for (unsigned i = 0; i < COMBOS_ALL; i++) {
        permutation[i] = i;
    }
    bool valid = false;
    while (!valid) {
        copy(base, board);
        valid = tryGenerate(board, permutation, 0, 0);
    }
}

bool removeIfNoOptions(sudoku* board, unsigned r, unsigned c) {
    unsigned value = board->sBoard[r][c];
    unsigned mask = 1 << (value - 1);
    fixEntry(0, r, c, board);
    unsigned squ = (3 * (r / SIZE_S) + (c / SIZE_S));
    range* square = constrRange();
    range* compare = constrRange();
    getSquare(square, board, squ);
    for (unsigned i = 0; i < SIZE; i++) {
        if (square->rRange[i] == FREE) {
            unsigned row = i / SIZE_S + (squ / SIZE_S * SIZE_S);
            unsigned col = i % SIZE_S + (squ % SIZE_S * SIZE_S);
            if(row == r && col == c) {
                continue;
            }
            getRow(compare, board, row);
            unsigned rowFull = compare->bitmask & mask;
            getColumn(compare, board, col);
            if ((compare->bitmask & mask) == 0 && rowFull == 0) {
                deleteRange(square);
                deleteRange(compare);
                fixEntry(value, r, c, board);
                return false;
            }
        }
    }
    deleteRange(square);
    deleteRange(compare);
    return true;
}

bool removeIfRangeFull(sudoku* board, unsigned r, unsigned c) {
    range* range = constrRange();
    unsigned modeVals[SIZE_S] = { r, c, (3 * (r / SIZE_S) + (c / SIZE_S)) };
    for (unsigned m = 0; m < SIZE_S; m++) {
        getRange(range, board, modeVals[m], m);
        if (range->bitmask == COMPLETE_RANGE) {
            fixEntry(0, r, c, board);
            deleteRange(range);
            return true;
        }
    }
    deleteRange(range);
    return false;
}

void tryRemove(sudoku* board, unsigned r, unsigned c) {
    if(board->sBoard[r][c] == FREE) {
    }
    if (!removeIfRangeFull(board, r, c)) {
        removeIfNoOptions(board, r, c);
        return;
    }
}

void createPuzzle(sudoku* board, unsigned seed, unsigned target) {
    if (seed != 0) {
        srand(seed);
    }

    unsigned permutation[SIZE * SIZE] = { 0 };
    for (unsigned i = 0; i < SIZE * SIZE; i++) {
        permutation[i] = i;
    }
    for (unsigned i = 0; i < target; i++) {
        swapRandom(permutation, i, SIZE * SIZE);
        tryRemove(board, permutation[i] % 9, (permutation[i] / 9) % 9);
    }
}

void generate(sudoku* board, unsigned seed) {
    generateSolution(board, seed);
    createPuzzle(board, 0, TARGET);
}