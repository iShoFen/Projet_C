#include"proj.h"

int 		menu(void) // Menu principal
{
	int choix;

	system(CLEAN_SCREEN);
	printf("\n-----------------------------------------------------------------\n");
	printf("| 1.	Afficher les jeux 					|\n");
	printf("| 2.	Afficher la liste des emprunts				|\n");
	printf("| 3.	Afficher les r‚servations pour un jeu			|\n");
	printf("| 4.	Saisir un nouveau jeu 					|\n");
	printf("| 5.	Saisir un nouvel emprunt				|\n");
	printf("| 6.	Saisir une nouvelle r‚servation				|\n"); 
	printf("| 7.	Retour d'un jeu						|\n");
	printf("| 8.	Annuler une r‚servation					|\n");
	printf("| 9.	SAUVEGARDER						|\n");
	printf("| 10.	QUITTER							|\n");                  
	printf("-----------------------------------------------------------------\n");
    scanf("%d",&choix);
	
    return choix;
}


void 		dateNow(char date[]) //r‚cupŠre la date du jour en format local (france)
{
	time_t  now = time (&now);

	strftime(date, 11 * sizeof(char*), "%d/%m/%Y", localtime(&now));
}


void 		upper(char tab[]) // met toutes les lettres d'une chaine de caractŠres en maj
{
	int i;
	for (i = 0; i < strlen(tab); i++)	
		tab[i] = toupper(tab[i]);
}

void 		upperLower(char tab[]) // met la premiŠre lettre en maj et le reste en min pour une chaine de caractŠres
{
	int i;
	tab	[0] = toupper(tab[0]);
	for (i = 1; i < strlen(tab); i++)
		tab[i] = tolower(tab[i]);
}


nodeType	*createNode(jeu	game) // cr‚e un maillon de liste a partir du jeu pass‚ en paramŠtre, next = NULL pour ‚viter les erreurs de segmentation
{
	nodeType *node;
	node = (nodeType*) malloc(sizeof(nodeType));

	if (node == NULL)
	{
		printf("ProblŠme d'allocation pour la liste chainee\n");
		exit(-1);
	}

	node->game = game;
	node->next = NULL;

	return node;
}

nodeType	*lastNode(nodeType *head) // renvoie le dernier maillon d'une liste
{
	nodeType *current;

	current = head;

	if (current != NULL)
	{
		while (current->next != NULL)
			current = current->next;
		return current;
	}

	return NULL;
}

void		createLastNode(nodeType **head, jeu game) // utilise les deux fonctions ci-dessus pour cr‚er un maillon … la fin de la liste chain‚e
{
	if (*head != NULL)
		lastNode(*head)->next = createNode(game); // si *head existe, on prend le dernier maillon et on assigne a son next le maillon que l'on veut ins‚rer	
	else
		*head = createNode(game); // sinon, on cr‚e le maillon … la place du premier
}

nodeType	*ChargementJeux(nodeType *head, int *nbJeux) // utilise les trois fonctions ci-dessus pour charger le fichier 'jeux.txt' dans la liste chain‚e
{
	FILE *flot;
	jeu	currentGame;
	int	i;

	flot = fopen("jeux.bin", "rb");

	if (flot == NULL)
	{
		printf("pb d'ouverture du fichier jeux.bin en lecture\n");
		exit(1);
	}

	fread(nbJeux, sizeof(int), 1, flot);

	for (i = 0; i < *nbJeux; i++)
	{
		fread(&currentGame, sizeof(jeu), 1, flot);	
		createLastNode(&head, currentGame);
	}

	fclose(flot);

	return head;
}


int 		ChargementAdherents(adherent *tabAdh[], int max)  //charge le contenu du fichier adherent en m‚moire 
{
	FILE *flot;
	int i = 0, val;
	
	flot = fopen("adherents.bin","rb");

	if (flot == NULL)
	{
			printf("pb d'ouverture du fichier adherents.bin en lecture\n");
			exit(1);
	}

	fread(&val,sizeof(int),1,flot);

	if (val > max)
	{
		printf("Tableaux d'adherent trop petit");
		exit(-2);
	}
	
	while (!feof(flot))
	{
		tabAdh[i] = (adherent *) malloc(sizeof(adherent));
		if (tabAdh[i] == NULL)
		{
			printf("Probleme d'allocation en memoire … la %de place pour les adherents",i);
			exit(-1);
		}
		fread(tabAdh[i],sizeof(adherent),1,flot);
		i++;
	}
	fclose(flot);

	return val;
}


empResa		*lectureEmpResa(empResa tabEmpResa[], int *nbEmpResa, FILE *flot) // lit le fichier envoy‚ en paramŠtre et remplis le tableau
{
	fread(tabEmpResa,sizeof(emprunt),*nbEmpResa,flot);
	fclose(flot);

	return tabEmpResa;
}

