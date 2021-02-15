#include "Snake.h"

int Entree(int taille,int x, int y)
{
	printf("Pos : x %d\n",x);
	int ajout = x;
	int N = 0,tmp=1,t=-1;
	int tab[4]={0};
	int count = 0;
	couleur c;
	couleur b ;
		c = CouleurParNom("black");
		ChoisirCouleurDessin(c);

		couleur e;
		e = CouleurParNom("red");
	while (t != XK_KP_Enter && t != XK_Return )
	{
		
		t=Touche();
			if (count >= taille)
			{
				ChoisirCouleurDessin(CouleurParNom("white"));
				RemplirRectangle(x,y-30,200,50);
				ChoisirCouleurDessin(e);
				EcrireTexte(x,y,"Taille Invalide",2);
				
			}

			if(t==XK_KP_0)
			{
				tab[count] =0;
				printf("test :0\n");
				EcrireTexte(ajout,y,"0",2);
			}

			if(t==XK_KP_1)
			{
				tab[count] =1;
				printf("test :1\n");
				EcrireTexte(ajout,y,"1",2);
			}

			if(t==XK_KP_2)
			{
				tab[count] =2;
				printf("test :2\n");
				EcrireTexte(ajout,y,"2",2);
			}

			if(t==XK_KP_3)
			{
				tab[count] =3;
				printf("test :3\n");
				EcrireTexte(ajout,y,"3",2);
			}

			if(t==XK_KP_4)
			{
				tab[count] =4;
				printf("test :4\n");
				EcrireTexte(ajout,y,"4",2);
			}

			if(t==XK_KP_5)
			{
				tab[count] =5;
				printf("test :5\n");
				EcrireTexte(ajout,y,"5",2);
			}

			if(t==XK_KP_6)
			{
				tab[count] =6;
				printf("test :6\n");
				EcrireTexte(ajout,y,"6",2);
			}

			if(t==XK_KP_7)
			{
				tab[count] =7;
				printf("test :7\n");
				EcrireTexte(ajout,y,"7",2);
			}

			if(t==XK_KP_8)
			{
				tab[count] =8;
				printf("test :8\n");
				EcrireTexte(ajout,y,"8",2);

			}

			if(t==XK_KP_9)
			{
				tab[count] =9;
				printf("test :9\n");
				EcrireTexte(ajout,y,"9",2);

			}
			++count;
			ajout += 15;
			printf("Ajout: %d\n",ajout);
			
		}
		for(tmp=count-2,t=1;tmp!=-1;tmp--,t*=10)
			N+=tab[tmp]*t;
		return(N);
	}

void EcritureOption()
{
	couleur c;
	c = CouleurParNom("white");
	ChoisirCouleurDessin(c);
	ChargerImageFond("backgroundmenu.jpg");
	//EffacerEcran(CouleurParNom("grey"));
	EcrireTexte(290,100,"Difficulte",2);

	EcrireTexte(100,150,"Facile",2);
	//DessinerRectangle(75,115,140,50);


	EcrireTexte(300,150,"Moyen",2);
	//DessinerRectangle(275,115,140,50);

	EcrireTexte(500,150,"Difficile",2);
	//DessinerRectangle(475,115,140,50);

	EcrireTexte(30,820,"Retour",2);
	//DessinerRectangle(10,790,140,50);

	EcrireTexte(500,820,"Jouer",2);
	//DessinerRectangle(490,790,140,50);

	EcrireTexte(320,250,"Mode de jeu",2);

	EcrireTexte(50,300,"Avec Bord",2);
	//DessinerRectangle(40,270,200,50);

	EcrireTexte(300,300,"Sans Bord",2);
	//DessinerRectangle(290,270,200,50);

	EcrireTexte(530,300,"Infini",2);
	//DessinerRectangle(520,270,100,50);

	EcrireTexte(50,400,"Nombre de lignes",2);
	DessinerRectangle(50,400,200,50);
	RemplirRectangle(50,400,200,50);

	EcrireTexte(375,400,"Nombre de colonnes",2);
	DessinerRectangle(375,400,200,50);
	RemplirRectangle(375,400,200,50);

	EcrireTexte(50,550,"Nombre de pommes",2);
	DessinerRectangle(50,550,200,50);
	RemplirRectangle(50,550,200,50);

	EcrireTexte(375,550,"Nombre d'obstacles",2);
	DessinerRectangle(375,550,200,50);
	RemplirRectangle(375,550,200,50);

	EcrireTexte(290,700,"Taille",2);
	DessinerRectangle(250,715,200,50);
	RemplirRectangle(250,715,200,50);


}



