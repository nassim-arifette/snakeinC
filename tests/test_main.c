#include "test.h"

#include <stdio.h>

int main(void)
{
    test_board_suite();
    test_snake_suite();
    test_game_suite();

    puts("all core tests: ok");
    return 0;
}