emprunt		*ChargementEmprunt(int *nbEmp, int *idEmp) // charge le contenu du fichier emprunt en m‚moire
{
	emprunt *tabEmp;
	int i;
	FILE *flot;

	flot = fopen("emprunt.bin","rb");
	if (flot == NULL)
	{	
		printf("ProblŠme d'ouverture du fichier emprunt.bin en lecture");
		exit(1);
	}
	
	fread(nbEmp,sizeof(int),1,flot);

	tabEmp = (emprunt *) malloc(*nbEmp * sizeof (emprunt));

	if (tabEmp == NULL)
	{
			printf("ProblŠme d'allocation en m‚moire pour les emprunts");
			exit(-1);
	}

	tabEmp = lectureEmpResa(tabEmp, nbEmp, flot);
	*idEmp = tabEmp[*nbEmp-1].idEmpResa;
	return tabEmp;
}

reservation *ChargementReservation(int *nbResa, int *idResa) // charge le contenu du fichier reservation en m‚moire
{
	reservation *tabResa;
	int i;
	FILE *flot;
	
	flot = fopen("reservation.bin","rb");

	if (flot == NULL)
	{	
		printf("ProblŠme d'ouverture du fichier reservation.bin en lecture");
		exit(1);
	}

	fread(nbResa,sizeof(int),1,flot);

	tabResa = (reservation *) malloc(*nbResa * sizeof (reservation));

	if (tabResa == NULL)
	{
			printf("ProblŠme d'allocation en m‚moire pour les reservations");
			exit(-1);
	}

	tabResa = lectureEmpResa(tabResa, nbResa, flot);
	*idResa = tabResa[*nbResa-1].idEmpResa;
	return tabResa;
}


nodeType 	*copyList(nodeType *head) // renvoie une liste copi‚e a partir de celle pass‚e en paramŠtre
{ 
	nodeType *newNode;

    if (head == NULL) 
        return NULL; 
    else 
	{
		newNode = (nodeType*) malloc(sizeof(nodeType)); 
        newNode->game = head->game;
        newNode->next = copyList(head->next);
        return newNode; 
    } 
} 

int			comptNbEmp(emprunt tabEmp[], int idJeu, int nbEmp) // compte le nombre d'emprunts en cours d'un jeu donn‚ en paramŠtre
{
	int i, nbEmpJeu = 0;

	for (i = 0; i < nbEmp; i++)
	{
		if (tabEmp[i].idJeu == idJeu)
			nbEmpJeu++;
	}
	
	return nbEmpJeu;
}

void		printJeu(nodeType* head, emprunt tabEmp[], int nbEmp) // affiche une liste de jeux
{
	nodeType* currentNode = head;
	jeu	currentGame;
	int k = 0;

	system(CLEAN_SCREEN);
	printf("\nID JEU |            NOM            |      TYPE      | NB EXEMPL\n");

    while (currentNode != NULL) 
	{
		currentGame = currentNode->game;
		currentGame.nbExp -= comptNbEmp(tabEmp, currentGame.idJeu, nbEmp);
		if (currentGame.nbExp > 0)
		{
			printf("%-6d | %-25s | %-14s | %d\n", currentGame.idJeu, currentGame.nom, currentGame.type, currentGame.nbExp);
			k++;
		}
		currentNode = currentNode->next;
    }
	if (k == 0)
		printf("\ntous les jeux sont actuellement emprunt‚s.\n");	
}

void		freeListe(nodeType *head) // libŠre la liste chain‚
{
	nodeType *currentNode = head;
	jeu	currentGame;
	int	nbEmpCurrent;
	
	while (currentNode != NULL) 
	{
		currentGame = currentNode->game;
		free(currentNode);
		currentNode = currentNode->next;
    }
}

void    	triTypeJeu(nodeType *head, emprunt tabEmp[], int nbEmp) // affiche les jeux disponibles tri‚s par type (FONCTION Nø1.2 du menu)
{
    nodeType  	*current;
    jeu    		temp_jeu;

    current = head;

    while (current != 0 && current->next != 0)
    {
        if (strcmp(current->game.type, current->next->game.type) > 0)
        {
            temp_jeu = current->game;
            current->game = current->next->game;
            current->next->game = temp_jeu;
            current = head;
        }

        else
            current = current->next;
    }

	printJeu(head, tabEmp, nbEmp);
}

void    	triAlphaJeu(nodeType *head, emprunt tabEmp[], int nbEmp) // affiche les jeux disponibles dans l'ordre alphab‚tique (FONCTION Nø1.1 du menu)
{
    nodeType  	*current;
	nodeType	*headbis;
    jeu    		temp_jeu;

	headbis = copyList(head);
 
    if (headbis == NULL)
	{
		printf("\nProblŠme de copie de la liste de jeux\n");
		exit(-4);
	}
    current = headbis;

    while (current != 0 && current->next != 0)
    {
        if (strcmp(current->game.nom, current->next->game.nom) > 0)
        {
            temp_jeu = current->game;
            current->game = current->next->game;
            current->next->game = temp_jeu;
            current = headbis;
        }

        else
            current = current->next;
    }

	triTypeJeu(headbis,tabEmp,nbEmp);

	freeListe(headbis);
}

