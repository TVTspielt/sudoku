#include <stdio.h>
#include <time.h>
#include "../sudoku.h"
#include "../generator.c"

#define TRIES 50

bool checkSolution(sudoku* board) {
    range* range = constrRange();

    for (unsigned i = 0; i < SIZE; i++) {
        for(unsigned m = 0; m < SIZE_S; m++) {
            getRange(range, board, i, m);
            if(range->bitmask != COMPLETE_RANGE) {
                deleteRange(range);
                return false;
            }
        }
    }

    deleteRange(range);
    return true;
}

void test_solutions(sudoku* board) {
    unsigned valid = 0;
    for(int i = 0; i < TRIES; i++) {
        generateSolution(board, i+time(NULL));
        if(checkSolution(board)) {
            valid++;
        }
    }
    printf("Valid solutions generated: %u/%u\n", valid, TRIES);
}

void test_generator() {
    
    sudoku* board = constrBoard();
    generate(board, time(NULL));

    FILE* output;
    output = fopen("generator.out", "w");
    for(int r = 0; r < SIZE; r++) {
        for(int c = 0; c < SIZE; c++) {
            fprintf(output, "%u ", board->sBoard[r][c]);
        }
        fprintf(output, "\n");
    }
    fclose(output);

    test_solutions(board);

    printf("Generator: Passed 1 test.\n");
}