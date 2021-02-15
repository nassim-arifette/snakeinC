#include <stdio.h>
#include <stdlib.h>
#include <graph.h>
#include <math.h>
#include <time.h>
#include <unistd.h>


typedef struct Noeud Noeud;
struct Noeud {
	int x;
	int y;
	Noeud* suiv;
};

typedef struct Serpent Serpent;
struct Serpent{
	Noeud* tete;
	Noeud* queue;
	int taille;
};


typedef struct Jeux Jeux;
struct Jeux{
	int** plat;
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
struct Menu{
	int ligne;
	int colonne;
	int gamemode;
	int nbpommes;
	int trap;
	int coeff;
	int taille;
	int score;
};



Serpent* SpawnSerpent(Jeux* game);
void drawsc(Serpent* snake,int inf2);
void addtete(Serpent* snake,int x,int y,Jeux* game,Direction* direction);
void delqueue(Serpent* snake,Jeux* game);
void gererEvenement(Serpent* snake,Jeux* game,Direction* direction);
void lvlup(Jeux* game,Serpent* snake);
Jeux* initialisation(Menu* menue);
void generateFeed(Jeux* game,int feed);
void generateTrap(Jeux* game,int trap);
void printinit(Jeux* game);
void draw(Jeux* game);
void print(Jeux *game);
void Inscritscore(Jeux *game);
void Litscore(Jeux *game);
void launch();
void MenuBase();
int EcriteMenuBase(Menu* menue);
int Entree(int taille,int x, int y);
void jeuxdebase(Menu* menue,int score);