void		printAllJeu(nodeType* head, emprunt tabEmp[], int nbEmp) // affiche une liste de jeux
{
	nodeType*	currentNode = head;
	jeu			currentGame;

	system(CLEAN_SCREEN);
	printf("\nID JEU |            NOM            |      TYPE      | NB EXEMPL\n");

    while (currentNode != NULL) 
	{
		currentGame = currentNode->game;
		currentGame.nbExp -= comptNbEmp(tabEmp, currentGame.idJeu, nbEmp);
		printf("%-6d | %-25s | %-14s | %d", currentGame.idJeu, currentGame.nom, currentGame.type, currentGame.nbExp);
		if(currentGame.nbExp < 1)
			printf("\tCe jeu n'est plus disponible\n");
		else
			printf("\n");
		currentNode = currentNode->next;
    }
}

int			menuJeu(void) // menu affichage choix jeu
{
	int choix;

	system(CLEAN_SCREEN);
	printf("\n-----------------------------------------------------------------\n");
	printf("| 1.	Afficher les jeux disponibles				|\n");
	printf("| 2.	Afficher tous les jeux 					|\n");
	printf("| 3.	QUITTER							|\n");
	printf("-----------------------------------------------------------------\n");
    scanf("%d",&choix);

    return choix;
}


int 		dateEmp(emprunt tabEmp[], char date[], int i) // v‚rifie la date d'emprunt
{
	int ji, jn, mi, mn, ai, an;
	
	sscanf(date, "%d/%d/%d", &jn,&mn,&an);
	sscanf(tabEmp[i].dateEmpResa, "%d/%d/%d", &ji, &mi, &ai);

	if (ai < an)
	{
		if (mn > 1 || mi < 12)
			return -1;
		else if (ji < jn)
			return -1;
	}
	else if (mi < mn)
	{	
			if (ji < jn)
				return -1;
			else if (mi-mn <-1)
				return -1;
	}
}

void 		printEmprunt(emprunt tabEmp[], char date[], int max) // affiche tout les emprunt en cours (FONCTION Nø 2)
{
	int i, exp;

	system(CLEAN_SCREEN);
	printf("ID EMPRUNT | ID ADHERENT | ID JEU | DATE EMPRUNT\n");

	for (i = 0; i<max; i++, exp = 0)
	{
		printf("%-10d | %-11d | %-6d | %s", tabEmp[i].idEmpResa, tabEmp[i].idAdherent, tabEmp[i].idJeu, tabEmp[i].dateEmpResa);
		if (dateEmp(tabEmp, date, i) == -1)
			printf("\tRetard de l'emprunt");
		printf("\n");
	}
}

void 		printResa(reservation *tabResa, nodeType *head, int nbResa, int nbJeux) // affiche toute les reservation en cours en focntion du jeux donn‚ (FONCTION Nø 3)
{
	int i, idJeu, trouve, k = 0;
	char nomJeu[25];

	system(CLEAN_SCREEN);
	printf("Quelle(s) res‚rvation(s) voulez-vous afficher ? ");
	scanf("%s%*c", nomJeu);
	upperLower(nomJeu);

	idJeu = rechercheJeu(head, nomJeu, nbJeux, &trouve);

	if (trouve == -1)
	{
		printf("Le jeu recherch‚ n'existe pas, retour au menu ...\n");
		return;
	}

	else
	{
		for (i = 0; i < nbResa; i++)
			if (tabResa[i].idJeu == idJeu)
			{
				k++;
				if (k == 1)
					printf("\nID RESERVATION | ID ADHERENT | ID JEU | DATE EMPRUNT\n");
				printf("%-14d | %-11d | %-6d | %s\n", tabResa[i].idEmpResa, tabResa[i].idAdherent, tabResa[i].idJeu, tabResa[i].dateEmpResa);
			}

		if (k == 0)
			printf("\nAucune r‚servation pour ce jeu, retour au menu ...\n");
	}
}


nodeType	*saisieNouveauJeu(nodeType *head, int *nbJeux) // cr‚e un nouveau jeu
{
	jeu game;
	char nom[25];
	
	system(CLEAN_SCREEN);
	printf("Entrez un nom, un type, un nb d'exemplaire : ");	
	
	scanf("%s%s%d", game.nom, game.type, &game.nbExp);
	game.idJeu = *nbJeux+1;
	upperLower(game.nom);
	upperLower(game.type);

	createLastNode(&head, game);
	*nbJeux+= 1;
	printf("\nJeu bien ajout‚.\n");
	
	return head;
}


int			rechercheAdherent(adherent *tabAdh[], char nom[], int nbAdh) // permet de savoir si l'adh‚rent existe ou non
{
	int i;
	for (i = 0; i < nbAdh; i++)
	{
		if (strcmp(nom,tabAdh[i]->nom) == 0)
			return i;
	}
	return -1;
}

