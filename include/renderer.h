#ifndef RENDERER_H
#define RENDERER_H

#include "game.h"

void renderer_open(const Game *game);
void renderer_draw(const Game *game, int highscore);
void renderer_close(void);

#endif
