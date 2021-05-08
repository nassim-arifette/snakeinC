#ifndef SNAKE_RENDERER_H
#define SNAKE_RENDERER_H

#include "snake/game.h"

#include <stdbool.h>

bool renderer_init(void);
void renderer_shutdown(void);
void renderer_prepare_game(const Game *game);
void renderer_draw_game(const Game *game, int highscore);

#endif