void 		nouvelAdherent(adherent *tabAdh[], char date[], int *nbAdh, int max) // cr‚e un nouvel adherent
{
	char civ[4], nom[15], prenom[15];

	if (*nbAdh == max)
	{
		printf("\nTableau d'adh‚rents pleins, impossible d'en ajouter un nouveau\n");
		exit(-2);
	}

	tabAdh[*nbAdh] = (adherent *) malloc (sizeof (adherent));

	if (tabAdh[*nbAdh] == NULL)
	{
		printf("\nProblŠme d'allocation m‚moire pour un nouvel adherent\n");
		exit(-1);
	}

	tabAdh[*nbAdh]->idAdherent = *nbAdh+1;
	printf("\nCivilit‚, NOM, Prenom : ");
	scanf("%s%s%s%*c", civ, nom, prenom);

	upperLower(civ);

	if (strcmp(civ, "Mr") != 0 && strcmp (civ, "Mme") != 0)
		while (strcmp(civ, "Mr") != 0 && strcmp (civ, "Mme") != 0)
		{
			printf("\nCette civilit‚ n'existe pas, veuillez choisir entre Mr ou Mme : ");
			scanf("%s%*c",civ);
			upperLower(civ);
		}

	upper(nom);
	upperLower(prenom);

	strcpy(tabAdh[*nbAdh]->civilite, civ);
	strcpy(tabAdh[*nbAdh]->nom, nom);
	strcpy(tabAdh[*nbAdh]->prenom, prenom);
	strcpy(tabAdh[*nbAdh]->dateIns, date);

	printf("\nNouvel adherent ajout‚ avec succes\n");

	*nbAdh+= 1;
}

int 		dateIns(adherent *tabAdh[], char date[], int i) // v‚rifie la validit‚ de l'adherent
{
	int ji, jn, mi, mn, ai, an;
	sscanf(date, "%d/%d/%d", &jn, &mn, &an);
	sscanf(tabAdh[i]->dateIns, "%d/%d/%d", &ji, &mi, &ai);
	
	if (ai-an == -1)
	{
		if (mi <= mn)
		{	
			if (mi-mn < -1)
				return -1;
			else if (ji < jn)
				return -1;
		}
	}
	else if (ai < an)
		return -1;		
}

int			comptNbEmpAdh(emprunt tabEmp[], int idAdh, int nbEmp) // compte le nombre d'emprunts en cours d'un jeu donn‚ en paramŠtre
{
	int i, nbEmpAdh;

	for (nbEmpAdh = 0, i = 0; i < nbEmp; i++)
	{
		if (tabEmp[i].idAdherent == idAdh)
			nbEmpAdh++;
	}
	
	return nbEmpAdh;
}

int         RetardEmp(emprunt tabEmp[], char date[], int nbEmp, int idAdh)
{
	int i;
	for (i = 0; i < nbEmp; i++)
		if (tabEmp[idAdh].idAdherent == idAdh+1 && dateEmp(tabEmp, date, i) == -1) 
			return -1;
}

int			rechercheJeu(nodeType* head, char name[25], int nbJeux, int *trouve) // permet de savoir si un jeu existe ou non
{
	int i = 0;
	nodeType*	currentNode = head;
	jeu			currentGame;

    while (i < nbJeux) 
	{
		currentGame = currentNode->game;
		if (strcmp(currentGame.nom, name) == 0)
		{
			*trouve = currentGame.nbExp;
			return currentGame.idJeu;	
		}
		currentNode = currentNode->next;
		i++;
    }

	*trouve = -1;
	return -1;
}

int			rechercheJeuxAct(empResa tabEmpResa[], int idJeu, int idAdh, int nbEmp) // recherche si un adherent … d‚j… un emprunt ou r‚servation du jeu
{
	int i;

	for (i = 0; i < nbEmp; i++)
		if (tabEmpResa[i].idJeu == idJeu && tabEmpResa[i].idAdherent == idAdh)
			return 1;

	return 0;
}

empResa		*ajoutEmpResa(empResa tabEmpResa[], adherent *tabAdh[], int *nb,  int *id, int *nbMax, char date[], int i, int idJeu) // ajoute un/e emprunt/reservation en fonction de ce qui lui est pass‚ en paramŠtre
{
	empResa *newtab;

	if (*nb == *nbMax)
	{
		*nbMax+= 5;
		newtab = (empResa *) realloc(tabEmpResa, *nbMax * sizeof (empResa));
		if (newtab == NULL)
		{
			printf("\nProblŠme de r‚allocation\n");
			exit(-3);
		}
		tabEmpResa = newtab;
	}

	tabEmpResa[*nb].idEmpResa = *id+1;
	tabEmpResa[*nb].idAdherent = tabAdh[i]->idAdherent;
	tabEmpResa[*nb].idJeu = idJeu;
	strcpy(tabEmpResa[*nb].dateEmpResa, date);

	*id+= 1;
	*nb+= 1;
	printf("\nnouvel ajout effectu‚\n");

	return tabEmpResa;
}

