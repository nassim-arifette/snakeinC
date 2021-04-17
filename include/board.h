#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stddef.h>

typedef enum CellType {
    CELL_EMPTY = 0,
    CELL_SNAKE = 1,
    CELL_APPLE = 2,
    CELL_TRAP = 3,
    CELL_WALL = 4,
    CELL_PORTAL_TOP = 6,
    CELL_PORTAL_BOTTOM = 7,
    CELL_PORTAL_LEFT = 8,
    CELL_PORTAL_RIGHT = 9,
    CELL_PORTAL_WRAP_LEFT = 10,
    CELL_PORTAL_WRAP_RIGHT = 11
} CellType;

typedef struct Board {
    int rows;
    int columns;
    CellType *cells;
} Board;

bool board_init(Board *board, int rows, int columns);
void board_destroy(Board *board);
bool board_in_bounds(const Board *board, int row, int column);
CellType board_get(const Board *board, int row, int column);
bool board_set(Board *board, int row, int column, CellType cell);
void board_fill(Board *board, CellType cell);

#endif
