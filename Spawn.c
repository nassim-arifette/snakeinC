#include "Snake.h"

Serpent* SpawnSerpent(Jeux* game){

	Serpent* snake;
	Noeud* tetesnake=malloc(sizeof(Noeud));
	Noeud* queuesnake=malloc(sizeof(Noeud));
	Noeud* corpsnake=malloc(sizeof(Noeud));

	int tmp=0,tmp2=0;
	if(game->gamemode==2){
		tmp=game->colonne;
		game->colonne=game->colonne/2-5;
	}


	tetesnake->x = game->ligne/2+2;
	tetesnake->y = game->colonne/2+1;
	tetesnake->suiv = NULL;

	corpsnake->x = game->ligne/2+2;
	corpsnake->y = game->colonne/2;
	corpsnake->suiv = tetesnake;

	queuesnake->x = game->ligne/2+2;
	queuesnake->y = game->colonne/2-1;
	queuesnake->suiv = corpsnake;

	snake->tete = tetesnake;
	snake->queue = queuesnake;

	for (int i=game->colonne/2+1;i<=game->colonne/2+7+game->taille-10;i++){
		Noeud* newtete=malloc(sizeof(Noeud));

		newtete->x = game->ligne/2+2;
		newtete->y = i;
		newtete->suiv = NULL;

		snake->tete->suiv = newtete;
		snake->tete = snake->tete->suiv;
	}

	for(int i=0;i<game->ligne;i++){
		for(int j=0;j<game->colonne;j++){
			if(i==game->ligne/2+2 && j>=game->colonne/2 && j<= game->colonne/2+7+game->taille-10){
				game->plat[i][j]=1;
			}
		}
	}

	if(game->gamemode==2){
		game->colonne=tmp;
	}

	return(snake);
}


void generateFeed(Jeux* game,int feed)
{
	int i = 0;
	int j = 0;
	int count  = 0;

	srand(time(NULL));

	if(game->gamemode==3)
		feed=1;

	while (count != feed){
		i = rand() % (game->ligne);
		j = rand() % (game->colonne);
		if(game->plat[i][j] == 0 && i!=game->ligne/2+2) {
			game->plat[i][j] = 2;
			if(game->gamemode==3){
				AfficherSprite(1,20*j,20*i);
			}
			count++;
		}
	}
	game->nbpommes=feed;
}

void generateTrap(Jeux* game,int trap)
{
	int i = 0;
	int j = 0;
	int count  = 0;

	srand(time(NULL));

	while (count != trap){
		i = rand() % (game->ligne);
		j = rand() % (game->colonne);
		if(game->plat[i][j] == 0 && i!=game->ligne/2+2) {
			game->plat[i][j] = 3;
			if(game->gamemode==3){
				AfficherSprite(5,20*j,20*i);
			}
			count++;
		}
	}
}