emprunt 	*saisieNouvelEmprunt(adherent *tabAdh[], emprunt tabEmp[], nodeType* head, char date[], int *nbAdh, int *nbEmp, int *idEmp, int *nbEmpmax, int nbJeux, int max) // saisie les informations pour un nouvel emprunt (FONCTION Nø 2)
{
	char nom[15], jeu[25], choix[4];
	int idAdh, idJeu, trouve;

	system(CLEAN_SCREEN);
	printf("Entrez le nom de l'emprunteur : ");
	scanf("%s%*c", nom);
	upper(nom);

	idAdh = rechercheAdherent(tabAdh, nom, *nbAdh);
		
	if (idAdh == -1)
	{
		printf("cette personne n'a jamais ‚t‚ enregistr‚e, voulez vous l'ajouter ? (oui/non) : ");
		scanf("%s%*c", choix);
		if (strcmp(choix, "oui") == 0)
		{
			printf("Cette personne a-t-elle pay‚ sa cotisation ? (oui/non) : ");
			scanf("%s%*c", choix);
			if (strcmp(choix, "oui") == 0)
				nouvelAdherent(tabAdh, date, nbAdh, max);
			else
				return tabEmp;
		}
		else
			return tabEmp;
	}
	else
	{		
		if (dateIns(tabAdh, date, idAdh) == -1)
		{
			printf("\nL'adh‚rent %s %s est arriv‚ … expiration, a-t-il pay‚ sa cotisation ? (oui/non) : ",  tabAdh[idAdh]->nom, tabAdh[idAdh]->prenom);
			scanf("%s", choix);
			if (strcmp(choix, "oui") == 0)	
				strcpy(tabAdh[idAdh]->dateIns, date); 
			else
				return tabEmp;
		}
		else
		{	
			if (comptNbEmpAdh(tabEmp, idAdh+1, *nbEmp) == 3)
			{
				printf("\nL'adh‚rent %s %s a d‚j… 3 emprunts, retour au menu ...\n", tabAdh[idAdh]->nom, tabAdh[idAdh]->prenom);
				return tabEmp;
			}
			else if (RetardEmp(tabEmp, date, *nbEmp, idAdh+1) == -1)
				
			{
				printf("\nL'adh‚rent %s %s a un emprunt de plus de 1 mois, retour au menu ...\n",tabAdh[idAdh]->nom, tabAdh[idAdh]->prenom);
				return tabEmp;
			}
		}	
	}

	Jeu :
	printf("\nEntrez le nom du jeu  … emprunter : ");
	scanf("%s%*c", jeu);
	upperLower(jeu);

	idJeu = rechercheJeu(head, jeu, nbJeux, &trouve);

	if (trouve == -1)
	{
		printf("\nCe jeu n'existe pas, voulez vous en choisir un autre ? (oui/non) : ");
		scanf("%s", choix);
			if (strcmp(choix, "oui") == 0)	
				goto Jeu;
			else
				return tabEmp;
	}
	else
	{	
		if (rechercheJeuxAct(tabEmp, idJeu, idAdh+1, *nbEmp) == 1)
		{
			printf("Cet adherent … d‚j… emprunt‚ ce jeu, voulez vous en choisir un autre ? (oui/non) : ");
			scanf("%s", choix);
			if (strcmp(choix, "oui") == 0)	
				goto Jeu;
			else
				return tabEmp;
		}
		else
		{
			if (trouve-comptNbEmp(tabEmp, idJeu, *nbEmp) == 0)
			{
				printf("Ce jeu n'est plus disponible, veuillez en faire une r‚servation, voulez vous en choisir un autre ? (oui/non) : ");
				scanf("%s", choix);
				if (strcmp(choix, "oui") == 0)	
					goto Jeu;
				else
					return tabEmp;
			}
			else
				return ajoutEmpResa(tabEmp, tabAdh, nbEmp, idEmp,nbEmpmax, date, idAdh, idJeu);
		}
	}
}

