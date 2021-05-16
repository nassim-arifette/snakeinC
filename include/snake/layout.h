#ifndef SNAKE_LAYOUT_H
#define SNAKE_LAYOUT_H

/*
 * Politique de dimensionnement de la fenetre et de la surface de jeu.
 *
 * Ce module ne depend d'aucune API graphique : il ne contient que du calcul.
 * L'application choisit une seule taille de fenetre au demarrage et la garde
 * pour toute la duree de vie du programme (menu et partie comprises).
 */

/* Marges reservees autour de la fenetre (bords, barre de titre, barre des taches). */
#define LAYOUT_MARGIN_HORIZONTAL 48
#define LAYOUT_MARGIN_VERTICAL 120

/* Bornes de securite pour les ecrans tres petits ou tres grands. */
#define LAYOUT_MIN_WIDTH 640
#define LAYOUT_MIN_HEIGHT 520
#define LAYOUT_MAX_WIDTH 1920
#define LAYOUT_MAX_HEIGHT 1200

/* Bornes de la taille d'une case du plateau. */
#define LAYOUT_CELL_SIZE_DEFAULT 20
#define LAYOUT_CELL_SIZE_MIN 6
#define LAYOUT_CELL_SIZE_MAX 48

typedef struct WindowSize {
    int width;
    int height;
} WindowSize;

typedef struct BoardGeometry {
    int cell_size;
    int origin_x;
    int origin_y;
} BoardGeometry;

typedef struct UiTransform {
    float scale;
    float offset_x;
    float offset_y;
} UiTransform;

/* Taille de fenetre adaptee a la taille d'ecran fournie. */
WindowSize layout_window_size(int monitor_width, int monitor_height);

/* Placement du plateau dans la fenetre : cases entieres, plateau centre. */
BoardGeometry layout_board_geometry(WindowSize window, int rows, int columns, int hud_height);

/* Mise a l'echelle d'une interface concue dans un espace de reference. */
UiTransform layout_ui_transform(WindowSize window, int design_width, int design_height);

#endif
