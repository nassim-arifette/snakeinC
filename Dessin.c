#include "Snake.h"

void draw(Jeux* game){

	for(int i=0;i<game->ligne;i++){
		for(int j=0;j<game->colonne;j++){
			if(game->plat[i][j]==4){
				couleur c;
				c=CouleurParComposante(87,138,52);
				ChoisirCouleurDessin(c);
				DessinerRectangle(20*j,20*i,20,20);
				RemplirRectangle(20*j,20*i,20,20);
			}
			if(game->plat[i][j]==0){
				couleur c;
				if(i%2==0){
					if(j%2==0)
						c=CouleurParComposante(170,215,80);
					else
						c=CouleurParComposante(162,209,73);
				}
				else{
					if(j%2==1)
						c=CouleurParComposante(170,215,80);
					else
						c=CouleurParComposante(162,209,73);
				}
				ChoisirCouleurDessin(c);
				DessinerRectangle(20*j,20*i,20,20);
				RemplirRectangle(20*j,20*i,20,20);
			}
			if(game->plat[i][j]==1){
				couleur c;
				c=CouleurParNom("blue");
				ChoisirCouleurDessin(c);
				DessinerRectangle(20*j,20*i,20,20);
				RemplirRectangle(20*j,20*i,20,20);
			}
			if(game->plat[i][j]==2){
				AfficherSprite(1,20*j,20*i);
			}
			if(game->plat[i][j]==3){
				/*couleur c;
				c=CouleurParNom("black");
				ChoisirCouleurDessin(c);
				DessinerRectangle(20*j,20*i,20,20);
				RemplirRectangle(20*j,20*i,20,20);*/
				AfficherSprite(5,20*j,20*i);
			}
			if(game->plat[i][j]==6 || game->plat[i][j]==7 || game->plat[i][j]==8 || game->plat[i][j]==9 || game->plat[i][j]==10 || game->plat[i][j]==11){
				/*couleur c;
				c=CouleurParNom("yellow");
				ChoisirCouleurDessin(c);
				DessinerRectangle(20*j,20*i,20,20);
				RemplirRectangle(20*j,20*i,20,20);*/
				AfficherSprite(4,20*j,20*i);
			}
		}
		printf("\n");
	}
}

void drawsc(Serpent* snake,int inf2){
	if(inf2==0){
		couleur c;
		if((snake->queue->x)%2==0){
			if((snake->queue->y)%2==0)
				c=CouleurParComposante(170,215,80);
			else
				c=CouleurParComposante(162,209,73);
		}
		else{
			if((snake->queue->y)%2==1)
				c=CouleurParComposante(170,215,80);
			else
				c=CouleurParComposante(162,209,73);
		}
		ChoisirCouleurDessin(c);
		DessinerRectangle(20*(snake->queue->y),20*(snake->queue->x),20,20);
		RemplirRectangle(20*(snake->queue->y),20*(snake->queue->x),20,20);
	}
	else{
		couleur c;
		c=CouleurParNom("blue");
		ChoisirCouleurDessin(c);
		DessinerRectangle(20*(snake->tete->y),20*(snake->tete->x),20,20);
		RemplirRectangle(20*(snake->tete->y),20*(snake->tete->x),20,20);
	}
}
