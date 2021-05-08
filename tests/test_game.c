#include "test.h"

#include "snake/game.h"

#include <assert.h>

static void remove_cells(Game *game, CellType type)
{
    int row;
    int column;

    for (row = 0; row < game->board.rows; ++row) {
        for (column = 0; column < game->board.columns; ++column) {
            if (board_get(&game->board, row, column) == type) {
                (void)board_set(&game->board, row, column, CELL_EMPTY);
            }
        }
    }
}

static Game *create_test_game(GameMode mode)
{
    GameConfig config;

    game_config_default(&config);
    config.rows = 24;
    config.columns = 30;
    config.mode = mode;
    config.apple_count = 2;
    config.trap_count = 0;
    config.snake_length = 3;
    config.random_seed = 12345U;
    return game_create(&config);
}

static void test_config(void)
{
    GameConfig config;

    game_config_default(&config);
    config.rows = -1;
    config.columns = 1000;
    config.mode = (GameMode)99;
    config.apple_count = 0;
    config.trap_count = -3;
    config.speed_coeff = 999;
    config.snake_length = 1;

    game_config_sanitize(&config);
    assert(config.rows == 24);
    assert(config.columns == 150);
    assert(config.mode == GAME_WITH_WALLS);
    assert(config.apple_count == 1);
    assert(config.trap_count == 0);
    assert(config.speed_coeff == 20);
    assert(config.snake_length == 3);
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
    Game *game = create_test_game(GAME_WITH_WALLS);
    Direction direction = {0, 1};
    int row;
    int column;
    size_t initial_length;
    GameEvent event;

    assert(game != NULL);
    remove_cells(game, CELL_APPLE);
    remove_cells(game, CELL_TRAP);
    game->apples_remaining = 2;

    row = game->snake.head->row;
    column = game->snake.head->column + 1;
    assert(board_set(&game->board, row, column, CELL_APPLE));

    initial_length = game->snake.length;
    game_start(game);
    event = game_step(game, &direction);

    assert((event & GAME_EVENT_APPLE) != 0);
    assert((event & GAME_EVENT_LEVEL_UP) == 0);
    assert(game->score == 5);
    assert(game->apples_remaining == 1);
    assert(game->snake.length == initial_length + 1U);
    assert(!game_is_over(game));

    game_destroy(game);
}

static void test_level_up(void)
{
    Game *game = create_test_game(GAME_WITH_WALLS);
    Direction direction = {0, 1};
    int row;
    int column;
    GameEvent event;

    assert(game != NULL);
    remove_cells(game, CELL_APPLE);
    remove_cells(game, CELL_TRAP);
    game->apples_remaining = 1;

    row = game->snake.head->row;
    column = game->snake.head->column + 1;
    assert(board_set(&game->board, row, column, CELL_APPLE));

    game_start(game);
    event = game_step(game, &direction);

    assert((event & GAME_EVENT_APPLE) != 0);
    assert((event & GAME_EVENT_LEVEL_UP) != 0);
    assert(game->level == 2);
    assert(game->trap_count == 1);
    assert(game->apples_per_level == 3);
    assert(!game->started);
    assert(!game_is_over(game));

    game_destroy(game);
}

static void test_portal_traversal(void)
{
    Game *game = create_test_game(GAME_WITH_PORTALS);
    Direction direction = {0, 1};
    int portal_left;
    int row;
    GameEvent event;

    assert(game != NULL);
    remove_cells(game, CELL_APPLE);
    remove_cells(game, CELL_TRAP);
    game->apples_remaining = 99;

    portal_left = game->board.columns / 2 - 9;
    row = game->board.rows / 2;
    assert(game->snake.head->column == portal_left - 2);

    game_start(game);
    event = game_step(game, &direction);
    assert((event & GAME_EVENT_MOVED) != 0);
    assert(game->snake.head->column == portal_left - 1);

    event = game_step(game, &direction);
    assert((event & GAME_EVENT_MOVED) != 0);
    assert(game->snake.head->row == row);
    assert(game->snake.head->column == portal_left + 18);
    assert(!game_is_over(game));

    game_destroy(game);
}

static void test_collision(void)
{
    Game *game = create_test_game(GAME_WITH_WALLS);
    Direction direction = {0, 1};
    int row;
    int column;
    GameEvent event;

    assert(game != NULL);
    row = game->snake.head->row;
    column = game->snake.head->column + 1;
    assert(board_set(&game->board, row, column, CELL_WALL));

    game_start(game);
    event = game_step(game, &direction);

    assert((event & GAME_EVENT_GAME_OVER) != 0);
    assert(game->end_reason == GAME_END_COLLISION);
    assert(game_is_over(game));

    game_destroy(game);
}

void test_game_suite(void)
{
    test_config();
    test_direction();
    test_apple_growth();
    test_level_up();
    test_portal_traversal();
    test_collision();
}
