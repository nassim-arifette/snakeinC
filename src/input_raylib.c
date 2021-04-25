#include "input.h"

#include <raylib.h>

InputCommand input_poll(void)
{
    int key;

    if (WindowShouldClose()) {
        return INPUT_QUIT;
    }

    key = GetKeyPressed();

    switch (key) {
    case KEY_UP:
        return INPUT_UP;
    case KEY_DOWN:
        return INPUT_DOWN;
    case KEY_LEFT:
        return INPUT_LEFT;
    case KEY_RIGHT:
        return INPUT_RIGHT;
    case KEY_ESCAPE:
        return INPUT_QUIT;
    case 0:
        return INPUT_NONE;
    default:
        return INPUT_START;
    }
}