reservation *saisieNouvelResa(adherent *tabAdh[], reservation tabResa[], emprunt tabEmp[], nodeType* head, char date[], int *nbAdh, int *nbResa,  int *idResa, int *nbResaMax, int nbEmp, int nbJeux, int max) // saisie les informations pour une nouvelle reservation (FONCTION Nø 3)
{
	char nom[15], jeu[25], choix[4];
	int idAdh, idJeu, trouve;

	system(CLEAN_SCREEN);
	printf("Entrez le nom de l'adh‚rent souhaitant r‚server : ");
	scanf("%s%*c",nom);
	upper(nom);

	idAdh = rechercheAdherent(tabAdh, nom, *nbAdh);
		
	if (idAdh == -1)
	{
		printf("cette personne n'a jamais ‚t‚ enregistr‚e, voulez vous l'ajouter ? (oui/non) : \n");
		scanf("%s%*c", choix);
		if (strcmp(choix, "oui") == 0)
		{	
			printf("Cette personne a-t-elle pay‚ sa cotisation ? (oui/non) : ");
			scanf("%s%*c", choix);
			if (strcmp(choix, "oui") == 0)
				nouvelAdherent(tabAdh, date, nbAdh, max);
			else
				return tabResa;
		}
		else
			return tabResa;
	}
	else
	{
		if (dateIns(tabAdh, date, idAdh) == -1)
		{
			printf("L'adherent %s %s est arriv‚ … expiration, a-t-il pay‚ sa cotisation ? (oui/non) : \n\n",  tabAdh[idAdh]->nom, tabAdh[idAdh]->prenom);
			scanf("%s", choix);
			if (strcmp(choix, "oui") == 0)
			{	
				strcpy(tabAdh[idAdh]->dateIns, date);
			}
			else
				return tabEmp;
		}
		else if (RetardEmp(tabEmp, date, nbEmp, idAdh+1) == -1)		
		{
			printf("\nL'adh‚rent %s %s a un emprunt de plus de 1 mois, retour au menu ...\n",tabAdh[idAdh]->nom, tabAdh[idAdh]->prenom);
			return tabResa;
		}
	}
	
	Jeu :
	printf("Entrez le nom du jeu … reserver : ");
	scanf ("%s%*c", jeu);
	upperLower (jeu);

	idJeu = rechercheJeu(head, jeu, nbJeux, &trouve);

	if (trouve == -1)
	{
		printf("Ce jeu n'existe pas, voulez vous en choisir un autre ? (oui/non) : ");
		scanf("%s", choix);
			if (strcmp(choix, "oui") == 0)	
				goto Jeu;
			else
				return tabEmp;
	}
	else
	{	
		if (rechercheJeuxAct(tabResa, idJeu, idAdh+1, *nbResa) == 1)
		{
			printf("Cet adherent a d‚j… r‚serv‚ ce jeu, voulez vous en choisir un autre ? (oui/non) : ");
			scanf("%s", choix);
			if (strcmp(choix, "oui") == 0)	
				goto Jeu;
			else
				return tabEmp;
		}
		else
		{
			if (trouve-comptNbEmp(tabEmp, idJeu, nbEmp) > 0)
			{
				printf("Ce jeu est disponible, veuillez en faire un emprunt.\n");
				return tabResa;
			}
			else
				return ajoutEmpResa(tabResa, tabAdh, nbResa, idResa,nbResaMax, date, idAdh, idJeu);
		}
	}
}


int			printEmpResa(empResa tabEmpResa[], int nbEmpResa, int id) // affiche les emprunt ou r‚servations d'un adherent.
{
	int j, k;

	for (k = 0, j = 0; j < nbEmpResa; j++)
		if (tabEmpResa[j].idAdherent == id)
		{
			k++;
			if (k ==1)
				printf("\nID EMPRESA | ID ADHERENT | ID JEU | DATE EMPRUNT\n");
			printf("%-10d | %-11d | %-6d | %s\n", tabEmpResa[j].idEmpResa, tabEmpResa[j].idAdherent, tabEmpResa[j].idJeu, tabEmpResa[j].dateEmpResa);	
		}	
	printf("\n");
	return k;
}

empResa 	*decalerGauche(empResa tabEmpResa[], int *nbEmpResa, int val) // d‚caler un tableau … gauche
{
	while (val < *nbEmpResa-1)
	{
		tabEmpResa[val] = tabEmpResa[val+1];
		val++; 
	}

	*nbEmpResa-=1;
	
	return tabEmpResa;
}

int			rechercheResa(reservation tabResa[], int nbResa, int idjeu) // recherche si une r‚servation corespond au nom du jeu
{
	int i;
	for (i = 0; i < nbResa ; i++)
		if (tabResa[i].idJeu == idjeu)
			return i;
	return -1;
}

emprunt		*RetourJeu(adherent *tabAdh[], emprunt tabEmp[], emprunt tabResa[], int *nbEmp, int *idEmp, int *nbResa, int nbAdh, char date[]) // retourne un jeu et regarde si une reservation peu passer en emprunt (FONCTION Nø6)
{
	int idAdh, idResa, val, exp = 0;
	int idjeu;
	char nom[15];

	system(CLEAN_SCREEN);
	printf("Entrez le nom de l'emprunteur : ");
	scanf("%s", nom);
	upper(nom);

	idAdh = rechercheAdherent(tabAdh, nom, nbAdh);

	if (idAdh == -1)
	{
		printf("\ncette personne n'a jamais ‚t‚ enregistr‚e, retour au menu ...\n");
		return tabEmp;
	}
	else 
	{
		if (printEmpResa(tabEmp, *nbEmp, idAdh+1) == 0)
		{
			printf("Aucun emprunt en cours pour cet adh‚rent\n");
			return tabEmp;
		}
		else
		{
			printf("Choissisez un num‚ro d'emprunt pour cet adh‚rent : ");
			scanf("%d", &val);

			if (tabEmp[val-1].idAdherent != idAdh+1)	
				while (tabEmp[val-1].idAdherent != idAdh+1)
				{
					printf("Votre numero d'emprunt pour cet adherent n'est pas correct veuillez resaisir : ");
					scanf("%d", &val);
				}

			idjeu = tabEmp[val-1].idJeu;
			tabEmp = decalerGauche(tabEmp, nbEmp, val-1);
			printf("\nRetour bien effectu‚\n");
			
			idResa = rechercheResa(tabResa, *nbResa, idjeu);
			if ( idResa != -1)
			{	
				if (comptNbEmpAdh(tabEmp, tabResa[idResa].idAdherent, *nbEmp) < 3 && dateIns(tabAdh, date, idAdh) != -1  && RetardEmp(tabEmp, date, *nbEmp, idAdh+1) != -1)
				{
					tabEmp[*nbEmp].idEmpResa = *idEmp+1;
					tabEmp[*nbEmp].idAdherent = tabResa[idResa].idAdherent;
					tabEmp[*nbEmp].idJeu = tabResa[idResa].idJeu;
					strcpy(tabEmp[*nbEmp].dateEmpResa, date);

					*idEmp+= 1;
					*nbEmp+= 1;
					printf("\nLa r‚servation num‚ro %d au nom de %s %s est pass‚e en emprunt\n", tabResa[idResa].idEmpResa, tabAdh[(tabResa[idResa].idAdherent)-1]->nom, tabAdh[(tabResa[idResa].idAdherent)-1]->prenom);
					tabResa = decalerGauche(tabResa, nbResa, idResa);
				}
			}
		}		
	}
	return tabEmp;	
}

