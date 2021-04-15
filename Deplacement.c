#include "Snake.h"

static void endGame(Jeux *game)
{
    printf("Le score final: %d\n", game->score);
    Inscritscore(game);
    Litscore(game);
    game->end = 1;
}

static int appendHead(Serpent *snake, Jeux *game, int x, int y)
{
    Noeud *new_head;

    if (x < 0 || x >= game->ligne || y < 0 || y >= game->colonne) {
        return 0;
    }

    new_head = malloc(sizeof(*new_head));
    if (new_head == NULL) {
        return 0;
    }

    new_head->x = x;
    new_head->y = y;
    new_head->suiv = NULL;

    snake->tete->suiv = new_head;
    snake->tete = new_head;
    snake->taille++;
    game->plat[x][y] = CELL_SNAKE;
    drawsc(snake, 1);

    return 1;
}

void addtete(Serpent *snake, int x, int y, Jeux *game, Direction *direction)
{
    int target_x;
    int target_y;
    int cell;

    if (snake == NULL || snake->tete == NULL || game == NULL || direction == NULL) {
        if (game != NULL) {
            game->end = 1;
        }
        return;
    }

    target_x = snake->tete->x + direction->ligne;
    target_y = snake->tete->y + direction->colonne;

    if (target_x < 0 || target_x >= game->ligne || target_y < 0 || target_y >= game->colonne) {
        endGame(game);
        return;
    }

    cell = game->plat[target_x][target_y];
    if (cell == CELL_WALL || cell == CELL_SNAKE || cell == CELL_TRAP) {
        endGame(game);
        return;
    }

    switch (cell) {
    case CELL_APPLE:
        game->score += 5;
        game->nbpommes--;
        break;
    case CELL_PORTAL_TOP:
        x += 18;
        break;
    case CELL_PORTAL_BOTTOM:
        x -= 18;
        break;
    case CELL_PORTAL_LEFT:
        y += 18;
        break;
    case CELL_PORTAL_RIGHT:
        y -= 18;
        break;
    case CELL_PORTAL_WRAP_LEFT:
        y = game->colonne - 2;
        break;
    case CELL_PORTAL_WRAP_RIGHT:
        y = 1;
        break;
    default:
        break;
    }

    if (!appendHead(snake, game, x, y)) {
        game->end = 1;
    }
}

void delqueue(Serpent *snake, Jeux *game)
{
    Noeud *old_queue;

    if (snake == NULL || snake->queue == NULL || game == NULL || game->tour != 0) {
        return;
    }

    drawsc(snake, 0);
    old_queue = snake->queue;
    game->plat[old_queue->x][old_queue->y] = CELL_EMPTY;
    snake->queue = old_queue->suiv;
    free(old_queue);

    if (snake->taille > 0) {
        snake->taille--;
    }
}
