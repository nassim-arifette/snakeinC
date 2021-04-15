#include "Snake.h"

static void updateDirection(Direction *direction, int key)
{
    Direction next = *direction;

    if (direction->ligne == 0) {
        if (key == XK_Up) {
            next.ligne = -1;
            next.colonne = 0;
        } else if (key == XK_Down) {
            next.ligne = 1;
            next.colonne = 0;
        }
    } else if (direction->colonne == 0) {
        if (key == XK_Left) {
            next.colonne = -1;
            next.ligne = 0;
        } else if (key == XK_Right) {
            next.colonne = 1;
            next.ligne = 0;
        }
    }

    *direction = next;
}

void gererEvenement(Serpent *snake, Jeux *game, Direction *direction)
{
    int key = 0;
    int apples_before;

    if (snake == NULL || game == NULL || direction == NULL || game->end) {
        return;
    }

    if (ToucheEnAttente()) {
        key = Touche();
    }

    if (key == XK_Escape) {
        game->end = 1;
        return;
    }

    if (!game->start) {
        if (key == 0) {
            print(game);
            return;
        }
        game->start = 1;
    }

    updateDirection(direction, key);
    print(game);

    apples_before = game->nbpommes;
    delqueue(snake, game);
    addtete(snake,
            snake->tete->x + direction->ligne,
            snake->tete->y + direction->colonne,
            game,
            direction);

    if (game->end) {
        return;
    }

    if (apples_before != game->nbpommes) {
        game->tour += 2;
    }
    if (game->tour > 0) {
        game->tour--;
    }

    if (game->nbpommes == 0 &&
        (game->gamemode == GAME_WITH_WALLS || game->gamemode == GAME_WITH_PORTALS)) {
        int speed_step = 1500 * game->coeff;

        game->lvl++;
        game->trap++;
        game->nbpommestot++;
        if (game->time - speed_step < MIN_TICK_US) {
            game->time = MIN_TICK_US;
        } else {
            game->time -= speed_step;
        }

        direction->ligne = 0;
        direction->colonne = 1;
        lvlup(game, snake);
        return;
    }

    if (game->nbpommes == 0 &&
        (game->gamemode == GAME_INFINITE || game->gamemode == GAME_INFINITE_ALT)) {
        game->lvl++;
        generateFeed(game, 4 + game->lvl);
        generateTrap(game, game->lvl - 1);
    }
}
