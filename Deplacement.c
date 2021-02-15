#include "Snake.h"

void addtete(Serpent* snake,int x,int y,Jeux* game,Direction* direction){
	Noeud* newtete=malloc(sizeof(Noeud));

	/*newtete->x = x+1;
	newtete->y = 1;
	newtete->suiv = NULL;*/

	if(game->plat[(snake->tete->x)+(direction->ligne)][(snake->tete->y)+(direction->colonne)]==4
			|| game->plat[(snake->tete->x)+(direction->ligne)][(snake->tete->y)+(direction->colonne)]==1
			|| game->plat[(snake->tete->x)+(direction->ligne)][(snake->tete->y)+(direction->colonne)]==3){
		printf("Le score final: %d\n", game->score);
		Inscritscore(game);
		Litscore(game);
		game->end=1;
	}
	else if(game->plat[(snake->tete->x)+(direction->ligne)][(snake->tete->y)+(direction->colonne)]==2){
		newtete->x = x;
		newtete->y = y;
		newtete->suiv = NULL;
		game->score+=5;
		game->nbpommes--;
		printf("test2\n");
		snake->tete->suiv = newtete;
		game->plat[(snake->tete->x)][(snake->tete->y)] =1;
		snake->tete = snake->tete->suiv;
		game->plat[snake->tete->x][snake->tete->y] = 1;
		drawsc(snake,1);
	}
	else if(game->plat[(snake->tete->x)+(direction->ligne)][(snake->tete->y)+(direction->colonne)]==6){
		newtete->x = x+18;
		newtete->y = y;
		newtete->suiv = NULL;
		snake->tete->suiv = newtete;
		game->plat[(snake->tete->x)][(snake->tete->y)] =1;
		snake->tete = snake->tete->suiv;
		game->plat[snake->tete->x][snake->tete->y] = 1;
		drawsc(snake,1);
	}
	else if(game->plat[(snake->tete->x)+(direction->ligne)][(snake->tete->y)+(direction->colonne)]==7){
		newtete->x = x-18;
		newtete->y = y;
		newtete->suiv = NULL;
		snake->tete->suiv = newtete;
		game->plat[(snake->tete->x)][(snake->tete->y)] =1;
		snake->tete = snake->tete->suiv;
		game->plat[snake->tete->x][snake->tete->y] = 1;
		drawsc(snake,1);
	}
	else if(game->plat[(snake->tete->x)+(direction->ligne)][(snake->tete->y)+(direction->colonne)]==8){
		newtete->x = x;
		newtete->y = y+18;
		newtete->suiv = NULL;
		snake->tete->suiv = newtete;
		game->plat[(snake->tete->x)][(snake->tete->y)] =1;
		snake->tete = snake->tete->suiv;
		game->plat[snake->tete->x][snake->tete->y] = 1;
		drawsc(snake,1);
	}
	else if(game->plat[(snake->tete->x)+(direction->ligne)][(snake->tete->y)+(direction->colonne)]==9){
		newtete->x = x;
		newtete->y = y-18;
		newtete->suiv = NULL;
		snake->tete->suiv = newtete;
		game->plat[(snake->tete->x)][(snake->tete->y)] =1;
		snake->tete = snake->tete->suiv;
		game->plat[snake->tete->x][snake->tete->y] = 1;
		drawsc(snake,1);
	}
	else if(game->plat[(snake->tete->x)+(direction->ligne)][(snake->tete->y)+(direction->colonne)]==10){
		newtete->x = x;
		newtete->y = game->colonne-2;
		newtete->suiv = NULL;
		snake->tete->suiv = newtete;
		game->plat[(snake->tete->x)][(snake->tete->y)] =1;
		snake->tete = snake->tete->suiv;
		game->plat[snake->tete->x][snake->tete->y] = 1;
		drawsc(snake,1);
	}
	else if(game->plat[(snake->tete->x)+(direction->ligne)][(snake->tete->y)+(direction->colonne)]==11){
		newtete->x = x;
		newtete->y = 1;
		newtete->suiv = NULL;
		snake->tete->suiv = newtete;
		game->plat[(snake->tete->x)][(snake->tete->y)] =1;
		snake->tete = snake->tete->suiv;
		game->plat[snake->tete->x][snake->tete->y] = 1;
		drawsc(snake,1);
	}
	else{
		newtete->x = x;
		newtete->y = y;
		newtete->suiv = NULL;
		snake->tete->suiv = newtete;
		game->plat[(snake->tete->x)][(snake->tete->y)] =1;
		snake->tete = snake->tete->suiv;
		game->plat[snake->tete->x][snake->tete->y] = 1;
		drawsc(snake,1);
	}

}

void delqueue(Serpent* snake,Jeux* game) {
	if(game->tour==0){
		Noeud* auxi;

		drawsc(snake,0);
		auxi = snake->queue;
		game->plat[snake->queue->x][snake->queue->y] = 0;
		snake->queue = snake->queue->suiv;
		free(auxi);
	}
}