#ifndef SNAKE_INTERNAL_LEVEL_H
#define SNAKE_INTERNAL_LEVEL_H

#include "snake/game.h"

#include <stdbool.h>

bool level_reset(Game *game);
GameEvent level_advance(Game *game, Direction *direction);
bool level_resolve_portal(const Game *game, CellType entry, int *row, int *column);

#endif
