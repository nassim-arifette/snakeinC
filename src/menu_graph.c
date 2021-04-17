#include "menu.h"

#include <graph.h>
#include <stdio.h>

typedef struct MenuState {
    GameConfig config;
    int last_score;
} MenuState;

static void draw_main_menu(const MenuState *state)
{
    char buffer[100];

    EffacerEcran(CouleurParNom("white"));
    ChargerImageFond("backgroundmenu.jpg");
    ChoisirCouleurDessin(CouleurParNom("black"));

    EcrireTexte(300, 100, "Menu", 2);
    EcrireTexte(300, 250, "Jouer", 2);
    EcrireTexte(300, 325, "Option", 2);
    EcrireTexte(300, 400, "Fermer", 2);

    snprintf(buffer, sizeof(buffer), "Dernier Score: %d", state->last_score);
    EcrireTexte(100, 500, buffer, 2);
}

static void draw_options(void)
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

static int key_to_digit(int key)
{
    if (key >= XK_0 && key <= XK_9) {
        return key - XK_0;
    }
    if (key >= XK_KP_0 && key <= XK_KP_9) {
        return key - XK_KP_0;
    }
    return -1;
}

static int read_number(int max_digits, int x, int y)
{
    int value = 0;
    int count = 0;
    int draw_x = x;

    ChoisirCouleurDessin(CouleurParNom("black"));

    for (;;) {
        int key = Touche();
        int digit;
        char text[2];

        if (key == XK_KP_Enter || key == XK_Return) {
            return value;
        }

        digit = key_to_digit(key);
        if (digit < 0) {
            continue;
        }

        if (count >= max_digits) {
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
}

static void highlight_text(int x, int y, const char *text)
{
    ChoisirCouleurDessin(CouleurParNom("red"));
    EcrireTexte(x, y, text, 2);
}

bool menu_run(int last_score, GameConfig *out_config)
{
    MenuState state;
    int screen = 0;

    if (out_config == NULL) {
        return false;
    }

    game_config_default(&state.config);
    state.last_score = last_score;

    InitialiserGraphique();
    CreerFenetre(10, 10, 700, 900);
    draw_main_menu(&state);

    for (;;) {
        if (!SourisCliquee()) {
            continue;
        }

        SourisPosition();

        if (screen == 0) {
            if (_X >= 295 && _X <= 395 && _Y >= 220 && _Y <= 270) {
                game_config_sanitize(&state.config);
                *out_config = state.config;
                FermerGraphique();
                return true;
            }

            if (_X >= 295 && _X <= 395 && _Y >= 290 && _Y <= 340) {
                screen = 1;
                draw_options();
                continue;
            }

            if (_X >= 295 && _X <= 370 && _Y >= 375 && _Y <= 430) {
                FermerGraphique();
                return false;
            }

            continue;
        }

        if (_X >= 75 && _X <= 215 && _Y >= 115 && _Y <= 165) {
            state.config.speed_coeff = 1;
            draw_options();
            highlight_text(100, 150, "Facile");
        } else if (_X >= 275 && _X <= 415 && _Y >= 115 && _Y <= 165) {
            state.config.speed_coeff = 5;
            draw_options();
            highlight_text(300, 150, "Moyen");
        } else if (_X >= 475 && _X <= 615 && _Y >= 115 && _Y <= 165) {
            state.config.speed_coeff = 10;
            draw_options();
            highlight_text(500, 150, "Difficile");
        } else if (_X >= 40 && _X <= 240 && _Y >= 270 && _Y <= 320) {
            state.config.mode = GAME_WITH_WALLS;
            draw_options();
            highlight_text(50, 300, "Avec Bord");
        } else if (_X >= 290 && _X <= 490 && _Y >= 270 && _Y <= 320) {
            state.config.mode = GAME_WITH_PORTALS;
            draw_options();
            highlight_text(300, 300, "Sans Bord");
        } else if (_X >= 520 && _X <= 620 && _Y >= 270 && _Y <= 320) {
            state.config.mode = GAME_INFINITE;
            draw_options();
            highlight_text(530, 300, "Infini");
        } else if (_X >= 50 && _X <= 250 && _Y >= 400 && _Y <= 450) {
            int value = read_number(4, 50, 430);
            if (value > 0) {
                state.config.rows = value;
            }
        } else if (_X >= 375 && _X <= 675 && _Y >= 400 && _Y <= 450) {
            int value = read_number(4, 375, 430);
            if (value > 0) {
                state.config.columns = value;
            }
        } else if (_X >= 50 && _X <= 250 && _Y >= 550 && _Y <= 600) {
            int value = read_number(4, 50, 580);
            if (value > 0) {
                state.config.apple_count = value;
            }
        } else if (_X >= 375 && _X <= 600 && _Y >= 550 && _Y <= 600) {
            state.config.trap_count = read_number(4, 375, 580);
        } else if (_X >= 250 && _X <= 450 && _Y >= 715 && _Y <= 765) {
            int value = read_number(4, 250, 745);
            if (value > 0) {
                state.config.snake_length = value;
            }
        } else if (_X >= 10 && _X <= 150 && _Y >= 790 && _Y <= 840) {
            screen = 0;
            draw_main_menu(&state);
        } else if (_X >= 470 && _X <= 610 && _Y >= 790 && _Y <= 840) {
            game_config_sanitize(&state.config);
            *out_config = state.config;
            FermerGraphique();
            return true;
        }
    }
}
