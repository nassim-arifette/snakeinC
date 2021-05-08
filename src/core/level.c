#include "level.h"

#include <stdint.h>

#define PORTAL_BLOCK_SIZE 18
#define PORTAL_HALF_EXTENT 9
#define PORTAL_STRIP_HALF 5
#define PORTAL_EXIT_OFFSET 18

static uint32_t random_next(Game *game)
{
    uint32_t value = game->random_state;

    if (value == 0U) {
        value = 0xA341316CU;
    }

    value ^= value << 13;
    value ^= value >> 17;
    value ^= value << 5;
    game->random_state = value;
    return value;
}

static int random_bounded(Game *game, int upper_bound)
{
    if (upper_bound <= 1) {
        return 0;
    }

    return (int)(random_next(game) % (uint32_t)upper_bound);
}

static void build_outer_walls(Board *board)
{
    int row;
    int column;

    board_fill(board, CELL_EMPTY);

    for (column = 0; column < board->columns; ++column) {
        (void)board_set(board, 0, column, CELL_WALL);
        (void)board_set(board, board->rows - 1, column, CELL_WALL);
    }

    for (row = 0; row < board->rows; ++row) {
        (void)board_set(board, row, 0, CELL_WALL);
        (void)board_set(board, row, board->columns - 1, CELL_WALL);
    }
}

static void build_portal_layout(Board *board)
{
    int center_row = board->rows / 2;
    int center_column = board->columns / 2;
    int top = center_row - PORTAL_HALF_EXTENT;
    int bottom = top + PORTAL_BLOCK_SIZE - 1;
    int left = center_column - PORTAL_HALF_EXTENT;
    int right = left + PORTAL_BLOCK_SIZE - 1;
    int strip_row_start = center_row - PORTAL_STRIP_HALF;
    int strip_row_end = center_row + PORTAL_STRIP_HALF - 1;
    int strip_column_start = center_column - PORTAL_STRIP_HALF;
    int strip_column_end = center_column + PORTAL_STRIP_HALF - 1;
    int row;
    int column;

    for (row = top; row <= bottom; ++row) {
        for (column = left; column <= right; ++column) {
            (void)board_set(board, row, column, CELL_WALL);
        }
    }

    for (column = strip_column_start; column <= strip_column_end; ++column) {
        (void)board_set(board, top, column, CELL_PORTAL_TOP);
        (void)board_set(board, bottom, column, CELL_PORTAL_BOTTOM);
    }

    for (row = strip_row_start; row <= strip_row_end; ++row) {
        (void)board_set(board, row, left, CELL_PORTAL_LEFT);
        (void)board_set(board, row, right, CELL_PORTAL_RIGHT);
        (void)board_set(board, row, 0, CELL_PORTAL_WRAP_LEFT);
        (void)board_set(board, row, board->columns - 1, CELL_PORTAL_WRAP_RIGHT);
    }
}

static void reset_board_layout(Game *game)
{
    build_outer_walls(&game->board);
    if (game->mode == GAME_WITH_PORTALS) {
        build_portal_layout(&game->board);
    }
}

static bool spawn_snake(Game *game)
{
    int row = game->board.rows / 2;
    int start_column;

    if (game->mode == GAME_WITH_PORTALS) {
        int portal_left = game->board.columns / 2 - PORTAL_HALF_EXTENT;
        start_column = portal_left - game->initial_snake_length - 1;
    } else {
        start_column = game->board.columns / 2 - game->initial_snake_length / 2;
    }

    if (start_column < 1) {
        start_column = 1;
    }

    return snake_place_horizontal(&game->snake,
                                  &game->board,
                                  row,
                                  start_column,
                                  game->initial_snake_length);
}

static int count_empty_cells(const Game *game)
{
    int row;
    int column;
    int count = 0;

    for (row = 0; row < game->board.rows; ++row) {
        for (column = 0; column < game->board.columns; ++column) {
            if (board_get(&game->board, row, column) == CELL_EMPTY) {
                ++count;
            }
        }
    }

    return count;
}

static bool place_random_cell(Game *game, CellType cell)
{
    int available = count_empty_cells(game);
    int selected;
    int seen = 0;
    int row;
    int column;

    if (available <= 0) {
        return false;
    }

    selected = random_bounded(game, available);

    for (row = 0; row < game->board.rows; ++row) {
        for (column = 0; column < game->board.columns; ++column) {
            if (board_get(&game->board, row, column) != CELL_EMPTY) {
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
    int available;
    int placed = 0;

    if (requested <= 0) {
        return 0;
    }

    available = count_empty_cells(game);
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

bool level_reset(Game *game)
{
    if (game == NULL) {
        return false;
    }

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

GameEvent level_advance(Game *game, Direction *direction)
{
    GameEvent event = GAME_EVENT_LEVEL_UP;

    if (game == NULL || direction == NULL) {
        return GAME_EVENT_NONE;
    }

    game->level++;

    if (game->mode == GAME_WITH_WALLS || game->mode == GAME_WITH_PORTALS) {
        uint32_t speed_step = (uint32_t)(1500 * game->speed_coeff);

        game->trap_count++;
        game->apples_per_level++;
        game->tick_us = game->tick_us <= GAME_MIN_TICK_US + speed_step
                            ? GAME_MIN_TICK_US
                            : game->tick_us - speed_step;

        direction->row = 0;
        direction->column = 1;

        if (!level_reset(game)) {
            game->end_reason = GAME_END_ERROR;
            event = (GameEvent)(event | GAME_EVENT_GAME_OVER);
        }
        return event;
    }

    spawn_apples(game, 1);
    spawn_traps(game, game->level - 1);
    if (game->apples_remaining == 0) {
        game->end_reason = GAME_END_ERROR;
        event = (GameEvent)(event | GAME_EVENT_GAME_OVER);
    }

    return event;
}

bool level_resolve_portal(const Game *game, CellType entry, int *row, int *column)
{
    if (game == NULL || row == NULL || column == NULL) {
        return false;
    }

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
