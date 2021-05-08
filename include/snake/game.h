#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include "snake/board.h"
#include "snake/snake.h"

#include <stdbool.h>
#include <stdint.h>

#define GAME_MIN_TICK_US 10000U
#define GAME_DEFAULT_TICK_US 60000U

typedef enum GameMode {
    GAME_WITH_WALLS = 1,
    GAME_WITH_PORTALS,
    GAME_INFINITE
} GameMode;

typedef enum GameEndReason {
    GAME_END_NONE = 0,
    GAME_END_COLLISION,
    GAME_END_QUIT,
    GAME_END_ERROR
} GameEndReason;

typedef enum GameEvent {
    GAME_EVENT_NONE = 0,
    GAME_EVENT_MOVED = 1 << 0,
    GAME_EVENT_APPLE = 1 << 1,
    GAME_EVENT_LEVEL_UP = 1 << 2,
    GAME_EVENT_GAME_OVER = 1 << 3
} GameEvent;

typedef struct Direction {
    int row;
    int column;
} Direction;

typedef struct GameConfig {
    int rows;
    int columns;
    GameMode mode;
    int apple_count;
    int trap_count;
    int speed_coeff;
    int snake_length;
    uint32_t random_seed;
} GameConfig;

typedef struct Game {
    Board board;
    Snake snake;
    GameMode mode;
    int apples_remaining;
    int apples_per_level;
    int trap_count;
    int level;
    int score;
    bool started;
    GameEndReason end_reason;
    uint32_t tick_us;
    int speed_coeff;
    int initial_snake_length;
    uint32_t random_state;
} Game;

void game_config_default(GameConfig *config);
void game_config_sanitize(GameConfig *config);
Game *game_create(const GameConfig *config);
void game_destroy(Game *game);
void game_start(Game *game);
void game_request_quit(Game *game);
bool game_is_over(const Game *game);
double game_tick_seconds(const Game *game);
void direction_try_update(Direction *direction, int row, int column);
GameEvent game_step(Game *game, Direction *direction);

#endif
