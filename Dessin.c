#include "Snake.h"

static couleur boardColor(int row, int column)
{
    if ((row % 2) == (column % 2)) {
        return CouleurParComposante(170, 215, 80);
    }
    return CouleurParComposante(162, 209, 73);
}

void draw(Jeux *game)
{
    int i;
    int j;

    if (game == NULL) {
        return;
    }

    for (i = 0; i < game->ligne; ++i) {
        for (j = 0; j < game->colonne; ++j) {
            switch (game->plat[i][j]) {
            case CELL_WALL:
                ChoisirCouleurDessin(CouleurParComposante(87, 138, 52));
                DessinerRectangle(CELL_SIZE * j, CELL_SIZE * i, CELL_SIZE, CELL_SIZE);
                RemplirRectangle(CELL_SIZE * j, CELL_SIZE * i, CELL_SIZE, CELL_SIZE);
                break;
            case CELL_EMPTY:
                ChoisirCouleurDessin(boardColor(i, j));
                DessinerRectangle(CELL_SIZE * j, CELL_SIZE * i, CELL_SIZE, CELL_SIZE);
                RemplirRectangle(CELL_SIZE * j, CELL_SIZE * i, CELL_SIZE, CELL_SIZE);
                break;
            case CELL_SNAKE:
                ChoisirCouleurDessin(CouleurParNom("blue"));
                DessinerRectangle(CELL_SIZE * j, CELL_SIZE * i, CELL_SIZE, CELL_SIZE);
                RemplirRectangle(CELL_SIZE * j, CELL_SIZE * i, CELL_SIZE, CELL_SIZE);
                break;
            case CELL_APPLE:
                AfficherSprite(1, CELL_SIZE * j, CELL_SIZE * i);
                break;
            case CELL_TRAP:
                AfficherSprite(5, CELL_SIZE * j, CELL_SIZE * i);
                break;
            case CELL_PORTAL_TOP:
            case CELL_PORTAL_BOTTOM:
            case CELL_PORTAL_LEFT:
            case CELL_PORTAL_RIGHT:
            case CELL_PORTAL_WRAP_LEFT:
            case CELL_PORTAL_WRAP_RIGHT:
                AfficherSprite(4, CELL_SIZE * j, CELL_SIZE * i);
                break;
            default:
                break;
            }
        }
    }
}

void drawsc(Serpent *snake, int inf2)
{
    Noeud *node;

    if (snake == NULL) {
        return;
    }

    node = (inf2 == 0) ? snake->queue : snake->tete;
    if (node == NULL) {
        return;
    }

    if (inf2 == 0) {
        ChoisirCouleurDessin(boardColor(node->x, node->y));
    } else {
        ChoisirCouleurDessin(CouleurParNom("blue"));
    }

    DessinerRectangle(CELL_SIZE * node->y, CELL_SIZE * node->x, CELL_SIZE, CELL_SIZE);
    RemplirRectangle(CELL_SIZE * node->y, CELL_SIZE * node->x, CELL_SIZE, CELL_SIZE);
}
