#include "Snake.h"

void print(Jeux *game)
{
    couleur c;
    char buf[100];

    if (game == NULL) {
        return;
    }

    c = CouleurParComposante(87, 138, 52);
    ChoisirCouleurDessin(c);
    DessinerRectangle(50, 40, 50, 50);
    RemplirRectangle(50, 40, 50, 50);
    DessinerRectangle(345, 40, 50, 50);
    RemplirRectangle(345, 40, 50, 50);
    DessinerRectangle(435, 40, 50, 50);
    RemplirRectangle(435, 40, 50, 50);

    AfficherSprite(2, 0, 30);
    snprintf(buf, sizeof(buf), "%d", game->nbpommes);
    ChoisirCouleurDessin(CouleurParNom("black"));
    EcrireTexte(60, 70, buf, 2);

    AfficherSprite(3, 100, 30);
    Litscore(game);
    snprintf(buf, sizeof(buf), "%d", game->highscore);
    EcrireTexte(160, 70, buf, 2);

    snprintf(buf, sizeof(buf), "Score: %d", game->score);
    EcrireTexte(250, 70, buf, 2);

    if (game->gamemode == GAME_WITH_WALLS || game->gamemode == GAME_WITH_PORTALS) {
        snprintf(buf, sizeof(buf), "Level: %d", game->lvl);
        EcrireTexte(440, 70, buf, 2);
    }
}

void printinit(Jeux *game)
{
    (void)game;
}

void Inscritscore(Jeux *game)
{
    FILE *fichier;

    if (game == NULL) {
        return;
    }

    fichier = fopen(SCORE_FILE, "a");
    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir %s en ecriture\n", SCORE_FILE);
        return;
    }

    fprintf(fichier, "%d\n", game->score);
    fclose(fichier);
}

void Litscore(Jeux *game)
{
    FILE *fichier;
    int value;
    int highscore = 0;

    if (game == NULL) {
        return;
    }

    fichier = fopen(SCORE_FILE, "r");
    if (fichier == NULL) {
        game->highscore = 0;
        return;
    }

    while (fscanf(fichier, "%d", &value) == 1) {
        if (value > highscore) {
            highscore = value;
        }
    }

    fclose(fichier);
    game->highscore = highscore;
}
