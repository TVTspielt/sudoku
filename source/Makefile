FILES = main.c sudoku.c
TARGET = sudoku_game

IDIR = .

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -I$(IDIR)
LDFLAGS = -lm


.PHONY: clean all
all: $(TARGET)

$(TARGET): $(FILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(FILES) $(LDFLAGS)

clean:
	rm $(TARGET)
	
