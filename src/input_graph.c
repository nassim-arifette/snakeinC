#include "input.h"

#include <graph.h>

InputCommand input_poll(void)
{
    int key;

    if (!ToucheEnAttente()) {
        return INPUT_NONE;
    }

    key = Touche();

    switch (key) {
    case XK_Up:
        return INPUT_UP;
    case XK_Down:
        return INPUT_DOWN;
    case XK_Left:
        return INPUT_LEFT;
    case XK_Right:
        return INPUT_RIGHT;
    case XK_Escape:
        return INPUT_QUIT;
    default:
        return INPUT_START;
    }
}
