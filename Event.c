#include "Snake.h"

void gererEvenement(Serpent* snake,Jeux* game,Direction* direction) {
	int test;
	Direction provisoire = {direction->ligne,direction->colonne};
	if(ToucheEnAttente() || game->start==0){
		if(game->start==0)
			test= XK_space;
		else
			test=Touche();
		
		if(direction->ligne == 0) {
			if (test == XK_Up){ 
				provisoire.ligne = -1;
				provisoire.colonne = 0;
			}
			if (test == XK_Down) {
				provisoire.ligne = 1;
				provisoire.colonne = 0;
			}
		}
		if (direction->colonne == 0) {
			if (test == XK_Left) {
				provisoire.colonne = -1;
				provisoire.ligne = 0;
			}
			if (test == XK_Right) {
				provisoire.colonne = 1;
				provisoire.ligne = 0;
			}
		}
		if(test == XK_space ){
			test=Touche();
		}
		if(test == XK_Escape){
			game->end=1;
		}
	}

	print(game);
	direction->ligne = provisoire.ligne;
	direction->colonne = provisoire.colonne;
	int nbpommesbis=game->nbpommes;
	delqueue(snake,game);
	addtete(snake,snake->tete->x + direction->ligne,snake->tete->y + direction->colonne,game,direction);
	if(nbpommesbis!=game->nbpommes)
		game->tour +=2;
	if(game->tour>0)
		game->tour--;
	if(game->nbpommes==0 && (game->gamemode==1 || game->gamemode==2)){
		printf("test1\n");
		game->lvl++;
		game->trap++;
		game->nbpommestot++;
		game->time=game->time -(1500*game->coeff);
		lvlup(game,snake);
	}
	if(game->nbpommes==0 && (game->gamemode==3 || game->gamemode==4)){
		game->lvl++;
		generateFeed(game,4+game->lvl);
		generateTrap(game,game->lvl-1);
	}
	if(game->end==1){
		FermerGraphique();
		/*couleur c;
		char buf[100];
 	   snprintf(buf,100,"Score: %d",game->score);
    	c = CouleurParNom("black");
    	ChoisirCouleurDessin(c);
    	EcrireTexte(100,100,buf,2);*/
		MenuBase(game->score);
	}
	game->start++;
}