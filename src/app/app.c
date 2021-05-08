#include "snake/app.h"

#include "snake/game.h"
#include "snake/input.h"
#include "snake/menu.h"
#include "snake/renderer.h"
#include "snake/score.h"

#include <limits.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>

#define INITIAL_WINDOW_WIDTH 720
#define INITIAL_WINDOW_HEIGHT 760
#define TARGET_FPS 60
#define MAX_STEPS_PER_FRAME 8

typedef struct SessionResult {
    int score;
    bool close_requested;
} SessionResult;

static void apply_input(Game *game, Direction *direction, InputCommand command)
{
    switch (command) {
    case INPUT_UP:
        direction_try_update(direction, -1, 0);
        game_start(game);
        break;
    case INPUT_DOWN:
        direction_try_update(direction, 1, 0);
        game_start(game);
        break;
    case INPUT_LEFT:
        direction_try_update(direction, 0, -1);
        game_start(game);
        break;
    case INPUT_RIGHT:
        direction_try_update(direction, 0, 1);
        game_start(game);
        break;
    case INPUT_START:
        game_start(game);
        break;
    case INPUT_BACK:
        game_request_quit(game);
        break;
    case INPUT_NONE:
    default:
        break;
    }
}

static SessionResult run_game(const GameConfig *config, int *highscore)
{
    SessionResult result = {0};
    Direction direction = {0, 1};
    Game *game = game_create(config);
    double accumulator = 0.0;

    if (game == NULL) {
        TraceLog(LOG_ERROR, "Impossible d'initialiser la partie");
        return result;
    }

    renderer_prepare_game(game);

    while (!game_is_over(game)) {
        InputCommand command;
        int step_count = 0;

        if (WindowShouldClose()) {
            result.close_requested = true;
            game_request_quit(game);
            break;
        }

        command = input_poll();
        apply_input(game, &direction, command);

        if (game_is_over(game)) {
            break;
        }

        if (game->started) {
            accumulator += (double)GetFrameTime();
            while (step_count < MAX_STEPS_PER_FRAME && !game_is_over(game)) {
                double tick_seconds = game_tick_seconds(game);
                GameEvent event;

                if (accumulator < tick_seconds) {
                    break;
                }

                event = game_step(game, &direction);
                accumulator -= tick_seconds;
                ++step_count;

                if ((event & GAME_EVENT_LEVEL_UP) != 0 || !game->started) {
                    accumulator = 0.0;
                    break;
                }
            }

            if (step_count == MAX_STEPS_PER_FRAME) {
                accumulator = 0.0;
            }
        } else {
            accumulator = 0.0;
        }

        if (game->score > *highscore) {
            *highscore = game->score;
        }
        renderer_draw_game(game, *highscore);
    }

    result.score = game->score;
    if (game->score > *highscore) {
        *highscore = game->score;
    }

    game_destroy(game);
    return result;
}

int app_run(void)
{
    GameConfig config;
    int last_score = 0;
    int highscore;
    int persisted_highscore;

    InitWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "Snake");
    SetExitKey(KEY_NULL);
    SetTargetFPS(TARGET_FPS);

    if (!renderer_init()) {
        TraceLog(LOG_WARNING, "Certains assets n'ont pas pu etre charges");
    }

    highscore = score_load_highscore();
    persisted_highscore = highscore;
    game_config_default(&config);

    while (!WindowShouldClose()) {
        SessionResult session;

        if (!menu_run(last_score, highscore, &config)) {
            break;
        }

        config.random_seed = (uint32_t)GetRandomValue(1, INT_MAX);
        session = run_game(&config, &highscore);
        last_score = session.score;

        if (highscore > persisted_highscore) {
            if (score_save_highscore(highscore)) {
                persisted_highscore = highscore;
            } else {
                TraceLog(LOG_WARNING, "Impossible d'enregistrer le meilleur score");
            }
        }

        if (session.close_requested) {
            break;
        }
    }

    renderer_shutdown();
    CloseWindow();
    return 0;
}
