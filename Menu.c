#include "Snake.h"

static void drawMainMenu(const Menu *menue)
{
    char buf[100];

    EffacerEcran(CouleurParNom("white"));
    ChargerImageFond("backgroundmenu.jpg");
    ChoisirCouleurDessin(CouleurParNom("black"));

    EcrireTexte(300, 100, "Menu", 2);
    EcrireTexte(300, 250, "Jouer", 2);
    EcrireTexte(300, 325, "Option", 2);
    EcrireTexte(300, 400, "Fermer", 2);

    snprintf(buf, sizeof(buf), "Dernier Score: %d", menue->score);
    EcrireTexte(100, 500, buf, 2);
}

static void EcritureOption(void)
{
    ChoisirCouleurDessin(CouleurParNom("white"));
    ChargerImageFond("backgroundmenu.jpg");

    EcrireTexte(290, 100, "Difficulte", 2);
    EcrireTexte(100, 150, "Facile", 2);
    EcrireTexte(300, 150, "Moyen", 2);
    EcrireTexte(500, 150, "Difficile", 2);

    EcrireTexte(30, 820, "Retour", 2);
    EcrireTexte(500, 820, "Jouer", 2);

    EcrireTexte(320, 250, "Mode de jeu", 2);
    EcrireTexte(50, 300, "Avec Bord", 2);
    EcrireTexte(300, 300, "Sans Bord", 2);
    EcrireTexte(530, 300, "Infini", 2);

    EcrireTexte(50, 400, "Nombre de lignes", 2);
    DessinerRectangle(50, 400, 200, 50);
    RemplirRectangle(50, 400, 200, 50);

    EcrireTexte(375, 400, "Nombre de colonnes", 2);
    DessinerRectangle(375, 400, 200, 50);
    RemplirRectangle(375, 400, 200, 50);

    EcrireTexte(50, 550, "Nombre de pommes", 2);
    DessinerRectangle(50, 550, 200, 50);
    RemplirRectangle(50, 550, 200, 50);

    EcrireTexte(375, 550, "Nombre d'obstacles", 2);
    DessinerRectangle(375, 550, 200, 50);
    RemplirRectangle(375, 550, 200, 50);

    EcrireTexte(290, 700, "Taille", 2);
    DessinerRectangle(250, 715, 200, 50);
    RemplirRectangle(250, 715, 200, 50);
}

static int keyToDigit(int key)
{
    if (key >= XK_0 && key <= XK_9) {
        return key - XK_0;
    }
    if (key >= XK_KP_0 && key <= XK_KP_9) {
        return key - XK_KP_0;
    }
    return -1;
}

int Entree(int taille, int x, int y)
{
    int value = 0;
    int count = 0;
    int draw_x = x;

    if (taille <= 0) {
        return 0;
    }

    ChoisirCouleurDessin(CouleurParNom("black"));

    for (;;) {
        int key = Touche();
        int digit;
        char text[2];

        if (key == XK_KP_Enter || key == XK_Return) {
            break;
        }

        digit = keyToDigit(key);
        if (digit < 0) {
            continue;
        }

        if (count >= taille) {
            ChoisirCouleurDessin(CouleurParNom("red"));
            EcrireTexte(x, y - 25, "Taille Invalide", 1);
            ChoisirCouleurDessin(CouleurParNom("black"));
            continue;
        }

        value = value * 10 + digit;
        text[0] = (char)('0' + digit);
        text[1] = '\0';
        EcrireTexte(draw_x, y, text, 2);
        draw_x += 15;
        ++count;
    }

    return value;
}

static void validateMenu(Menu *menue)
{
    int max_snake;

    if (menue->ligne < 20) {
        menue->ligne = 20;
    } else if (menue->ligne > 100) {
        menue->ligne = 100;
    }

    if (menue->colonne < 30) {
        menue->colonne = 30;
    } else if (menue->colonne > 150) {
        menue->colonne = 150;
    }

    if (menue->gamemode < GAME_WITH_WALLS || menue->gamemode > GAME_INFINITE) {
        menue->gamemode = GAME_WITH_WALLS;
    }

    if (menue->coeff < 1) {
        menue->coeff = 1;
    } else if (menue->coeff > 20) {
        menue->coeff = 20;
    }

    if (menue->nbpommes < 1) {
        menue->nbpommes = 1;
    }

    if (menue->trap < 0) {
        menue->trap = 0;
    }

    if (menue->taille < 3) {
        menue->taille = 3;
    }

    if (menue->gamemode == GAME_WITH_PORTALS) {
        max_snake = menue->colonne / 4 - 4;
    } else {
        max_snake = menue->colonne / 2;
    }
    if (max_snake < 3) {
        max_snake = 3;
    }
    if (menue->taille > max_snake) {
        menue->taille = max_snake;
    }
}

