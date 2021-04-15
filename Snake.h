#ifndef SNAKE_H
#define SNAKE_H

#include <graph.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define CELL_SIZE 20
#define SCORE_FILE "test.txt"
#define MIN_TICK_US 10000

typedef enum CellType {
    CELL_EMPTY = 0,
    CELL_SNAKE = 1,
    CELL_APPLE = 2,
    CELL_TRAP = 3,
    CELL_WALL = 4,
    CELL_PORTAL_TOP = 6,
    CELL_PORTAL_BOTTOM = 7,
    CELL_PORTAL_LEFT = 8,
    CELL_PORTAL_RIGHT = 9,
    CELL_PORTAL_WRAP_LEFT = 10,
    CELL_PORTAL_WRAP_RIGHT = 11
} CellType;

typedef enum GameMode {
    GAME_WITH_WALLS = 1,
    GAME_WITH_PORTALS = 2,
    GAME_INFINITE = 3,
    GAME_INFINITE_ALT = 4
} GameMode;

typedef struct Noeud Noeud;
struct Noeud {
    int x;
    int y;
    Noeud *suiv;
};

typedef struct Serpent Serpent;
struct Serpent {
    Noeud *tete;
    Noeud *queue;
    int taille;
};

typedef struct Jeux Jeux;
struct Jeux {
    int **plat;
    int ligne;
    int colonne;
    int nbpommes;
    int nbpommestot;
    int trap;
    int lvl;
    int tour;
    int end;
    int score;
    int highscore;
    int start;
    int gamemode;
    int time;
    int coeff;
    int taille;
};

typedef struct Direction Direction;
struct Direction {
    int ligne;
    int colonne;
};

typedef struct Menu Menu;
struct Menu {
    int ligne;
    int colonne;
    int gamemode;
    int nbpommes;
    int trap;
    int coeff;
    int taille;
    int score;
};

Serpent *SpawnSerpent(Jeux *game);
int ResetSerpent(Serpent *snake, Jeux *game);
void destroySnake(Serpent *snake);
void destroyGame(Jeux *game);

void drawsc(Serpent *snake, int inf2);
void addtete(Serpent *snake, int x, int y, Jeux *game, Direction *direction);
void delqueue(Serpent *snake, Jeux *game);
void gererEvenement(Serpent *snake, Jeux *game, Direction *direction);
void lvlup(Jeux *game, Serpent *snake);

Jeux *initialisation(Menu *menue);
void generateFeed(Jeux *game, int feed);
void generateTrap(Jeux *game, int trap);
void printinit(Jeux *game);
void draw(Jeux *game);
void print(Jeux *game);

void Inscritscore(Jeux *game);
void Litscore(Jeux *game);

int launch(Menu *menue);
int MenuBase(int score);
int EcriteMenuBase(Menu *menue);
int Entree(int taille, int x, int y);
void jeuxdebase(Menu *menue, int score);

#endif