reservation	*AnnulationResa(adherent *tabAdh[], reservation tabResa[], int *nbResa, int nbAdh) // Annule une reservation (FONCTION Nø7)
{
	int idAdh, j, k = 0, val, test = 0;
	char nom[15];
	
	system(CLEAN_SCREEN);
	printf("Entrez le nom de l'emprunteur qui souhaite annuler une r‚servation : ");
	scanf("%s", nom);
	upper(nom);

	idAdh = rechercheAdherent(tabAdh, nom, nbAdh);

	if (idAdh == -1)
	{
		printf("\ncette personne n'a jamais ‚t‚ enregistr‚e, retour au menu ...\n");
		return tabResa;
	}
	else
	{
		if (printEmpResa(tabResa, *nbResa, idAdh+1) == 0)
			{
				printf("Aucun emprunt en cours pour cet adh‚rent\n");
				return tabResa;
			}
		else
		{		
			printf("Choissisez un numero de r‚servation pour cet adherent : ");
			scanf("%d", &val);

			if (tabResa[val-1].idAdherent != idAdh+1)	
				while (tabResa[val-1].idAdherent != idAdh+1)
				{
					printf("Votre num‚ro de r‚servation n'est pas correct pour cet adherent veuillez resaisir : ");
					scanf("%d", &val);
				}

			tabResa = decalerGauche(tabResa, nbResa, val-1);
			printf("Annulation bien effectu‚e\n");

			return tabResa;
		}	
	}
}	


void 		saveJeux(nodeType *head, int nbJeux) //sauvegarde la liste de jeux dans son fichier binaire
{
	nodeType* currentNode = head;
	jeu	currentGame;
	FILE *flot;

	flot = fopen ("jeux.bin","wb");
	
	if (flot == NULL)
	{
		printf("ProblŠme d'ouverture du fichier jeux.bin en ‚criture");
		return;
	}

	fwrite(&nbJeux, sizeof(int), 1, flot);

	while (currentNode != NULL) 
	{
		currentGame = currentNode->game;
		fwrite(&currentGame, sizeof(jeu), 1, flot);
		currentNode = currentNode->next;
    }

	fclose(flot);
	printf("Sauvegarde des jeux termin‚e\n");
}

void 		saveAdh(adherent **tabAdh, int nbAdh) // sauvegarde le tableau de pointeur d'adherents dans son fichier binaire
{
	FILE *flot;
	int i;

	flot = fopen ("adherents.bin","wb");

	if (flot == NULL)
	{
		printf("ProblŠme d'ouverture du fichier adherents.bin en ‚criture");
		return;
	}

	fwrite(&nbAdh, sizeof(int), 1, flot);

	for (i = 0; i < nbAdh; i++)
		fwrite(tabAdh[i], sizeof(adherent), 1, flot);

	fclose(flot);
	printf("Sauvegarde des adh‚rents termin‚e\n");
}

void 		saveEmp(emprunt *tabEmp, int nbEmp) // sauvegarde le tableau d'emprunt dans son fichier binaire
{
	FILE *flot;

	flot = fopen("emprunt.bin","wb");

	if (flot == NULL)
	{
		printf("ProblŠme d'ouverture du fichier emprunt.bin en ‚criture");
		return;
	}

	fwrite(&nbEmp, sizeof(int), 1, flot);
	fwrite(tabEmp, sizeof(emprunt), nbEmp, flot);
	fclose(flot);

	printf("Sauvegarde des emprunts termin‚e\n");
}

void 		saveResa(reservation *tabResa, int nbResa) // sauvegarde le tableau de reservation dans son fichier binaire
{
	FILE *flot;

	flot = fopen("reservation.bin","wb");

	if (flot == NULL)
	{
		printf("ProblŠme d'ouverture du fichier reservation.bin en ‚criture");
		return;
	}

	fwrite(&nbResa, sizeof(int),1,flot);
	fwrite(tabResa, sizeof(reservation),nbResa,flot);
	fclose(flot);

	printf("Sauvegarde des r‚servations termin‚e\n");
}

void 		sauvegarder(nodeType *head, adherent **tabAdh, emprunt *tabEmp, reservation *tabResa, int nbJeux, int nbAdh, int nbEmp, int nbResa) // sauvegarde tout les tableaux  dans un fichier binaire
{
	system(CLEAN_SCREEN);
	saveJeux(head, nbJeux);
	saveAdh(tabAdh, nbAdh);
	saveEmp(tabEmp, nbEmp);
	saveResa(tabResa, nbResa);
}

