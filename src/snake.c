#include "snake.h"

#include <stdlib.h>

void snake_init(Snake *snake)
{
    if (snake == NULL) {
        return;
    }

    snake->head = NULL;
    snake->tail = NULL;
    snake->length = 0;
}

void snake_clear(Snake *snake, Board *board)
{
    SnakeNode *node;

    if (snake == NULL) {
        return;
    }

    node = snake->tail;
    while (node != NULL) {
        SnakeNode *next = node->next;

        if (board != NULL && board_get(board, node->row, node->column) == CELL_SNAKE) {
            board_set(board, node->row, node->column, CELL_EMPTY);
        }

        free(node);
        node = next;
    }

    snake_init(snake);
}

bool snake_append_head(Snake *snake, Board *board, int row, int column)
{
    SnakeNode *node;

    if (snake == NULL || board == NULL || !board_in_bounds(board, row, column)) {
        return false;
    }

    node = malloc(sizeof(*node));
    if (node == NULL) {
        return false;
    }

    node->row = row;
    node->column = column;
    node->next = NULL;

    if (snake->head == NULL) {
        snake->tail = node;
    } else {
        snake->head->next = node;
    }

    snake->head = node;
    snake->length++;
    board_set(board, row, column, CELL_SNAKE);
    return true;
}

bool snake_remove_tail(Snake *snake, Board *board)
{
    SnakeNode *old_tail;

    if (snake == NULL || snake->tail == NULL || board == NULL) {
        return false;
    }

    old_tail = snake->tail;
    if (board_get(board, old_tail->row, old_tail->column) == CELL_SNAKE) {
        board_set(board, old_tail->row, old_tail->column, CELL_EMPTY);
    }

    snake->tail = old_tail->next;
    if (snake->tail == NULL) {
        snake->head = NULL;
    }

    free(old_tail);
    snake->length--;
    return true;
}

bool snake_place_horizontal(Snake *snake, Board *board, int row, int start_column, int length)
{
    int i;

    if (snake == NULL || board == NULL || length <= 0) {
        return false;
    }

    snake_clear(snake, board);

    for (i = 0; i < length; ++i) {
        int column = start_column + i;

        if (!board_in_bounds(board, row, column) ||
            board_get(board, row, column) != CELL_EMPTY ||
            !snake_append_head(snake, board, row, column)) {
            snake_clear(snake, board);
            return false;
        }
    }

    return true;
}

bool snake_tail_is(const Snake *snake, int row, int column)
{
    return snake != NULL &&
           snake->tail != NULL &&
           snake->tail->row == row &&
           snake->tail->column == column;
}
