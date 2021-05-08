#ifndef SNAKE_INTERNAL_ASSETS_H
#define SNAKE_INTERNAL_ASSETS_H

#include <raylib.h>
#include <stdbool.h>

typedef struct AppAssets {
    Texture2D apple;
    Texture2D trophy;
    Texture2D portal;
    Texture2D bomb;
    Texture2D menu_background;
} AppAssets;

bool assets_load(void);
void assets_unload(void);
const AppAssets *assets_get(void);

#endif