int EcriteMenuBase(Menu* menue)
{

	EffacerEcran(CouleurParNom("white"));
	ChargerImageFond("backgroundmenu.jpg");
    couleur b;
    b = CouleurParNom("black");
    ChoisirCouleurDessin(b);
	EcrireTexte(300,100,"Menu",2);

	EcrireTexte(300,250,"Jouer",2);
	//DessinerRectangle(295,220,100,50);

	EcrireTexte(300, 325,"Option",2);
	//DessinerRectangle(295,290,100,50);

	EcrireTexte(300,400,"Fermer",2);
	//DessinerRectangle(295,370,120,50);

    /*EcrireTexte(300,400,"Fermer",2);
    DessinerRectangle(295,370,120,50);*/

    char buf[100];
    couleur d;
    snprintf(buf,100,"Dernier Score: %d",menue->score);
    d = CouleurParNom("black");
    ChoisirCouleurDessin(d);
    EcrireTexte(100,500,buf,2);

	int change = 0;
	while (!change)
	{
    	if (SourisCliquee())
    	{
    		SourisPosition();
    		if ((_X>=295 && _X<=395) && (_Y>=220 && _Y<=270))
    		{
    			printf("yes\n");
    			couleur c;
    			c= CouleurParNom("white");
    			ChoisirCouleurDessin(c);
    			EcrireTexte(300,250,"Jouer",2);
    			change = 1;
    			return(change);
    		}

    		if ((_X>=295 && _X<=395) && (_Y>=290 && _Y<=340))
    		{
    			couleur c;
    			c= CouleurParNom("white");
    			ChoisirCouleurDessin(c);
    			EcrireTexte(300,325,"Option",2);
    			ChargerImageFond("backgroundmenu.jpg");
    			EcritureOption();
    			change = 2;
    		}
    		if ((_X>=295 && _X<=370) && (_Y>=375 && _Y<=430))
    		{
    			printf("yes\n");
    			couleur c;
    			c= CouleurParNom("white");
    			ChoisirCouleurDessin(c);
    			EcrireTexte(300,400,"Fermer",2);
    			change = 3;
    			return(change);
    		}

    	}
    }
    while (change == 2)
    {
    	if (SourisCliquee())
    	{
    		SourisPosition();
    		//Difficulté
    		if (_X>= 75 && _X <= 215 && _Y >= 115 && _Y<=165){
    			EcritureOption();
    			couleur c;
    			c= CouleurParNom("red");
    			ChoisirCouleurDessin(c);
    			EcrireTexte(100,150,"Facile",2);
    			menue->coeff = 1;
    			printf("Coeff : %d\n",menue->coeff);
    		}
    		else if (_X>= 275 && _X <= 415 && _Y >= 115 && _Y<=165)
    		{
    			EcritureOption();
    			couleur c;
    			c= CouleurParNom("red");
    			ChoisirCouleurDessin(c);
    			EcrireTexte(300,150,"Moyen",2);
    			menue->coeff = 5;
    			printf("Coeff : %d\n",menue->coeff);
    		}
    		else if (_X>= 475 && _X <= 615 && _Y >= 115 && _Y<=165)
			{
				EcritureOption();
    			couleur c;
    			c= CouleurParNom("red");
    			ChoisirCouleurDessin(c);
    			EcrireTexte(500,150,"Difficile",2);
    			menue->coeff = 10;
    			printf("Coeff : %d\n",menue->coeff);
    		} 

    		//mode de jeu
    		if (_X>=40 && _X<= 240 && _Y>=270 && _Y<=320){
    			EcritureOption();
    			couleur c;
    			c= CouleurParNom("red");
    			ChoisirCouleurDessin(c);
    			EcrireTexte(50,300,"Avec Bord",2);
    			menue->gamemode = 1;
    			printf("Gamemode : %d\n",menue->gamemode);
    		}
    		else if (_X>= 290 && _X <= 490 && _Y >= 270 && _Y<=320)
    		{
    			EcritureOption();
    			couleur c;
    			c= CouleurParNom("red");
    			ChoisirCouleurDessin(c);
    			EcrireTexte(300,300,"Sans Bord",2);
    			menue->gamemode = 2 ;
    			printf("Gamemode : %d\n",menue->gamemode);
    			//Entree(pomme,5);
    		}

    		else if(_X>= 520 && _X <= 620 && _Y >= 270 && _Y<=320)
    		{
    			EcritureOption();
    			couleur c;
    			c= CouleurParNom("red");
    			ChoisirCouleurDessin(c);
    			EcrireTexte(530,300,"Infini",2);
    			menue->gamemode = 3 ;
    			printf("Gamemode : %d\n",menue->gamemode);
    		}

    		//Changer les parametres de lignes, colonnes, pommes et traps
    		else if (_X>=50&& _X<= 250 &&_Y>=400 && _Y<= 450)
    		{	


    			int Nbrlignes = Entree(4,50,430);
    			char ligne[4];
    			sprintf(ligne,"%d",Nbrlignes);
    			ligne[4]='\0'; 
    			menue->ligne =  Nbrlignes;
    			printf("\nNbr ligne: %d\n",menue->ligne);
    			//printf("Test: %d",Entree(3,50,430));

    		}

    		else if (_X>=375 && _X<= 675 && _Y>=400 && _Y<= 450)
    		{	
    			int Nbrcolonnes = Entree(4,375,430);
    			char colonnes[4];
    			sprintf(colonnes,"%d",Nbrcolonnes);
    			colonnes[4]='\0'; 
    			menue->colonne =  Nbrcolonnes;
    			printf("\nNbr colonnes: %d\n",menue->colonne);

    		}

    		else if (_X>=50 && _X<= 250 && _Y>=550 && _Y<= 600)
    		{	
    			int pommes = Entree(4,50,580);
    			char nbrpomme[4];
    			sprintf(nbrpomme,"%d",pommes);
    			nbrpomme[4]='\0'; 
    			menue->nbpommes =  pommes;
    			printf("\nNbr pommes: %d\n",menue->nbpommes);

    		}

    		else if (_X>=375 && _X<= 600 && _Y>=550 && _Y<= 600)
    		{	
    			int traps = Entree(4,375,580);
    			char obstacles[4];
    			sprintf(obstacles,"%d",traps);
    			obstacles[4]='\0'; 
    			menue->trap =  traps;
    			printf("\nNbr trap: %d\n",menue->trap);

    		}

    		else if (_X>=250 && _X<= 450 && _Y>=715 && _Y<= 765)
    		{	
    			int length = Entree(4,250,745);
    			char sizes[4];
    			sprintf(sizes,"%d",length);
    			sizes[4]='\0'; 
    			menue->taille =  length;
    			printf("\nNbr taille %d\n",menue->taille);

    		}

    		else if (_X>= 10 && _X <= 150 && _Y >= 790 && _Y<=840)
    		{
                FermerGraphique();
    			MenuBase(menue->score);
    		} 
    		else if (_X >= 470 && _X<= 610 && _Y>=790 && _Y<= 840)
    			return(1);
    	}
    }
}


void MenuBase(int score)
{
	InitialiserGraphique();
    CreerFenetre(10,10,700,900);
    Menu* menue= malloc(sizeof(Menu));
	jeuxdebase(menue,score);
    int menu = EcriteMenuBase(menue);
    if (menu == 1)
    	launch(menue); 
    if (menu == 3)
    	FermerGraphique();
}