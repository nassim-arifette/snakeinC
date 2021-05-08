#include "snake/score.h"

#include <raylib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SCORE_DIRECTORY "data"
#define SCORE_PATH "data/highscore.txt"

int score_load_highscore(void)
{
    char *text;
    char *end;
    long value;
    bool valid;

    if (!FileExists(SCORE_PATH)) {
        return 0;
    }

    text = LoadFileText(SCORE_PATH);
    if (text == NULL) {
        return 0;
    }

    value = strtol(text, &end, 10);
    while (*end != '\0' && isspace((unsigned char)*end) != 0) {
        ++end;
    }
    valid = end != text && *end == '\0' && value >= 0L && value <= 2147483647L;
    UnloadFileText(text);
    return valid ? (int)value : 0;
}

bool score_save_highscore(int highscore)
{
    char buffer[32];

    if (highscore < 0) {
        return false;
    }

    if (!DirectoryExists(SCORE_DIRECTORY) && MakeDirectory(SCORE_DIRECTORY) != 0) {
        return false;
    }

    (void)snprintf(buffer, sizeof(buffer), "%d\n", highscore);
    return SaveFileText(SCORE_PATH, buffer);
}
