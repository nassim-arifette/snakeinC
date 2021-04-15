#include "Snake.h"

static void resetBoard(Jeux *game)
{
    int i;
    int j;

    for (i = 0; i < game->ligne; ++i) {
        for (j = 0; j < game->colonne; ++j) {
            if (i == game->ligne / 2 - 6 && j >= game->colonne / 2 - 5 && j <= game->colonne / 2 + 4) {
                game->plat[i][j] = (game->gamemode == GAME_WITH_PORTALS) ? CELL_PORTAL_TOP : CELL_EMPTY;
            } else if (i == game->ligne / 2 + 11 && j >= game->colonne / 2 - 5 && j <= game->colonne / 2 + 4) {
                game->plat[i][j] = (game->gamemode == GAME_WITH_PORTALS) ? CELL_PORTAL_BOTTOM : CELL_EMPTY;
            } else if (j == game->colonne / 2 - 9 && i >= game->ligne / 2 - 2 && i <= game->ligne / 2 + 7) {
                game->plat[i][j] = (game->gamemode == GAME_WITH_PORTALS) ? CELL_PORTAL_LEFT : CELL_EMPTY;
            } else if (j == game->colonne / 2 + 8 && i >= game->ligne / 2 - 2 && i <= game->ligne / 2 + 7) {
                game->plat[i][j] = (game->gamemode == GAME_WITH_PORTALS) ? CELL_PORTAL_RIGHT : CELL_EMPTY;
            } else if (j == 0 && i >= game->ligne / 2 - 2 && i <= game->ligne / 2 + 7) {
                game->plat[i][j] = (game->gamemode == GAME_WITH_PORTALS) ? CELL_PORTAL_WRAP_LEFT : CELL_WALL;
            } else if (j == game->colonne - 1 && i >= game->ligne / 2 - 2 && i <= game->ligne / 2 + 7) {
                game->plat[i][j] = (game->gamemode == GAME_WITH_PORTALS) ? CELL_PORTAL_WRAP_RIGHT : CELL_WALL;
            } else if (i >= game->ligne / 2 - 6 && i <= game->ligne / 2 + 11 &&
                       j >= game->colonne / 2 - 9 && j <= game->colonne / 2 + 8) {
                game->plat[i][j] = (game->gamemode == GAME_WITH_PORTALS) ? CELL_WALL : CELL_EMPTY;
            } else if (i > 5 && i != game->ligne - 1 && j != 0 && j != game->colonne - 1) {
                game->plat[i][j] = CELL_EMPTY;
            } else {
                game->plat[i][j] = CELL_WALL;
            }
        }
    }
}

int main(void)
{
    int last_score = 0;

    srand((unsigned int)time(NULL));

    for (;;) {
        int result = MenuBase(last_score);
        if (result < 0) {
            break;
        }
        last_score = result;
    }

    return 0;
}

int launch(Menu *menue)
{
    Jeux *game;
    Serpent *snake;
    Direction direction = {0, 1};
    int final_score;

    FermerGraphique();
    InitialiserGraphique();
    CreerFenetre(0, 0, CELL_SIZE * (menue->colonne + 2), CELL_SIZE * (menue->ligne + 7));

    ChargerSprite("pomme.png");
    ChargerSprite("pommetop.png");
    ChargerSprite("trophee.png");
    ChargerSprite("portal.png");
    ChargerSprite("bomb.png");

    game = initialisation(menue);
    if (game == NULL) {
        FermerGraphique();
        return menue->score;
    }

    snake = SpawnSerpent(game);
    if (snake == NULL) {
        destroyGame(game);
        FermerGraphique();
        return menue->score;
    }

    generateFeed(game, game->nbpommes);
    generateTrap(game, game->trap);
    printinit(game);
    draw(game);

    while (!game->end) {
        usleep((useconds_t)game->time);
        gererEvenement(snake, game, &direction);
    }

    final_score = game->score;
    destroySnake(snake);
    destroyGame(game);
    FermerGraphique();

    return final_score;
}

Jeux *initialisation(Menu *menue)
{
    Jeux *game;
    int i;

    game = calloc(1, sizeof(*game));
    if (game == NULL) {
        return NULL;
    }

    game->ligne = menue->ligne + 7;
    game->colonne = menue->colonne + 2;
    game->gamemode = menue->gamemode;
    game->nbpommes = menue->nbpommes;
    game->nbpommestot = menue->nbpommes;
    game->trap = menue->trap;
    game->lvl = 1;
    game->time = 60000;
    game->coeff = menue->coeff;
    game->taille = menue->taille;

    game->plat = calloc((size_t)game->ligne, sizeof(*game->plat));
    if (game->plat == NULL) {
        free(game);
        return NULL;
    }

    for (i = 0; i < game->ligne; ++i) {
        game->plat[i] = calloc((size_t)game->colonne, sizeof(*game->plat[i]));
        if (game->plat[i] == NULL) {
            while (i > 0) {
                free(game->plat[--i]);
            }
            free(game->plat);
            free(game);
            return NULL;
        }
    }

    resetBoard(game);
    return game;
}

void destroyGame(Jeux *game)
{
    int i;

    if (game == NULL) {
        return;
    }

    if (game->plat != NULL) {
        for (i = 0; i < game->ligne; ++i) {
            free(game->plat[i]);
        }
        free(game->plat);
    }

    free(game);
}

void lvlup(Jeux *game, Serpent *snake)
{
    resetBoard(game);

    if (!ResetSerpent(snake, game)) {
        game->end = 1;
        return;
    }

    game->tour = 0;
    game->start = 0;
    generateFeed(game, game->nbpommestot);
    generateTrap(game, game->trap);
    printinit(game);
    draw(game);
}

void jeuxdebase(Menu *menue, int score)
{
    menue->ligne = 40;
    menue->colonne = 60;
    menue->gamemode = GAME_WITH_WALLS;
    menue->nbpommes = 5;
    menue->trap = 0;
    menue->coeff = 3;
    menue->taille = 10;
    menue->score = score;
}
