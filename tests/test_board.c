#include "test.h"

#include "snake/board.h"

#include <assert.h>
#include <stddef.h>

void test_board_suite(void)
{
    Board board = {0};

    assert(board_init(&board, 3, 4));
    assert(board.rows == 3);
    assert(board.columns == 4);
    assert(board_get(&board, 1, 2) == CELL_EMPTY);
    assert(board_set(&board, 1, 2, CELL_APPLE));
    assert(board_get(&board, 1, 2) == CELL_APPLE);
    assert(!board_set(&board, -1, 0, CELL_EMPTY));
    assert(board_get(&board, -1, 2) == CELL_WALL);

    board_fill(&board, CELL_TRAP);
    assert(board_get(&board, 0, 0) == CELL_TRAP);
    assert(board_get(&board, 2, 3) == CELL_TRAP);

    board_destroy(&board);
    assert(board.cells == NULL);
    assert(board.rows == 0);
    assert(board.columns == 0);
}
