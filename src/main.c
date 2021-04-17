#include "game.h"
#include "input.h"
#include "menu.h"
#include "renderer.h"
#include "score.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SCORE_FILE "test.txt"

static void apply_input(Game *game, Direction *direction, InputCommand command)
{
    switch (command) {
    case INPUT_UP:
        direction_try_update(direction, -1, 0);
        break;
    case INPUT_DOWN:
        direction_try_update(direction, 1, 0);
        break;
    case INPUT_LEFT:
        direction_try_update(direction, 0, -1);
        break;
    case INPUT_RIGHT:
        direction_try_update(direction, 0, 1);
        break;
    case INPUT_QUIT:
        game_request_quit(game);
        return;
    case INPUT_START:
    case INPUT_NONE:
    default:
        break;
    }

    if (command != INPUT_NONE) {
        game_start(game);
    }
}

static int run_game(const GameConfig *config)
{
    Game *game;
    Direction direction = {0, 1};
    int highscore;
    int final_score;
    GameEndReason end_reason;

    game = game_create(config);
    if (game == NULL) {
        fprintf(stderr, "Impossible d'initialiser la partie.\n");
        return 0;
    }

    highscore = score_load_highscore(SCORE_FILE);
    renderer_open(game);
    renderer_draw(game, highscore);

    while (!game_is_over(game)) {
        InputCommand command;

        usleep((useconds_t)game->tick_us);
        command = input_poll();
        apply_input(game, &direction, command);

        if (game_is_over(game) || !game->started) {
            continue;
        }

        (void)game_step(game, &direction);

        if (game->score > highscore) {
            highscore = game->score;
        }

        renderer_draw(game, highscore);
    }

    final_score = game->score;
    end_reason = game->end_reason;
    renderer_close();

    if (end_reason == GAME_END_COLLISION && !score_append(SCORE_FILE, final_score)) {
        fprintf(stderr, "Impossible d'enregistrer le score.\n");
    }

    game_destroy(game);
    return final_score;
}

int main(void)
{
    GameConfig config;
    int last_score = 0;

    srand((unsigned int)time(NULL));

    while (menu_run(last_score, &config)) {
        last_score = run_game(&config);
    }

    return 0;
}
