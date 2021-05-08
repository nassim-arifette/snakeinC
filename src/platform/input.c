#include "snake/input.h"

#include <raylib.h>

InputCommand input_poll(void)
{
    int key = GetKeyPressed();

    switch (key) {
    case KEY_UP:
    case KEY_W:
        return INPUT_UP;
    case KEY_DOWN:
    case KEY_S:
        return INPUT_DOWN;
    case KEY_LEFT:
    case KEY_A:
        return INPUT_LEFT;
    case KEY_RIGHT:
    case KEY_D:
        return INPUT_RIGHT;
    case KEY_SPACE:
    case KEY_ENTER:
        return INPUT_START;
    case KEY_ESCAPE:
        return INPUT_BACK;
    default:
        return INPUT_NONE;
    }
}
