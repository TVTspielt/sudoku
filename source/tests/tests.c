#include <stdbool.h>
#include <stdio.h>
#include "../sudoku.h"
#include "test_generator.h"

bool test_rows(sudoku* board, range* r) {
    for(int i = 0; i < 9; i++) {
        getRow(r, board, i);
        if(r->bitmask != (1u << i)) {
            printf("Test failed: rows at %d.\n", i);
            return false;
        }
    }
    printf("Test passed: rows\n");
    return true;
}

bool test_cols(sudoku* board, range* r) {
    for(int i = 0; i < 9; i++) {
        getColumn(r, board, i);
        if(r->bitmask != (1u << i)) {
            printf("Test failed: columns at %d.\n", i);
            return false;
        }
    }
    printf("Test passed: columns\n");
    return true;
}

bool test_squares(sudoku* board, range* r) {
    unsigned expected[9] = {0b111, 0, 0, 0, 0b111000, 0, 0, 0, 0b111000000};

    for(int i = 0; i < 9; i++) {
        getSquare(r, board, i);
        if(r->bitmask != expected[i]) {
            printf("Test failed: squares at %d.\n", i);
            return false;
        }
    }
    printf("Test passed: squares\n");
    return true;
}

void test_generic() {
    sudoku* board = constrBoard();
    range* r = constrRange();

    for(int i = 0; i < 9; i++) {
        board->sBoard[i][i] = i+1;
    }

    unsigned pass = test_rows(board, r) + test_cols(board, r) + test_squares(board, r);
    printf("Generic: Passed %u/3 tests.\n", pass);

    deleteBoard(board);
    deleteRange(r);
}

int main(void) {
    test_generator();
    test_generic();

    return 0;
}