void 		saveJeuxTxt(nodeType *head, int nbJeux) //sauvegarde la liste de jeux dans son fichier texte
{
	nodeType*	currentNode = head;
	jeu			currentGame;
	FILE *flot;

	flot = fopen("jeux.txt","w");
	
	if (flot == NULL)
	{
		printf("ProblŠme d'ouverture du fichier jeux.txt en ‚criture");
		return;
	}

	fprintf(flot, "Nombre total de jeux : %d\n",nbJeux);
	fprintf(flot, "\nID JEU |            NOM            |      TYPE     | NOMBRE D'EXEMPLAIRE\n");

	while (currentNode != NULL) 
	{
		currentGame = currentNode->game;
		fprintf(flot, "%-6d | %-25s | %-13s | %d\n", currentGame.idJeu, currentGame.nom, currentGame.type, currentGame.nbExp);
		currentNode = currentNode->next;
    }

	fclose(flot);
	printf("Sauvegarde des jeux en texte termin‚e\n");
}

void 		saveAdhTxt(adherent **tabAdh, int nbAdh) //sauvegarde le tableau de pointeur d'adherents dans son fichier texte
{
	FILE *flot;
	int i;

	flot = fopen("adherents.txt","w");

	if (flot == NULL)
	{
		printf("ProblŠme d'ouverture du fichier adherents.txt en ‚criture");
		return;
	}

	fprintf(flot, "Nombre total d'adherents : %d\n", nbAdh);
	fprintf(flot, "\nID ADHERENT | CIVILITE |       NOM       |      PRENOM     |   DATE\n");

	for (i = 0; i < nbAdh; i++)
		fprintf(flot,"%-11d | %-8s | %-15s | %-15s | %s\n", tabAdh[i]->idAdherent, tabAdh[i]->civilite, tabAdh[i]->nom, tabAdh[i]->prenom, tabAdh[i]->dateIns);

	fclose(flot);
	printf("Sauvegarde des adh‚rents en texte termin‚e\n");
}

void 		saveEmpTxt(emprunt *tabEmp, int nbEmp) //sauvegarde le tableau d'emprunt dans son fichier texte
{
	FILE *flot;
	int i;

	flot = fopen("emprunt.txt","w");

	if (flot == NULL)
	{
		printf("ProblŠme d'ouverture du fichier emprunt.txt en ‚criture");
		return;
	}

	fprintf(flot, "Nombre d'emprunt en cours : %d\n", nbEmp);
	fprintf(flot, "\nID EMPRUNT | ID ADHERENT | ID JEU |   DATE\n");

	for ( i = 0; i < nbEmp; i++)
		fprintf(flot, "%-10d | %-11d | %-6d | %s\n", tabEmp[i].idEmpResa, tabEmp[i].idAdherent, tabEmp[i].idJeu, tabEmp[i].dateEmpResa);

	fclose(flot);
	printf("Sauvegarde des emprunts en texte termin‚e\n");
}

void 		saveResaTxt(reservation *tabResa, int nbResa) //sauvegarde le tableau de reservation dans son fichier texte
{
	FILE *flot;
	int i;

	flot = fopen("reservation.txt","w");
	
	if (flot == NULL)
	{
		printf("ProblŠme d'ouverture du fichier emprunt.txt en ‚criture");
		return;
	}

	fprintf(flot, "Nombre d'emprunt en cours : %d\n", nbResa);
	fprintf(flot, "\nID RESERVATION | ID ADHERENT | ID JEU |   DATE\n");

	for ( i = 0; i < nbResa; i++)
		fprintf(flot, "%-14d | %-11d | %-6d | %s\n", tabResa[i].idEmpResa, tabResa[i].idAdherent, tabResa[i].idJeu, tabResa[i].dateEmpResa);

	fclose(flot);
	printf("Sauvegarde des r‚servation en texte termin‚e\n");
}

void 		sauvegarderTxt(nodeType *head, adherent **tabAdh, emprunt *tabEmp, reservation *tabResa, int nbJeux, int nbAdh, int nbEmp, int nbResa) // sauvegarde totu les tableaux en binaire et txt
{
	sauvegarder (head, tabAdh, tabEmp, tabResa, nbJeux, nbAdh, nbEmp, nbResa);
	saveJeuxTxt(head, nbJeux);
	saveAdhTxt(tabAdh, nbAdh);
	saveEmpTxt(tabEmp, nbEmp);
	saveResaTxt(tabResa, nbResa);
}

int			menuSave(void) // menu affichage save (FONCTION Nø 8)
{
	int choix;

	system(CLEAN_SCREEN);
	printf("\n-----------------------------------------------------------------\n");
	printf("| 1.	Sauvegarder en fichier binaire.				|\n");
	printf("| 2.	Sauvegarder en fichier binaire et texte.		|\n");
	printf("| 3.	QUITTER							|\n");
	printf("-----------------------------------------------------------------\n");
    scanf("%d",&choix);

    return choix;
}