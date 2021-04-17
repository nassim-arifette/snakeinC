#include "renderer.h"

#include <graph.h>
#include <stdio.h>

#define CELL_SIZE 20

static couleur board_color(int row, int column)
{
    if ((row % 2) == (column % 2)) {
        return CouleurParComposante(170, 215, 80);
    }

    return CouleurParComposante(162, 209, 73);
}

static void draw_cell(const Game *game, int row, int column)
{
    CellType cell = board_get(&game->board, row, column);

    switch (cell) {
    case CELL_WALL:
        ChoisirCouleurDessin(CouleurParComposante(87, 138, 52));
        DessinerRectangle(CELL_SIZE * column, CELL_SIZE * row, CELL_SIZE, CELL_SIZE);
        RemplirRectangle(CELL_SIZE * column, CELL_SIZE * row, CELL_SIZE, CELL_SIZE);
        break;
    case CELL_EMPTY:
        ChoisirCouleurDessin(board_color(row, column));
        DessinerRectangle(CELL_SIZE * column, CELL_SIZE * row, CELL_SIZE, CELL_SIZE);
        RemplirRectangle(CELL_SIZE * column, CELL_SIZE * row, CELL_SIZE, CELL_SIZE);
        break;
    case CELL_SNAKE:
        ChoisirCouleurDessin(CouleurParNom("blue"));
        DessinerRectangle(CELL_SIZE * column, CELL_SIZE * row, CELL_SIZE, CELL_SIZE);
        RemplirRectangle(CELL_SIZE * column, CELL_SIZE * row, CELL_SIZE, CELL_SIZE);
        break;
    case CELL_APPLE:
        AfficherSprite(1, CELL_SIZE * column, CELL_SIZE * row);
        break;
    case CELL_TRAP:
        AfficherSprite(5, CELL_SIZE * column, CELL_SIZE * row);
        break;
    case CELL_PORTAL_TOP:
    case CELL_PORTAL_BOTTOM:
    case CELL_PORTAL_LEFT:
    case CELL_PORTAL_RIGHT:
    case CELL_PORTAL_WRAP_LEFT:
    case CELL_PORTAL_WRAP_RIGHT:
        AfficherSprite(4, CELL_SIZE * column, CELL_SIZE * row);
        break;
    default:
        break;
    }
}

static void draw_hud(const Game *game, int highscore)
{
    char buffer[100];

    ChoisirCouleurDessin(CouleurParComposante(87, 138, 52));
    DessinerRectangle(50, 40, 50, 50);
    RemplirRectangle(50, 40, 50, 50);
    DessinerRectangle(345, 40, 50, 50);
    RemplirRectangle(345, 40, 50, 50);
    DessinerRectangle(435, 40, 50, 50);
    RemplirRectangle(435, 40, 50, 50);

    AfficherSprite(2, 0, 30);
    snprintf(buffer, sizeof(buffer), "%d", game->apples_remaining);
    ChoisirCouleurDessin(CouleurParNom("black"));
    EcrireTexte(60, 70, buffer, 2);

    AfficherSprite(3, 100, 30);
    snprintf(buffer, sizeof(buffer), "%d", highscore);
    EcrireTexte(160, 70, buffer, 2);

    snprintf(buffer, sizeof(buffer), "Score: %d", game->score);
    EcrireTexte(250, 70, buffer, 2);

    if (game->mode == GAME_WITH_WALLS || game->mode == GAME_WITH_PORTALS) {
        snprintf(buffer, sizeof(buffer), "Level: %d", game->level);
        EcrireTexte(440, 70, buffer, 2);
    }
}

void renderer_open(const Game *game)
{
    if (game == NULL) {
        return;
    }

    InitialiserGraphique();
    CreerFenetre(0,
                 0,
                 CELL_SIZE * game->board.columns,
                 CELL_SIZE * game->board.rows);

    ChargerSprite("pomme.png");
    ChargerSprite("pommetop.png");
    ChargerSprite("trophee.png");
    ChargerSprite("portal.png");
    ChargerSprite("bomb.png");
}

void renderer_draw(const Game *game, int highscore)
{
    int row;
    int column;

    if (game == NULL) {
        return;
    }

    for (row = 0; row < game->board.rows; ++row) {
        for (column = 0; column < game->board.columns; ++column) {
            draw_cell(game, row, column);
        }
    }

    draw_hud(game, highscore);
}

void renderer_close(void)
{
    FermerGraphique();
}
