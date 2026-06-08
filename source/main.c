#include "sudoku.h"
#include "generator.c"
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

void displayBoard(sudoku* board) {
    printf("o  ");
    for (int i = 0; i < SIZE; i++) {
        printf(" %d", i);
        if (i % SIZE_S == SIZE_S - 1) {
            printf("  ");
        }
    }
    printf("\n");
    for (int r = 0; r < SIZE; r++) {
        if (r % SIZE_S == 0) {
            printf("  |-------|-------|-------|\n");
        }
        printf("%d ", r);
        for (int c = 0; c < SIZE; c++) {
            if (c % SIZE_S == 0) {
                printf("| ");
            }
            if (board->sBoard[r][c] == FREE) {
                printf("_ ");
            } else {
                printf("%d ", board->sBoard[r][c]);
            }
        }
        printf("|\n");
    }
    printf("  |-------|-------|-------|\n");
}

bool checkSolution(sudoku* board) {
    range* range = constrRange();

    for (unsigned i = 0; i < SIZE; i++) {
        for (unsigned m = 0; m < SIZE_S; m++) {
            getRange(range, board, i, m);
            if (range->bitmask != COMPLETE_RANGE) {
                deleteRange(range);
                return false;
            }
        }
    }

    deleteRange(range);
    return true;
}

void entryResponse(sudoku* board, int ret) {
    displayBoard(board);
    if (ret == RETURN_ILLEGAL) {
        printf("That number is fixed. Please try again.\n");
    } else if (ret == RETURN_INVALID) {
        printf("That is not a valid move. Please try again.\n");
    }
}

int main(void) {
    sudoku* board = constrBoard();

    printf("Enter a seed (or '0' for a random puzzle.): ");
    unsigned seed;
    scanf("%u", &seed);
    if (seed == 0) {
        srand(time(NULL));
    }

    generate(board, seed);
    displayBoard(board);

    unsigned option;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Enter a number\n");
        printf("2. Delete a number\n");
        printf("3. Check your solution\n");
        printf("4. Exit the program\n");
        printf("Your choice: ");
        scanf("%u", &option);

        if (option < 1 || option > 4) {
            printf("Invalid number. Please try again!\n");
            continue;
        }

        unsigned row, col, val;
        int ret;

        switch (option) {
        case 1:
            printf("Enter row, column and value (r c v): ");
            scanf("%u %u %u", &row, &col, &val);
            ret = addEntry(val, row, col, board);
            entryResponse(board, ret);
            break;

        case 2:
            printf("Enter row and column (r c): ");
            scanf("%u %u", &row, &col);
            ret = delEntry(row, col, board);
            entryResponse(board, ret);
            break;

        case 3:
            if (checkSolution(board)) {
                printf("That is correct! Congratulations! Exiting program...\n");
                return 0;
            } else {
                displayBoard(board);
                printf("Unfortunately, that is not quite right... Please try again.\n");
            }
            break;

        case 4:
            return 0;
        }
    }
    return 0;
}