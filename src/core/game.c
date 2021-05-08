#include "snake/game.h"

#include "level.h"

#include <stdlib.h>

#define GAME_MIN_ROWS 24
#define GAME_MAX_ROWS 100
#define GAME_MIN_COLUMNS 30
#define GAME_MAX_COLUMNS 150
#define PORTAL_HALF_EXTENT 9

static void game_end(Game *game, GameEndReason reason)
{
    if (game != NULL && game->end_reason == GAME_END_NONE) {
        game->end_reason = reason;
    }
}

void game_config_default(GameConfig *config)
{
    if (config == NULL) {
        return;
    }

    *config = (GameConfig){
        .rows = 40,
        .columns = 60,
        .mode = GAME_WITH_WALLS,
        .apple_count = 5,
        .trap_count = 0,
        .speed_coeff = 3,
        .snake_length = 10,
        .random_seed = 0U,
    };
}

void game_config_sanitize(GameConfig *config)
{
    int max_snake;

    if (config == NULL) {
        return;
    }

    if (config->rows < GAME_MIN_ROWS) {
        config->rows = GAME_MIN_ROWS;
    } else if (config->rows > GAME_MAX_ROWS) {
        config->rows = GAME_MAX_ROWS;
    }

    if (config->columns < GAME_MIN_COLUMNS) {
        config->columns = GAME_MIN_COLUMNS;
    } else if (config->columns > GAME_MAX_COLUMNS) {
        config->columns = GAME_MAX_COLUMNS;
    }

    if (config->mode < GAME_WITH_WALLS || config->mode > GAME_INFINITE) {
        config->mode = GAME_WITH_WALLS;
    }

    if (config->apple_count < 1) {
        config->apple_count = 1;
    }
    if (config->trap_count < 0) {
        config->trap_count = 0;
    }

    if (config->speed_coeff < 1) {
        config->speed_coeff = 1;
    } else if (config->speed_coeff > 20) {
        config->speed_coeff = 20;
    }

    if (config->snake_length < 3) {
        config->snake_length = 3;
    }

    if (config->mode == GAME_WITH_PORTALS) {
        int portal_left = config->columns / 2 - PORTAL_HALF_EXTENT;
        max_snake = portal_left - 2;
    } else {
        max_snake = config->columns - 4;
    }

    if (max_snake < 3) {
        max_snake = 3;
    }
    if (config->snake_length > max_snake) {
        config->snake_length = max_snake;
    }
}

Game *game_create(const GameConfig *config)
{
    GameConfig safe;
    Game *game;

    if (config == NULL) {
        return NULL;
    }

    safe = *config;
    game_config_sanitize(&safe);

    game = calloc(1, sizeof(*game));
    if (game == NULL) {
        return NULL;
    }

    snake_init(&game->snake);
    game->mode = safe.mode;
    game->apples_per_level = safe.apple_count;
    game->trap_count = safe.trap_count;
    game->level = 1;
    game->tick_us = GAME_DEFAULT_TICK_US;
    game->speed_coeff = safe.speed_coeff;
    game->initial_snake_length = safe.snake_length;
    game->end_reason = GAME_END_NONE;
    game->random_state = safe.random_seed == 0U ? 0xA341316CU : safe.random_seed;

    if (!board_init(&game->board, safe.rows, safe.columns) || !level_reset(game)) {
        game_destroy(game);
        return NULL;
    }

    return game;
}

void game_destroy(Game *game)
{
    if (game == NULL) {
        return;
    }

    snake_clear(&game->snake, NULL);
    board_destroy(&game->board);
    free(game);
}

void game_start(Game *game)
{
    if (game != NULL && game->end_reason == GAME_END_NONE) {
        game->started = true;
    }
}

void game_request_quit(Game *game)
{
    game_end(game, GAME_END_QUIT);
}

bool game_is_over(const Game *game)
{
    return game == NULL || game->end_reason != GAME_END_NONE;
}

double game_tick_seconds(const Game *game)
{
    if (game == NULL) {
        return 0.0;
    }

    return (double)game->tick_us / 1000000.0;
}

void direction_try_update(Direction *direction, int row, int column)
{
    if (direction == NULL || abs(row) + abs(column) != 1) {
        return;
    }

    if (direction->row + row == 0 && direction->column + column == 0) {
        return;
    }

    direction->row = row;
    direction->column = column;
}

static bool is_blocked(const Game *game, int row, int column)
{
    CellType cell = board_get(&game->board, row, column);

    if (cell == CELL_WALL || cell == CELL_TRAP) {
        return true;
    }

    return cell == CELL_SNAKE && !snake_tail_is(&game->snake, row, column);
}

GameEvent game_step(Game *game, Direction *direction)
{
    int target_row;
    int target_column;
    int destination_row;
    int destination_column;
    CellType entry;
    bool ate_apple;
    GameEvent event;

    if (game == NULL || direction == NULL || game_is_over(game) || !game->started) {
        return GAME_EVENT_NONE;
    }

    if (game->snake.head == NULL) {
        game_end(game, GAME_END_ERROR);
        return GAME_EVENT_GAME_OVER;
    }

    target_row = game->snake.head->row + direction->row;
    target_column = game->snake.head->column + direction->column;

    if (!board_in_bounds(&game->board, target_row, target_column) ||
        is_blocked(game, target_row, target_column)) {
        game_end(game, GAME_END_COLLISION);
        return GAME_EVENT_GAME_OVER;
    }

    entry = board_get(&game->board, target_row, target_column);
    ate_apple = entry == CELL_APPLE;
    destination_row = target_row;
    destination_column = target_column;

    if (!level_resolve_portal(game, entry, &destination_row, &destination_column) ||
        is_blocked(game, destination_row, destination_column)) {
        game_end(game, GAME_END_COLLISION);
        return GAME_EVENT_GAME_OVER;
    }

    if (!ate_apple && !snake_remove_tail(&game->snake, &game->board)) {
        game_end(game, GAME_END_ERROR);
        return GAME_EVENT_GAME_OVER;
    }

    if (!snake_append_head(&game->snake, &game->board, destination_row, destination_column)) {
        game_end(game, GAME_END_ERROR);
        return GAME_EVENT_GAME_OVER;
    }

    event = GAME_EVENT_MOVED;
    if (ate_apple) {
        game->score += 5;
        game->apples_remaining--;
        event = (GameEvent)(event | GAME_EVENT_APPLE);
    }

    if (game->apples_remaining == 0) {
        event = (GameEvent)(event | level_advance(game, direction));
    }

    return event;
}
