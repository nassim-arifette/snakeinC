#ifndef SNAKE_MENU_H
#define SNAKE_MENU_H

#include "snake/game.h"

#include <stdbool.h>

bool menu_run(int last_score, int highscore, GameConfig *out_config);

#endif
