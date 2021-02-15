#include "Snake.h"

void print(Jeux *game){

    couleur c;
    c=CouleurParComposante(87,138,52);
    ChoisirCouleurDessin(c);
    DessinerRectangle(50,40,50,50);
    RemplirRectangle(50,40,50,50);

    DessinerRectangle(345,40,50,50);
    RemplirRectangle(345,40,50,50);

    DessinerRectangle(435,40,50,50);
    RemplirRectangle(435,40,50,50);

    char buf[100];
    AfficherSprite(2,0,30);
    snprintf(buf,100,"%d",game->nbpommes);
    c = CouleurParNom("black");
    ChoisirCouleurDessin(c);
    EcrireTexte(60,70,buf,2);

    AfficherSprite(3,100,30);
    Litscore(game);
    snprintf(buf,100,"%d",game->highscore);

    c = CouleurParNom("black");
    ChoisirCouleurDessin(c);
    EcrireTexte(160,70,buf,2);

    snprintf(buf,100,"Score: %d",game->score);
    c = CouleurParNom("black");
    ChoisirCouleurDessin(c);
    EcrireTexte(250,70,buf,2);

    if(game->gamemode==1 || game->gamemode==2){
        snprintf(buf,100,"Level: %d",game->lvl);
        c = CouleurParNom("black");
        ChoisirCouleurDessin(c);
        EcrireTexte(440,70,buf,2);
    }
}

void printinit(Jeux* game){
    /*for(int i=0;i<game->ligne;i++){
        for(int j=0;j<game->colonne;j++){
            printf("%d",game->plat[i][j]);
        }
        printf("\n");
    }*/
}

void Inscritscore(Jeux *game)
{
    FILE* fichier = NULL;

    fichier = fopen("test.txt", "a");

    if (fichier != NULL)
    {
        // On peut lire et écrire dans le fichier
        fprintf(fichier, "%d\n", game->score);
        fclose(fichier);
    }
    else
    {
        // On affiche un message d'erreur si on veut
        printf("Impossible d'ouvrir le fichier test.txt");
    }
}

void Litscore(Jeux *game)
{
	FILE* fichier = NULL;
 
    fichier = fopen("test.txt", "r");
 	int score = 0;
 	int tmp;
 	int caractereActuel = 0;
    if(fichier != NULL)
    {
    	while (caractereActuel != EOF)
    	{
    		caractereActuel = fgetc(fichier);
    		fscanf(fichier, "%d",&tmp);
    		if (tmp >=score)
    		{
    			score = tmp;
    		}
    	}
    	game->highscore = score;
        fclose(fichier);
    }
}