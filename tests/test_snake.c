#include "test.h"

#include "snake/board.h"
#include "snake/snake.h"

#include <assert.h>

void test_snake_suite(void)
{
    Board board = {0};
    Snake snake;

    assert(board_init(&board, 5, 8));
    snake_init(&snake);

    assert(snake_place_horizontal(&snake, &board, 2, 2, 3));
    assert(snake.length == 3U);
    assert(snake.tail != NULL && snake.tail->column == 2);
    assert(snake.head != NULL && snake.head->column == 4);
    assert(board_get(&board, 2, 2) == CELL_SNAKE);
    assert(board_get(&board, 2, 4) == CELL_SNAKE);

    assert(snake_append_head(&snake, &board, 2, 5));
    assert(snake.length == 4U);
    assert(snake.head->column == 5);

    assert(snake_remove_tail(&snake, &board));
    assert(snake.length == 3U);
    assert(snake.tail->column == 3);
    assert(board_get(&board, 2, 2) == CELL_EMPTY);

    snake_clear(&snake, &board);
    assert(snake.length == 0U);
    assert(snake.head == NULL);
    assert(snake.tail == NULL);

    board_destroy(&board);
}
