#ifndef SNAKE_INPUT_H
#define SNAKE_INPUT_H

typedef enum InputCommand {
    INPUT_NONE = 0,
    INPUT_START,
    INPUT_UP,
    INPUT_DOWN,
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_BACK
} InputCommand;

InputCommand input_poll(void);

#endif
