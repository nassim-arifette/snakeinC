#ifndef SNAKE_MODEL_H
#define SNAKE_MODEL_H

#include "board.h"

#include <stdbool.h>

typedef struct SnakeNode {
    int row;
    int column;
    struct SnakeNode *next;
} SnakeNode;

typedef struct Snake {
    SnakeNode *head;
    SnakeNode *tail;
    int length;
} Snake;

void snake_init(Snake *snake);
void snake_clear(Snake *snake, Board *board);
bool snake_place_horizontal(Snake *snake, Board *board, int row, int start_column, int length);
bool snake_append_head(Snake *snake, Board *board, int row, int column);
bool snake_remove_tail(Snake *snake, Board *board);
bool snake_tail_is(const Snake *snake, int row, int column);

#endif
