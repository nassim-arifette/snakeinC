#include "game.h"

#include <stdlib.h>

#define GAME_HUD_ROWS 7
#define GAME_BORDER_COLUMNS 2
#define PORTAL_EXIT_OFFSET 18

static void game_end(Game *game, GameEndReason reason)
{
    if (game == NULL || game->end_reason != GAME_END_NONE) {
        return;
    }

    game->end_reason = reason;
}

void game_config_default(GameConfig *config)
{
    if (config == NULL) {
        return;
    }

    config->rows = 40;
    config->columns = 60;
    config->mode = GAME_WITH_WALLS;
    config->apple_count = 5;
    config->trap_count = 0;
    config->speed_coeff = 3;
    config->snake_length = 10;
}

void game_config_sanitize(GameConfig *config)
{
    int max_snake;

    if (config == NULL) {
        return;
    }

    if (config->rows < 20) {
        config->rows = 20;
    } else if (config->rows > 100) {
        config->rows = 100;
    }

    if (config->columns < 30) {
        config->columns = 30;
    } else if (config->columns > 150) {
        config->columns = 150;
    }

    if (config->mode < GAME_WITH_WALLS || config->mode > GAME_INFINITE_ALT) {
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
        max_snake = config->columns / 4 - 4;
    } else {
        max_snake = config->columns / 2;
    }

    if (max_snake < 3) {
        max_snake = 3;
    }
    if (config->snake_length > max_snake) {
        config->snake_length = max_snake;
    }
}

static void reset_board_layout(Game *game)
{
    int row;
    int column;
    Board *board = &game->board;

    board_fill(board, CELL_WALL);

    for (row = 0; row < board->rows; ++row) {
        for (column = 0; column < board->columns; ++column) {
            if (row == board->rows / 2 - 6 &&
                column >= board->columns / 2 - 5 &&
                column <= board->columns / 2 + 4) {
                board_set(board, row, column,
                          game->mode == GAME_WITH_PORTALS ? CELL_PORTAL_TOP : CELL_EMPTY);
            } else if (row == board->rows / 2 + 11 &&
                       column >= board->columns / 2 - 5 &&
                       column <= board->columns / 2 + 4) {
                board_set(board, row, column,
                          game->mode == GAME_WITH_PORTALS ? CELL_PORTAL_BOTTOM : CELL_EMPTY);
            } else if (column == board->columns / 2 - 9 &&
                       row >= board->rows / 2 - 2 &&
                       row <= board->rows / 2 + 7) {
                board_set(board, row, column,
                          game->mode == GAME_WITH_PORTALS ? CELL_PORTAL_LEFT : CELL_EMPTY);
            } else if (column == board->columns / 2 + 8 &&
                       row >= board->rows / 2 - 2 &&
                       row <= board->rows / 2 + 7) {
                board_set(board, row, column,
                          game->mode == GAME_WITH_PORTALS ? CELL_PORTAL_RIGHT : CELL_EMPTY);
            } else if (column == 0 &&
                       row >= board->rows / 2 - 2 &&
                       row <= board->rows / 2 + 7) {
                board_set(board, row, column,
                          game->mode == GAME_WITH_PORTALS ? CELL_PORTAL_WRAP_LEFT : CELL_WALL);
            } else if (column == board->columns - 1 &&
                       row >= board->rows / 2 - 2 &&
                       row <= board->rows / 2 + 7) {
                board_set(board, row, column,
                          game->mode == GAME_WITH_PORTALS ? CELL_PORTAL_WRAP_RIGHT : CELL_WALL);
            } else if (row >= board->rows / 2 - 6 &&
                       row <= board->rows / 2 + 11 &&
                       column >= board->columns / 2 - 9 &&
                       column <= board->columns / 2 + 8) {
                board_set(board, row, column,
                          game->mode == GAME_WITH_PORTALS ? CELL_WALL : CELL_EMPTY);
            } else if (row > 5 &&
                       row != board->rows - 1 &&
                       column != 0 &&
                       column != board->columns - 1) {
                board_set(board, row, column, CELL_EMPTY);
            } else {
                board_set(board, row, column, CELL_WALL);
            }
        }
    }
}

static bool spawn_snake(Game *game)
{
    int row;
    int start_column;
    int area_end;
    int length;

    row = game->board.rows / 2 + 2;

    if (game->mode == GAME_WITH_PORTALS) {
        int spawn_columns = game->board.columns / 2 - 5;
        start_column = spawn_columns / 2 - 1;
        area_end = game->board.columns / 2 - 9;
    } else {
        start_column = game->board.columns / 2 - 1;
        area_end = game->board.columns - 1;
    }

    if (start_column < 1) {
        start_column = 1;
    }

    length = game->initial_snake_length;
    if (length > area_end - start_column) {
        length = area_end - start_column;
    }

    if (row <= 0 || row >= game->board.rows - 1 || length < 3) {
        return false;
    }

    return snake_place_horizontal(&game->snake, &game->board, row, start_column, length);
}

static int count_spawn_cells(const Game *game)
{
    int row;
    int column;
    int count = 0;
    int excluded_row = game->board.rows / 2 + 2;

    for (row = 0; row < game->board.rows; ++row) {
        for (column = 0; column < game->board.columns; ++column) {
            if (row != excluded_row &&
                board_get(&game->board, row, column) == CELL_EMPTY) {
                ++count;
            }
        }
    }

    return count;
}

