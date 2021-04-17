#include "score.h"

#include <stdio.h>

int score_load_highscore(const char *path)
{
    FILE *file;
    int value;
    int highscore = 0;

    if (path == NULL) {
        return 0;
    }

    file = fopen(path, "r");
    if (file == NULL) {
        return 0;
    }

    while (fscanf(file, "%d", &value) == 1) {
        if (value > highscore) {
            highscore = value;
        }
    }

    fclose(file);
    return highscore;
}

bool score_append(const char *path, int score)
{
    FILE *file;

    if (path == NULL) {
        return false;
    }

    file = fopen(path, "a");
    if (file == NULL) {
        return false;
    }

    if (fprintf(file, "%d\n", score) < 0) {
        fclose(file);
        return false;
    }

    return fclose(file) == 0;
}
