#ifndef SCORE_H
#define SCORE_H

#include <stdbool.h>

int score_load_highscore(const char *path);
bool score_append(const char *path, int score);

#endif
