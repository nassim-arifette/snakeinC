#include "game.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void remove_cells(Game *game, CellType type)
{
    int row;
    int column;

    for (row = 0; row < game->board.rows; ++row) {
        for (column = 0; column < game->board.columns; ++column) {
            if (board_get(&game->board, row, column) == type) {
                board_set(&game->board, row, column, CELL_EMPTY);
            }
        }
    }
}

static void test_board(void)
{
    Board board = {0};

    assert(board_init(&board, 3, 4));
    assert(board.rows == 3);
    assert(board.columns == 4);
    assert(board_set(&board, 1, 2, CELL_APPLE));
    assert(board_get(&board, 1, 2) == CELL_APPLE);
    assert(board_get(&board, -1, 2) == CELL_WALL);
    board_destroy(&board);
    assert(board.cells == NULL);
}

static void test_direction(void)
{
    Direction direction = {0, 1};

    direction_try_update(&direction, 0, -1);
    assert(direction.row == 0 && direction.column == 1);

    direction_try_update(&direction, -1, 0);
    assert(direction.row == -1 && direction.column == 0);

    direction_try_update(&direction, 1, 0);
    assert(direction.row == -1 && direction.column == 0);
}

static void test_apple_growth(void)
{
    GameConfig config;
    Game *game;
    Direction direction = {0, 1};
    int target_row;
    int target_column;
    int initial_length;
    GameEvent event;

    game_config_default(&config);
    config.rows = 20;
    config.columns = 30;
    config.mode = GAME_INFINITE_ALT;
    config.apple_count = 1;
    config.trap_count = 0;
    config.snake_length = 3;

    game = game_create(&config);
    assert(game != NULL);

    remove_cells(game, CELL_APPLE);
    remove_cells(game, CELL_TRAP);
    game->apples_remaining = 1;

    target_row = game->snake.head->row;
    target_column = game->snake.head->column + 1;
    assert(board_get(&game->board, target_row, target_column) == CELL_EMPTY);
    assert(board_set(&game->board, target_row, target_column, CELL_APPLE));

    initial_length = game->snake.length;
    game_start(game);
    event = game_step(game, &direction);

    assert((event & GAME_EVENT_APPLE) != 0);
    assert(game->score == 5);
    assert(game->snake.length == initial_length + 1);
    assert(game->level == 2);
    assert(!game_is_over(game));

    game_destroy(game);
}

static void test_collision(void)
{
    GameConfig config;
    Game *game;
    Direction direction = {0, 1};
    int target_row;
    int target_column;
    GameEvent event;

    game_config_default(&config);
    config.rows = 20;
    config.columns = 30;
    config.mode = GAME_WITH_WALLS;
    config.snake_length = 3;

    game = game_create(&config);
    assert(game != NULL);

    target_row = game->snake.head->row;
    target_column = game->snake.head->column + 1;
    assert(board_set(&game->board, target_row, target_column, CELL_WALL));

    game_start(game);
    event = game_step(game, &direction);

    assert((event & GAME_EVENT_GAME_OVER) != 0);
    assert(game->end_reason == GAME_END_COLLISION);
    assert(game_is_over(game));

    game_destroy(game);
}

int main(void)
{
    srand(1U);

    test_board();
    test_direction();
    test_apple_growth();
    test_collision();

    puts("core tests: ok");
    return 0;
}
