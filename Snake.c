#include "Snake.h"

int main(){
	MenuBase(0);
	//launch();
	return 0;
}

void launch(Menu* menue){
	FermerGraphique();
	InitialiserGraphique();
	printf("%d\n",menue->ligne);
	CreerFenetre(0,0,20*menue->colonne+2*20,20*menue->ligne+7*20);
	ChargerSprite("pomme.png");
	ChargerSprite("pommetop.png");
	ChargerSprite("trophee.png");
	ChargerSprite("portal.png");
	ChargerSprite("bomb.png");
	Jeux* game=initialisation(menue);
	Serpent* snake=SpawnSerpent(game);
	Direction direction;
	direction.ligne=0;
	direction.colonne=1;
	generateFeed(game,game->nbpommes);
	generateTrap(game,game->trap);
	printinit(game);
	draw(game);
	int go_on=1;
	while(go_on){
		usleep(game->time);
		gererEvenement(snake,game,&direction);
	}
	FermerGraphique();
}

Jeux* initialisation(Menu* menue) {
	Jeux* game=malloc(sizeof(Jeux));
	game->ligne=menue->ligne+7;
	game->colonne=menue->colonne+2;
	printf("%d\n",game->ligne);
	game->gamemode=menue->gamemode;
	game->nbpommes=menue->nbpommes;
	game->nbpommestot=menue->nbpommes;
	game->trap=menue->trap;
	game->tour=0;
	game->end = 0;
	game->lvl=1;
	game->score=0;
	game->start=0;
	game->time=60000;
	game->coeff=menue->coeff;
	game->taille=menue->taille;
	game->plat=(int**) malloc(sizeof(int*) * game->ligne);
	for(int i=0;i<game->ligne;i++){
		game->plat[i]=(int*) malloc(sizeof(int) * game->colonne);
	}
	for(int i=0;i<game->ligne;i++){
		for(int j=0;j<game->colonne;j++){
			if(i==game->ligne/2-6 && j>=game->colonne/2-5 && j<=game->colonne/2+4){
				if(game->gamemode==2)
					game->plat[i][j]=6;
				else 
					game->plat[i][j]=0;
			}
			else if(i==game->ligne/2+11 && j>=game->colonne/2-5 && j<=game->colonne/2+4){
				if(game->gamemode==2)
					game->plat[i][j]=7;
				else 
					game->plat[i][j]=0;
			}
			else if(j==game->colonne/2-9 && i>=game->ligne/2-2 && i<=game->ligne/2+7){
				if(game->gamemode==2)
					game->plat[i][j]=8;
				else 
					game->plat[i][j]=0;
			}
			else if(j==game->colonne/2+8 && i>=game->ligne/2-2 && i<=game->ligne/2+7){
				if(game->gamemode==2)
					game->plat[i][j]=9;
				else 
					game->plat[i][j]=0;
			}
			else if(j==0 && i>=game->ligne/2-2 && i<=game->ligne/2+7){
				if(game->gamemode==2)
					game->plat[i][j]=10;
				else 
					game->plat[i][j]=4;
			}
			else if(j==game->colonne-1 && i>=game->ligne/2-2 && i<=game->ligne/2+7){
				if(game->gamemode==2)
					game->plat[i][j]=11;
				else 
					game->plat[i][j]=4;
			}
			else if(i>=game->ligne/2-6 && i<=game->ligne/2+11 && j>=game->colonne/2-9 && j<=game->colonne/2+8){
				if(game->gamemode==2)
					game->plat[i][j]=4;
				else 
					game->plat[i][j]=0;
			}
			else if(i>5 && i!=game->ligne-1 && j!=0 && j!=game->colonne-1){
				game->plat[i][j]=0;
			}
			else{
				game->plat[i][j]=4;
			}
		}
	}
	return game;
}

void lvlup(Jeux* game,Serpent* snake){
	/*EffacerEcran(CouleurParComposante(0,0,0));
	couleur c;
	char buf[100];
	snprintf(buf,100,"Passage au niveau %d",game->lvl);
	c = CouleurParNom("white");
	ChoisirCouleurDessin(c);
	EcrireTexte(570,940/2,buf,2);
	usleep(1000000);*/
	printf("test\n");

	for(int i=0;i<game->ligne;i++){
		for(int j=0;j<game->colonne;j++){
			if(i==game->ligne/2-6 && j>=game->colonne/2-5 && j<=game->colonne/2+4){
				if(game->gamemode==2)
					game->plat[i][j]=6;
				else 
					game->plat[i][j]=0;
			}
			else if(i==game->ligne/2+11 && j>=game->colonne/2-5 && j<=game->colonne/2+4){
				if(game->gamemode==2)
					game->plat[i][j]=7;
				else 
					game->plat[i][j]=0;
			}
			else if(j==game->colonne/2-9 && i>=game->ligne/2-2 && i<=game->ligne/2+7){
				if(game->gamemode==2)
					game->plat[i][j]=8;
				else 
					game->plat[i][j]=0;
			}
			else if(j==game->colonne/2+8 && i>=game->ligne/2-2 && i<=game->ligne/2+7){
				if(game->gamemode==2)
					game->plat[i][j]=9;
				else 
					game->plat[i][j]=0;
			}
			else if(j==0 && i>=game->ligne/2-2 && i<=game->ligne/2+7){
				if(game->gamemode==2)
					game->plat[i][j]=10;
				else 
					game->plat[i][j]=4;
			}
			else if(j==game->colonne-1 && i>=game->ligne/2-2 && i<=game->ligne/2+7){
				if(game->gamemode==2)
					game->plat[i][j]=11;
				else 
					game->plat[i][j]=4;
			}
			else if(i>=game->ligne/2-6 && i<=game->ligne/2+11 && j>=game->colonne/2-9 && j<=game->colonne/2+8){
				if(game->gamemode==2)
					game->plat[i][j]=4;
				else 
					game->plat[i][j]=0;
			}
			else if(i>5 && i!=game->ligne-1 && j!=0 && j!=game->colonne-1){
				game->plat[i][j]=0;
			}
			else{
				game->plat[i][j]=4;
			}
		}
	}
	printf("testv1.1\n");
	snake=SpawnSerpent(game);
	Direction direction;
	direction.ligne=0;
	direction.colonne=1;
	game->tour=0;
	game->start=0;
	printf("testv1\n");
	generateFeed(game,game->nbpommestot);
	generateTrap(game,game->trap);
	printf("testv2\n");
	printinit(game);
	draw(game);
	int go_on=1;
	while(go_on){
		usleep(game->time);
		gererEvenement(snake,game,&direction);
	}
}

void jeuxdebase(Menu* menue,int score){
	menue->ligne=40;
	menue->colonne=60;
	menue->gamemode=1;
	menue->nbpommes=5;
	menue->trap=0;
	menue->coeff=3;
	menue->taille=10;
	menue->score=score;
}