static void highlightText(int x, int y, const char *text)
{
    ChoisirCouleurDessin(CouleurParNom("red"));
    EcrireTexte(x, y, text, 2);
}

int EcriteMenuBase(Menu *menue)
{
    int screen = 0;

    drawMainMenu(menue);

    for (;;) {
        if (!SourisCliquee()) {
            continue;
        }

        SourisPosition();

        if (screen == 0) {
            if (_X >= 295 && _X <= 395 && _Y >= 220 && _Y <= 270) {
                validateMenu(menue);
                return 1;
            }
            if (_X >= 295 && _X <= 395 && _Y >= 290 && _Y <= 340) {
                screen = 1;
                EcritureOption();
                continue;
            }
            if (_X >= 295 && _X <= 370 && _Y >= 375 && _Y <= 430) {
                return 3;
            }
            continue;
        }

        if (_X >= 75 && _X <= 215 && _Y >= 115 && _Y <= 165) {
            menue->coeff = 1;
            EcritureOption();
            highlightText(100, 150, "Facile");
        } else if (_X >= 275 && _X <= 415 && _Y >= 115 && _Y <= 165) {
            menue->coeff = 5;
            EcritureOption();
            highlightText(300, 150, "Moyen");
        } else if (_X >= 475 && _X <= 615 && _Y >= 115 && _Y <= 165) {
            menue->coeff = 10;
            EcritureOption();
            highlightText(500, 150, "Difficile");
        } else if (_X >= 40 && _X <= 240 && _Y >= 270 && _Y <= 320) {
            menue->gamemode = GAME_WITH_WALLS;
            EcritureOption();
            highlightText(50, 300, "Avec Bord");
        } else if (_X >= 290 && _X <= 490 && _Y >= 270 && _Y <= 320) {
            menue->gamemode = GAME_WITH_PORTALS;
            EcritureOption();
            highlightText(300, 300, "Sans Bord");
        } else if (_X >= 520 && _X <= 620 && _Y >= 270 && _Y <= 320) {
            menue->gamemode = GAME_INFINITE;
            EcritureOption();
            highlightText(530, 300, "Infini");
        } else if (_X >= 50 && _X <= 250 && _Y >= 400 && _Y <= 450) {
            int value = Entree(4, 50, 430);
            if (value > 0) {
                menue->ligne = value;
            }
        } else if (_X >= 375 && _X <= 675 && _Y >= 400 && _Y <= 450) {
            int value = Entree(4, 375, 430);
            if (value > 0) {
                menue->colonne = value;
            }
        } else if (_X >= 50 && _X <= 250 && _Y >= 550 && _Y <= 600) {
            int value = Entree(4, 50, 580);
            if (value > 0) {
                menue->nbpommes = value;
            }
        } else if (_X >= 375 && _X <= 600 && _Y >= 550 && _Y <= 600) {
            menue->trap = Entree(4, 375, 580);
        } else if (_X >= 250 && _X <= 450 && _Y >= 715 && _Y <= 765) {
            int value = Entree(4, 250, 745);
            if (value > 0) {
                menue->taille = value;
            }
        } else if (_X >= 10 && _X <= 150 && _Y >= 790 && _Y <= 840) {
            screen = 0;
            drawMainMenu(menue);
        } else if (_X >= 470 && _X <= 610 && _Y >= 790 && _Y <= 840) {
            validateMenu(menue);
            return 1;
        }
    }
}

int MenuBase(int score)
{
    Menu menue;
    int action;

    jeuxdebase(&menue, score);

    InitialiserGraphique();
    CreerFenetre(10, 10, 700, 900);
    action = EcriteMenuBase(&menue);

    if (action == 3) {
        FermerGraphique();
        return -1;
    }

    if (action == 1) {
        return launch(&menue);
    }

    FermerGraphique();
    return -1;
}
