#include "sudoku.h"
#include <stdlib.h>

sudoku* constrBoard() {
    sudoku* loc = malloc(sizeof(*loc));
    clear(loc);
    return loc;
}

void deleteBoard(sudoku* board) {
    free(board);
}

range* constrRange() {
    range* loc = malloc(sizeof(*loc));
    return loc;
}

int clear(sudoku* board) {
    if (board == NULL) {
        return RETURN_NULL;
    }
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            board->sBoard[r][c] = EMPTY;
            board->fixMask[r][c] = FREE;
        }
    }
    return RETURN_SUCCESS;
}

int copy(sudoku* source, sudoku* dest) {
    if(source == NULL || dest == NULL) {
        return RETURN_NULL;
    }
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            dest->sBoard[r][c] = source->sBoard[r][c];
            dest->fixMask[r][c] = source->sBoard[r][c];
        }
    }
    return RETURN_SUCCESS;
}

int getRange(range* range, sudoku* board, unsigned nr, unsigned mode) {
    if (mode == MODE_ROW) {
        return getRow(range, board, nr);
    } else if (mode == MODE_COL) {
        return getColumn(range, board, nr);
    } else if (mode == MODE_SQU) {
        return getSquare(range, board, nr);
    }
    return RETURN_INVALID;
}

int getRow(range* range, sudoku* board, unsigned nr) {
    if (range == NULL || board == NULL) {
        return RETURN_NULL;
    } else if (nr > SIZE - 1) {
        return RETURN_INVALID;
    }

    range->bitmask = 0;
    for (int i = 0; i < SIZE; i++) {
        range->rRange[i] = board->sBoard[nr][i];
        if (range->rRange[i] != 0) {
            range->bitmask |= (1 << (range->rRange[i] - 1));
        }
    }
    return RETURN_SUCCESS;
}

int getColumn(range* range, sudoku* board, unsigned nr) {
    if (range == NULL || board == NULL) {
        return RETURN_NULL;
    } else if (nr > SIZE - 1) {
        return RETURN_INVALID;
    }

    range->bitmask = 0;
    for (int i = 0; i < SIZE; i++) {
        range->rRange[i] = board->sBoard[i][nr];
        if (range->rRange[i] != 0) {
            range->bitmask |= (1 << (range->rRange[i] - 1));
        }
    }
    return RETURN_SUCCESS;
}

int getSquare(range* range, sudoku* board, unsigned nr) {
    if (range == NULL || board == NULL) {
        return RETURN_NULL;
    } else if (nr > SIZE - 1) {
        return RETURN_INVALID;
    }

    range->bitmask = 0;
    for (int i = 0; i < SIZE; i++) {
        unsigned row = i / SIZE_S + (nr / SIZE_S * SIZE_S);
        unsigned column = i % SIZE_S + (nr % SIZE_S * SIZE_S);
        range->rRange[i] = board->sBoard[row][column];
        if (range->rRange[i] != 0) {
            range->bitmask |= (1 << (range->rRange[i] - 1));
        }
    }
    return RETURN_SUCCESS;
}

void deleteRange(range* range) {
    free(range);
}

int fixEntry(unsigned val, unsigned x, unsigned y, sudoku* board) {
    if (board == NULL) {
        return RETURN_NULL;
    } else if (x >= SIZE || y >= SIZE || val > SIZE) {
        return RETURN_INVALID;
    }
    board->sBoard[x][y] = val;
    board->fixMask[x][y] = (val != 0);
    return RETURN_SUCCESS;
}

// No way of checking for fixed entries right now
int addEntry(unsigned val, unsigned x, unsigned y, sudoku* board) {
    if (board == NULL) {
        return RETURN_NULL;
    } else if (x >= SIZE || y >= SIZE || val > SIZE) {
        return RETURN_INVALID;
    } else if (board->fixMask[x][y] == FIXED) {
        return RETURN_ILLEGAL;
    }
    board->sBoard[x][y] = val;
    return RETURN_SUCCESS;
}

int delEntry(unsigned x, unsigned y, sudoku* board) {
    return addEntry(0, x, y, board);
}