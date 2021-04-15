#include "Snake.h"

static void clearSnakeNodes(Serpent *snake)
{
    Noeud *node;

    if (snake == NULL) {
        return;
    }

    node = snake->queue;
    while (node != NULL) {
        Noeud *next = node->suiv;
        free(node);
        node = next;
    }

    snake->queue = NULL;
    snake->tete = NULL;
    snake->taille = 0;
}

Serpent *SpawnSerpent(Jeux *game)
{
    Serpent *snake;
    Noeud *previous = NULL;
    int row;
    int start_y;
    int area_end;
    int length;
    int i;

    if (game == NULL || game->plat == NULL) {
        return NULL;
    }

    snake = calloc(1, sizeof(*snake));
    if (snake == NULL) {
        return NULL;
    }

    row = game->ligne / 2 + 2;

    if (game->gamemode == GAME_WITH_PORTALS) {
        int spawn_columns = game->colonne / 2 - 5;
        start_y = spawn_columns / 2 - 1;
        area_end = game->colonne / 2 - 9;
    } else {
        start_y = game->colonne / 2 - 1;
        area_end = game->colonne - 1;
    }

    if (start_y < 1) {
        start_y = 1;
    }

    length = game->taille;
    if (length < 3) {
        length = 3;
    }
    if (length > area_end - start_y) {
        length = area_end - start_y;
    }

    if (row <= 0 || row >= game->ligne - 1 || length < 1) {
        free(snake);
        return NULL;
    }

    for (i = 0; i < length; ++i) {
        Noeud *node = malloc(sizeof(*node));
        int y = start_y + i;

        if (node == NULL) {
            clearSnakeNodes(snake);
            free(snake);
            return NULL;
        }

        node->x = row;
        node->y = y;
        node->suiv = NULL;

        if (snake->queue == NULL) {
            snake->queue = node;
        } else {
            previous->suiv = node;
        }

        previous = node;
        snake->tete = node;
        snake->taille++;
        game->plat[row][y] = CELL_SNAKE;
    }

    return snake;
}

int ResetSerpent(Serpent *snake, Jeux *game)
{
    Serpent *fresh;

    if (snake == NULL) {
        return 0;
    }

    clearSnakeNodes(snake);
    fresh = SpawnSerpent(game);
    if (fresh == NULL) {
        return 0;
    }

    snake->queue = fresh->queue;
    snake->tete = fresh->tete;
    snake->taille = fresh->taille;
    free(fresh);

    return 1;
}

void destroySnake(Serpent *snake)
{
    if (snake == NULL) {
        return;
    }

    clearSnakeNodes(snake);
    free(snake);
}

static int countEmptyCells(const Jeux *game)
{
    int count = 0;
    int i;
    int j;

    for (i = 0; i < game->ligne; ++i) {
        for (j = 0; j < game->colonne; ++j) {
            if (game->plat[i][j] == CELL_EMPTY && i != game->ligne / 2 + 2) {
                ++count;
            }
        }
    }

    return count;
}

void generateFeed(Jeux *game, int feed)
{
    int count = 0;
    int available;

    if (game == NULL) {
        return;
    }

    if (game->gamemode == GAME_INFINITE) {
        feed = 1;
    }
    if (feed < 0) {
        feed = 0;
    }

    available = countEmptyCells(game);
    if (feed > available) {
        feed = available;
    }

    while (count < feed) {
        int i = rand() % game->ligne;
        int j = rand() % game->colonne;

        if (game->plat[i][j] == CELL_EMPTY && i != game->ligne / 2 + 2) {
            game->plat[i][j] = CELL_APPLE;
            if (game->gamemode == GAME_INFINITE) {
                AfficherSprite(1, CELL_SIZE * j, CELL_SIZE * i);
            }
            ++count;
        }
    }

    game->nbpommes = feed;
}

void generateTrap(Jeux *game, int trap)
{
    int count = 0;
    int available;

    if (game == NULL) {
        return;
    }
    if (trap < 0) {
        trap = 0;
    }

    available = countEmptyCells(game);
    if (trap > available) {
        trap = available;
    }

    while (count < trap) {
        int i = rand() % game->ligne;
        int j = rand() % game->colonne;

        if (game->plat[i][j] == CELL_EMPTY && i != game->ligne / 2 + 2) {
            game->plat[i][j] = CELL_TRAP;
            if (game->gamemode == GAME_INFINITE) {
                AfficherSprite(5, CELL_SIZE * j, CELL_SIZE * i);
            }
            ++count;
        }
    }
}