static bool place_random_cell(Game *game, CellType cell)
{
    int available = count_spawn_cells(game);
    int selected;
    int row;
    int column;
    int seen = 0;
    int excluded_row = game->board.rows / 2 + 2;

    if (available <= 0) {
        return false;
    }

    selected = rand() % available;

    for (row = 0; row < game->board.rows; ++row) {
        for (column = 0; column < game->board.columns; ++column) {
            if (row == excluded_row ||
                board_get(&game->board, row, column) != CELL_EMPTY) {
                continue;
            }

            if (seen == selected) {
                return board_set(&game->board, row, column, cell);
            }
            ++seen;
        }
    }

    return false;
}

static int spawn_cells(Game *game, CellType cell, int requested)
{
    int placed = 0;
    int available;

    if (requested < 0) {
        requested = 0;
    }

    available = count_spawn_cells(game);
    if (requested > available) {
        requested = available;
    }

    while (placed < requested && place_random_cell(game, cell)) {
        ++placed;
    }

    return placed;
}

static void spawn_apples(Game *game, int count)
{
    if (game->mode == GAME_INFINITE) {
        count = 1;
    }

    game->apples_remaining = spawn_cells(game, CELL_APPLE, count);
}

static void spawn_traps(Game *game, int count)
{
    (void)spawn_cells(game, CELL_TRAP, count);
}

static bool reset_level(Game *game)
{
    snake_clear(&game->snake, NULL);
    reset_board_layout(game);

    if (!spawn_snake(game)) {
        return false;
    }

    spawn_apples(game, game->apples_per_level);
    spawn_traps(game, game->trap_count);
    game->started = false;
    return game->apples_remaining > 0;
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

    if (!board_init(&game->board,
                    safe.rows + GAME_HUD_ROWS,
                    safe.columns + GAME_BORDER_COLUMNS)) {
        free(game);
        return NULL;
    }

    if (!reset_level(game)) {
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

void direction_try_update(Direction *direction, int row, int column)
{
    if (direction == NULL || abs(row) + abs(column) != 1) {
        return;
    }

    if (direction->row + row == 0 &&
        direction->column + column == 0) {
        return;
    }

    direction->row = row;
    direction->column = column;
}

static bool portal_destination(CellType entry, int *row, int *column, const Game *game)
{
    switch (entry) {
    case CELL_PORTAL_TOP:
        *row += PORTAL_EXIT_OFFSET;
        break;
    case CELL_PORTAL_BOTTOM:
        *row -= PORTAL_EXIT_OFFSET;
        break;
    case CELL_PORTAL_LEFT:
        *column += PORTAL_EXIT_OFFSET;
        break;
    case CELL_PORTAL_RIGHT:
        *column -= PORTAL_EXIT_OFFSET;
        break;
    case CELL_PORTAL_WRAP_LEFT:
        *column = game->board.columns - 2;
        break;
    case CELL_PORTAL_WRAP_RIGHT:
        *column = 1;
        break;
    default:
        break;
    }

    return board_in_bounds(&game->board, *row, *column);
}

static bool is_blocked(const Game *game, int row, int column)
{
    CellType cell = board_get(&game->board, row, column);

    if (cell == CELL_WALL || cell == CELL_TRAP) {
        return true;
    }

    if (cell == CELL_SNAKE && !snake_tail_is(&game->snake, row, column)) {
        return true;
    }

    return false;
}

static GameEvent advance_level(Game *game, Direction *direction)
{
    GameEvent event = GAME_EVENT_LEVEL_UP;

    game->level++;

    if (game->mode == GAME_WITH_WALLS || game->mode == GAME_WITH_PORTALS) {
        unsigned int speed_step = (unsigned int)(1500 * game->speed_coeff);

        game->trap_count++;
        game->apples_per_level++;

        if (game->tick_us <= GAME_MIN_TICK_US + speed_step) {
            game->tick_us = GAME_MIN_TICK_US;
        } else {
            game->tick_us -= speed_step;
        }

        direction->row = 0;
        direction->column = 1;

        if (!reset_level(game)) {
            game_end(game, GAME_END_ERROR);
            event = (GameEvent)(event | GAME_EVENT_GAME_OVER);
        }
        return event;
    }

    spawn_apples(game, 4 + game->level);
    spawn_traps(game, game->level - 1);

    if (game->apples_remaining == 0) {
        game_end(game, GAME_END_ERROR);
        event = (GameEvent)(event | GAME_EVENT_GAME_OVER);
    }

    return event;
}

GameEvent game_step(Game *game, Direction *direction)
{
    int target_row;
    int target_column;
    int destination_row;
    int destination_column;
    CellType entry;
    bool ate_apple;
    GameEvent event = GAME_EVENT_NONE;

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

    if (!portal_destination(entry, &destination_row, &destination_column, game) ||
        is_blocked(game, destination_row, destination_column)) {
        game_end(game, GAME_END_COLLISION);
        return GAME_EVENT_GAME_OVER;
    }

    if (!ate_apple && !snake_remove_tail(&game->snake, &game->board)) {
        game_end(game, GAME_END_ERROR);
        return GAME_EVENT_GAME_OVER;
    }

    if (!snake_append_head(&game->snake,
                           &game->board,
                           destination_row,
                           destination_column)) {
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
        event = (GameEvent)(event | advance_level(game, direction));
    }

    return event;
}
