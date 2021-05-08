#ifndef SNAKE_BOARD_H
#define SNAKE_BOARD_H

#include <stdbool.h>

typedef enum CellType {
    CELL_EMPTY = 0,
    CELL_SNAKE,
    CELL_APPLE,
    CELL_TRAP,
    CELL_WALL,
    CELL_PORTAL_TOP,
    CELL_PORTAL_BOTTOM,
    CELL_PORTAL_LEFT,
    CELL_PORTAL_RIGHT,
    CELL_PORTAL_WRAP_LEFT,
    CELL_PORTAL_WRAP_RIGHT
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
