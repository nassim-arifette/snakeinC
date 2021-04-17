#include "board.h"

#include <stdint.h>
#include <stdlib.h>

static size_t board_index(const Board *board, int row, int column)
{
    return (size_t)row * (size_t)board->columns + (size_t)column;
}

bool board_init(Board *board, int rows, int columns)
{
    size_t count;

    if (board == NULL || rows <= 0 || columns <= 0) {
        return false;
    }

    if ((size_t)rows > SIZE_MAX / (size_t)columns) {
        return false;
    }

    count = (size_t)rows * (size_t)columns;
    board->cells = calloc(count, sizeof(*board->cells));
    if (board->cells == NULL) {
        board->rows = 0;
        board->columns = 0;
        return false;
    }

    board->rows = rows;
    board->columns = columns;
    return true;
}

void board_destroy(Board *board)
{
    if (board == NULL) {
        return;
    }

    free(board->cells);
    board->cells = NULL;
    board->rows = 0;
    board->columns = 0;
}

bool board_in_bounds(const Board *board, int row, int column)
{
    return board != NULL &&
           board->cells != NULL &&
           row >= 0 &&
           row < board->rows &&
           column >= 0 &&
           column < board->columns;
}

CellType board_get(const Board *board, int row, int column)
{
    if (!board_in_bounds(board, row, column)) {
        return CELL_WALL;
    }

    return board->cells[board_index(board, row, column)];
}

bool board_set(Board *board, int row, int column, CellType cell)
{
    if (!board_in_bounds(board, row, column)) {
        return false;
    }

    board->cells[board_index(board, row, column)] = cell;
    return true;
}

void board_fill(Board *board, CellType cell)
{
    int row;
    int column;

    if (board == NULL || board->cells == NULL) {
        return;
    }

    for (row = 0; row < board->rows; ++row) {
        for (column = 0; column < board->columns; ++column) {
            board->cells[board_index(board, row, column)] = cell;
        }
    }
}
