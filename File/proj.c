#include"proj.h"

int 		menu(void) // Menu principal
{
	int choix;

	system(CLEAN_SCREEN);
	printf("\n-----------------------------------------------------------------\n");
	printf("| 1.	Afficher les jeux 					|\n");
	printf("| 2.	Afficher la liste des adherents 			|\n");
	printf("| 3.	Afficher la liste des emprunts				|\n");
	printf("| 4.	Afficher les réservations pour un jeu			|\n");
	printf("| 5.	Saisir un nouveau jeu 					|\n");
	printf("| 6.	Saisir un nouvel emprunt				|\n");
	printf("| 7.	Saisir une nouvelle réservation				|\n"); 
	printf("| 8.	Retour d'un jeu						|\n");
	printf("| 9.	Annuler une réservation					|\n");
	printf("| 10.	SAUVEGARDER						|\n");
	printf("| 11.	QUITTER							|\n");                  
	printf("-----------------------------------------------------------------\n");
    scanf("%d",&choix);
	system(CLEAN_SCREEN);

    return choix;
}


void 		dateNow(char date[]) //récupère la date du jour en format local (france)
{
	time_t  now = time (&now);

	strftime(date, 11 * sizeof(char), "%d/%m/%Y", localtime(&now));
}


void 		upper(char tab[]) // met toutes les lettres d'une chaine de caractères en maj
{
	unsigned int i;
	for (i = 0; i < strlen(tab); i++)	
		tab[i] = toupper(tab[i]);
}

void 		upperLower(char tab[]) // met la première lettre en maj et le reste en min pour une chaine de caractères
{
	unsigned int i;
	tab[0] = toupper(tab[0]);
	for (i = 1; i < strlen(tab); i++)
	{
		if (tab[i] == ' ')
		{
			i++;
			tab[i] = toupper(tab[i]);
		}
		else
			tab[i] = tolower(tab[i]);
	}
}

void		formatTxt(char tab[]) // remplace le carac _ par un espace.
{
	unsigned int i;

	for ( i = 0; i < strlen(tab); i++)
		if (tab[i] == '_')

			tab[i] = ' ';
}

void		formatBin(char tab[]) // remplace le carac espace par _
{
	unsigned int i;

	for ( i = 0; i < strlen(tab); i++)
		if (tab[i] == ' ')

			tab[i] = '_';
}


nodeType	*createNode(jeu	game) // crée un maillon de liste chaînée
{
	nodeType *node;
	node = (nodeType*) malloc(sizeof(nodeType));

	if (node == NULL)
	{
		printf("Problème d'allocation pour la liste chaînée\n");
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

void		createLastNode(nodeType **head, jeu game) // utilise les deux fonctions ci-dessus pour créer un maillon à la fin de la liste chaînée
{
	if (*head != NULL)
		lastNode(*head)->next = createNode(game); // si *head existe, on prend le dernier maillon et on assigne a son next le maillon que l'on veut ins?rer	
	else
		*head = createNode(game); // sinon, on crée le maillon ? la place du premier
}

nodeType	*ChargementJeux(nodeType *head, int *nbJeux) // utilise les trois fonctions ci-dessus pour charger le fichier 'jeux.txt' dans la liste chaînée
{
	FILE *flot;
	jeu	currentGame;
	int	i;

	flot = fopen("File/Fichier bin/jeux.bin", "rb");
	//flot = fopen("File/Fichier txt/jeux.txt", "r");

	if (flot == NULL)
	{
		printf("problème d'ouverture du fichier jeux.bin en lecture\n");
		exit(1);
	}

	fread(nbJeux, sizeof(int), 1, flot);
	//fscanf(flot, "%d", nbJeux);

	for (i = 0; i < *nbJeux; i++)
	{
		fread(&currentGame, sizeof(jeu), 1, flot);
		//fscanf(flot, "%d%s%s%d" , &currentGame.idJeu, currentGame.nom, currentGame.type, &currentGame.nbExp);	
		formatTxt(currentGame.nom);
		createLastNode(&head, currentGame);
	}

	fclose(flot);

	return head;
}


int 		ChargementAdherents(adherent *tabAdh[], int max)  //charge le contenu du fichier adherent en mémoire 
{
	FILE *flot;
	int i = 0, nbAdh;

	flot = fopen("File/Fichier bin/adherents.bin", "rb");
	//flot = fopen("File/Fichier txt/adherents.txt", "r");

	if (flot == NULL)
	{
			printf("problème d'ouverture du fichier adherents.bin en lecture\n");
			exit(1);
	}

	fread(&nbAdh,sizeof(int),1,flot);
	//fscanf(flot, "%d", &nbAdh);

	if (nbAdh > max)
	{
		printf("Tableaux d'adhérents trop petit");
		exit(-2);
	}
	while (!feof(flot))
	{
		tabAdh[i] = (adherent *) malloc(sizeof(adherent));
		if (tabAdh[i] == NULL)
		{
			printf("Problème d'allocation en mémoire à la %de place pour les adhérents",i+1);
			exit(-1);
		}
		fread(tabAdh[i],sizeof(adherent),1,flot);
		//fscanf(flot, "%d%s%s%s%s", &tabAdh[i]->idAdherent, tabAdh[i]->civilite, tabAdh[i]->nom, tabAdh[i]->prenom, tabAdh[i]->dateIns);
		formatTxt(tabAdh[i]->nom); 
		i++;
	}
	fclose(flot);

	return nbAdh;
}


empResa		*lectureEmpResa(empResa tabEmpResa[], int nbEmpResa, FILE *flot) // lit le fichier envoyé en paramètre et remplis le tableau
{
	//int i;
	fread(tabEmpResa,sizeof(emprunt),nbEmpResa,flot);
	//for (i = 0; i < nbEmpResa; i++)
	//	fscanf(flot, "%d%d%d%s", &tabEmpResa[i].idEmpResa, &tabEmpResa[i].idAdherent, &tabEmpResa[i].idJeu, tabEmpResa[i].dateEmpResa);

	fclose(flot);

	return tabEmpResa;
}

emprunt		*ChargementEmprunt(int *nbEmp, int *idEmp) // charge le contenu du fichier emprunt en mémoire
{
	emprunt *tabEmp;
	FILE *flot;

	flot = fopen("File/Fichier bin/emprunts.bin", "rb");
	//flot = fopen("File/Fichier txt/emprunts.txt", "r");

	if (flot == NULL)
	{	
		printf("Problème d'ouverture du fichier emprunt.bin en lecture");
		exit(1);
	}

	fread(nbEmp,sizeof(int),1,flot);
	//fscanf(flot ,"%d", nbEmp);


	tabEmp = (emprunt *) malloc(*nbEmp * sizeof (emprunt));

	if (tabEmp == NULL)
	{
			printf("Problème d'allocation en mémoire pour les emprunts");
			exit(-1);
	}

	tabEmp = lectureEmpResa(tabEmp, *nbEmp, flot);
	*idEmp = tabEmp[*nbEmp-1].idEmpResa;
	return tabEmp;
}

reservation *ChargementReservation(int *nbResa, int *idResa) // charge le contenu du fichier reservation en m?moire
{
	reservation *tabResa;
	FILE *flot;
	
	flot = fopen("File/Fichier bin/reservations.bin", "rb");
	//flot = fopen("File/Fichier txt/reservations.txt", "r");

	if (flot == NULL)
	{	
		printf("Problème d'ouverture du fichier reservation.bin en lecture");
		exit(1);
	}

	fread(nbResa,sizeof(int),1,flot);
	//fscanf(flot, "%d", nbResa);

	tabResa = (reservation *) malloc(*nbResa * sizeof (reservation));

	if (tabResa == NULL)
	{
			printf("Problème d'allocation en mémoire pour les reservations");
			exit(-1);
	}

	tabResa = lectureEmpResa(tabResa, *nbResa, flot);
	*idResa = tabResa[*nbResa-1].idEmpResa;
	return tabResa;
}


nodeType 	*copyList(nodeType *head) // renvoie une liste copiée a partir de celle passée en paramètre
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

void        copierL(nodeType *headbis, int nbJ, nodeType **GD, nodeType **S)
{
    nodeType *GDc;
    int nb;
    if (*S == NULL)
    {
        *GD = headbis;
        GDc = *GD;

        for (nb = 0; nb < nbJ-1; nb++)
            GDc = GDc->next;
        *S = GDc->next;

        GDc->next = NULL;
    }
    else
        *GD = *S;
}

nodeType    *fusionL(nodeType *G, nodeType *D, int choix)
{
    nodeType *head;
    head = NULL;

	if (G != NULL && D != NULL)
	{
		if (choix == 0)
		{
			if (strcmp(G->game.nom,D->game.nom) <= 0)
			{
				G :
				head = G;  
				if (choix == 0)
					head->next = fusionL(G->next, D, 0);
				else
					head->next = fusionL(G->next, D, 1);
				G = NULL;
				D = NULL;
			}
			else 
			{
				D :
				head = D;
				if (choix == 0)
					head->next = fusionL(G, D->next, 0);
				else
					head->next = fusionL(G, D->next, 1);
				D = NULL; 
				G = NULL;
			}
		}
		else
		{
			if (strcmp(G->game.type,D->game.type) <= 0)
				goto G;
			else
				goto D;			
		}			   
	}

    if (D == NULL)
    {
        if (G != NULL)
        {
            head = G;
            head->next = fusionL(G->next,NULL, -1);
        }
        else
            return head;
    }
    else
    {
        if (D != NULL)
        {
            head = D;
            head->next = fusionL(NULL,D->next, -1);
        }
    }
	return head;
}

nodeType    *triFusionL(nodeType *headbis, int nbJ, int choix)
{
    nodeType *G, *D, *S;
    S = NULL;

    if (nbJ == 1)
        return headbis;
    
    copierL(headbis, nbJ/2, &G, &S);
    copierL(headbis, 0, &D, &S);
   	G = triFusionL(G, nbJ/2, choix);
   	D = triFusionL(D, nbJ-nbJ/2, choix);
	headbis = fusionL(G, D, choix);
    
    return headbis;
}

void		triSwitchJeu(nodeType *head, int choix)
{
    nodeType  	*current;
    jeu    		temp_jeu;

    current = head;

    while (current != 0 && current->next != 0)
    {
        if (choix == 0 && strcmp(current->game.nom, current->next->game.nom) > 0)
        {
			Switch : 
            temp_jeu = current->game;
            current->game = current->next->game;
            current->next->game = temp_jeu;
            current = head;
        }
        else
		{
			if (choix == 1 && strcmp(current->game.type, current->next->game.type) > 0)
				goto Switch;
			else
				current = current->next;
		}
    }
}

int			comptNbEmp(emprunt tabEmp[], int idJeu, int nbEmp) // compte le nombre d'emprunts en cours d'un jeu donné en paramètre
{
	int i, nbEmpJeu = 0;

	for (i = 0; i < nbEmp; i++)
	{
		if (tabEmp[i].idJeu == idJeu)
			nbEmpJeu++;
	}
	
	return nbEmpJeu;
}

void		freeListe(nodeType *head) // libère la liste chaînée
{
	nodeType *currentNode = head;
	jeu	currentGame;
	
	while (currentNode != NULL) 
	{
		currentGame = currentNode->game;
		free(currentNode);
		currentNode = currentNode->next;
    }
}

void		printJeu(nodeType* head, emprunt tabEmp[], int nbJeux, int nbEmp) // affiche une liste de jeux
{
	nodeType	*headbis, *currentNode;
	jeu	currentGame;
	int k = 0;

	headbis = copyList(head);
	//headbis = triFusionL(headbis, nbJeux, 0); // Version de tri alternative programmé par Samuel
	//headbis = triFusionL(headbis, nbJeux, 1);
	triSwitchJeu(headbis, 0);  // Version dévelopé par Antoine
	triSwitchJeu(headbis, 1);
	currentNode = headbis;

    while (currentNode != NULL) 
	{
		currentGame = currentNode->game;
		currentGame.nbExp -= comptNbEmp(tabEmp, currentGame.idJeu, nbEmp);
		if (currentGame.nbExp > 0)
		{
			k++;
			if (k == 1)
				printf("\nID JEU |            NOM            |     TYPE     | NB EXEMPL\n");

			printf("%-6d | %-25s | %-12s | %d\n", currentGame.idJeu, currentGame.nom, currentGame.type, currentGame.nbExp);
		}
		currentNode = currentNode->next;
    }
	if (k == 0)
		printf("\ntous les jeux sont actuellement empruntés.\n");	

	freeListe(headbis);
}

void		printAllJeu(nodeType* head, emprunt tabEmp[], int nbEmp) // affiche une liste de jeux
{
	nodeType*	currentNode = head;
	jeu			currentGame;

	printf("\nID JEU |            NOM            |     TYPE     | NB EXEMPL\n");

    while (currentNode != NULL) 
	{
		currentGame = currentNode->game;
		currentGame.nbExp -= comptNbEmp(tabEmp, currentGame.idJeu, nbEmp);
		printf("%-6d | %-25s | %-12s | %d", currentGame.idJeu, currentGame.nom, currentGame.type, currentGame.nbExp);
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
	system(CLEAN_SCREEN);

    return choix;
}


void		printAdherent(adherent *tabAdh[],int nbAdh)
{
	int i;

	printf("ID ADHERENT | CIVILITE |       NOM       |     PRENOM     |   DATE\n");

	for (i = 0; i < nbAdh; i++)
		printf("%-11d | %-8s | %-15s | %-14s | %s\n", tabAdh[i]->idAdherent, tabAdh[i]->civilite, tabAdh[i]->nom, tabAdh[i]->prenom, tabAdh[i]->dateIns);
}


int 		dateEmp(emprunt tabEmp[], char date[], int i) // vérifie la date d'emprunt
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
	return 0;
}

void 		printEmprunt(emprunt tabEmp[], char date[], int nbEmp) // affiche tout les emprunt en cours (FONCTION N° 2)
{
	int i;

	printf("ID EMPRUNT | ID ADHERENT | ID JEU | DATE EMPRUNT\n");

	for (i = 0; i < nbEmp; i++)
	{
		printf("%-10d | %-11d | %-6d | %s", tabEmp[i].idEmpResa, tabEmp[i].idAdherent, tabEmp[i].idJeu, tabEmp[i].dateEmpResa);
		if (dateEmp(tabEmp, date, i) == -1)
			printf("\tRetard de l'emprunt");
		printf("\n");
	}
}

void 		printResa(reservation *tabResa, nodeType *head, int nbResa, int nbJeux) // affiche toute les reservation en cours en focntion du jeux donné (FONCTION N° 3)
{
	int i, idJeu, trouve, k = 0;
	char nomJeu[26];

	printf("Quelle jeu voulez-vous rechercher ? ");
	fgets(nomJeu,26,stdin);
	nomJeu[strlen(nomJeu)-1] = '\0';
	upperLower(nomJeu);

	idJeu = rechercheJeu(head, nomJeu, nbJeux, &trouve);

	if (trouve == -1)
	{
		printf("Le jeu recherché n'existe pas, retour au menu ...\n");
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
			printf("\nAucune réservation pour ce jeu, retour au menu ...\n");
	}
}


nodeType	*saisieNouveauJeu(nodeType *head, int *nbJeux) // crée un nouveau jeu
{
	jeu game;
	
	game.idJeu = *nbJeux+1;

	printf("Entrez un Nom : ");	
	fgets(game.nom,26,stdin);
	game.nom[strlen(game.nom)-1] = '\0';
	upperLower(game.nom);

	printf("Un Type : "); 
	fgets(game.type,14,stdin);
	game.type[strlen(game.type)-1] = '\0';
	upperLower(game.type);

	printf("Un nb d'exemplaire : ");
	scanf("%d",&game.nbExp);
	

	createLastNode(&head, game);
	*nbJeux+= 1;
	printf("\nJeu bien ajouté.\n");
	
	return head;
}


int			rechercheAdherent(adherent *tabAdh[], char nom[], char prenom[], int nbAdh) // permet de savoir si l'adhérent existe ou non
{
	int i;
	for (i = 0; i < nbAdh; i++)
	{
		if (strcmp(nom,tabAdh[i]->nom) == 0 && strcmp(prenom,tabAdh[i]->prenom) == 0)
			return i;
	}
	return -1;
}

void 		nouvelAdherent(adherent *tabAdh[], char nom[], char prenom[], char date[], int *nbAdh, int max) // crée un nouvel adherent
{
	char civ[4];

	if (*nbAdh == max)
	{
		printf("\nTableau d'adhérents pleins, impossible d'en ajouter un nouveau\n");
		exit(-2);
	}

	tabAdh[*nbAdh] = (adherent *) malloc (sizeof (adherent));

	if (tabAdh[*nbAdh] == NULL)
	{
		printf("\nProblème d'allocation mémoire pour un nouvel adherent\n");
		exit(-1);
	}

	tabAdh[*nbAdh]->idAdherent = *nbAdh+1;

	printf("\nEntrez une Civilité : ");
	scanf("%s%*c", civ);
	upperLower(civ);

	while (strcmp(civ, "Mr") != 0 && strcmp (civ, "Mme") != 0)
	{
		printf("\nCette civilité n'existe pas, veuillez choisir entre Mr ou Mme : ");
		scanf("%s%*c",civ);
		upperLower(civ);
	}

	strcpy(tabAdh[*nbAdh]->civilite, civ);
	strcpy(tabAdh[*nbAdh]->nom, nom);
	strcpy(tabAdh[*nbAdh]->prenom, prenom);
	strcpy(tabAdh[*nbAdh]->dateIns, date);

	printf("\nNouvel adhérent ajouté\n");
	*nbAdh+= 1;
}

int 		dateIns(adherent *tabAdh[], char date[], int i) // vérifie la validité de l'adherent
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
		
	return 0;	
}

int			comptNbEmpAdh(emprunt tabEmp[], int idAdh, int nbEmp) // compte le nombre d'emprunts en cours d'un jeu donné en paramètre
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
	return 0;
}

int			rechercheJeu(nodeType* head, char name[], int nbJeux, int *trouve) // permet de savoir si un jeu existe ou non
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

int			rechercheJeuxAct(empResa tabEmpResa[], int idJeu, int idAdh, int nbEmp) // recherche si un adherent à déjà un emprunt ou réservation du jeu
{
	int i;

	for (i = 0; i < nbEmp; i++)
		if (tabEmpResa[i].idJeu == idJeu && tabEmpResa[i].idAdherent == idAdh)
			return 1;

	return 0;
}

empResa		*ajoutEmpResa(empResa tabEmpResa[], adherent *tabAdh[], int *nbEmpResa, int *nbEmpResaMax, int *idEmpResa, int idAdh, int idJeu, char date[]) // ajoute un/e emprunt/reservation en fonction de ce qui lui est passé en paramètre
{
	empResa *newtab;

	if (*nbEmpResa == *nbEmpResaMax)
	{
		*nbEmpResaMax+= 5;
		newtab = (empResa *) realloc(tabEmpResa, *nbEmpResaMax * sizeof (empResa));
		if (newtab == NULL)
		{
			printf("\nProblème de réallocation\n");
			exit(-3);
		}
		tabEmpResa = newtab;
	}

	tabEmpResa[*nbEmpResa].idEmpResa = *idEmpResa+1;
	tabEmpResa[*nbEmpResa].idAdherent = tabAdh[idAdh]->idAdherent;
	tabEmpResa[*nbEmpResa].idJeu = idJeu;
	strcpy(tabEmpResa[*nbEmpResa].dateEmpResa, date);

	*idEmpResa+= 1;
	*nbEmpResa+= 1;
	printf("\nnouvel ajout effectué\n");

	return tabEmpResa;
}

emprunt 	*saisieNouvelEmprunt(adherent *tabAdh[], emprunt tabEmp[], nodeType* head, char date[], int *nbAdh, int *nbEmp, int *idEmp, int *nbEmpmax, int nbJeux, int max) // saisie les informations pour un nouvel emprunt (FONCTION N° 2)
{
	char nom[16], prenom[16], jeu[26], choix[4];
	int idAdh, idJeu, trouve;

	printf("Entrez le NOM de l'emprunteur : ");
	fgets(nom,16,stdin);
	nom[strlen(nom)-1] = '\0';
	upper(nom);

	printf("Le Prénom : ");
	fgets(prenom,16,stdin);
	prenom[strlen(prenom)-1] = '\0';
	upperLower(prenom);

	idAdh = rechercheAdherent(tabAdh, nom, prenom, *nbAdh);
		
	if (idAdh == -1)
	{
		printf("%s %s n'a jamais été enregistrée, voulez vous l'ajouter ? (oui/non) : ", nom, prenom);
		scanf("%s%*c", choix);
		if (strcmp(choix, "oui") == 0)
		{
			printf("Cette personne a-t-elle payée sa cotisation ? (oui/non) : ");
			scanf("%s%*c", choix);
			if (strcmp(choix, "oui") == 0)
				nouvelAdherent(tabAdh, nom, prenom, date, nbAdh, max);
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
			printf("\nL'adhérent %s %s est arrivé à expiration, a-t-il payée sa cotisation ? (oui/non) : ",  tabAdh[idAdh]->nom, tabAdh[idAdh]->prenom);
			scanf("%s%*c", choix);
			if (strcmp(choix, "oui") == 0)	
				strcpy(tabAdh[idAdh]->dateIns, date); 
			else
				return tabEmp;
		}	
		if (comptNbEmpAdh(tabEmp, idAdh+1, *nbEmp) == 3)
		{
			printf("\nL'adhérent %s %s a déjà 3 emprunts, retour au menu ...\n", tabAdh[idAdh]->nom, tabAdh[idAdh]->prenom);
			return tabEmp;
		}
		else if (RetardEmp(tabEmp, date, *nbEmp, idAdh+1) == -1)
			
		{
			printf("\nL'adhérent %s %s a un emprunt de plus de 1 mois, retour au menu ...\n",tabAdh[idAdh]->nom, tabAdh[idAdh]->prenom);
			return tabEmp;
		}
		
	}

	Jeu :
	printf("\nEntrez le nom du jeu à emprunter : ");
	fgets(jeu,26,stdin);
	jeu[strlen(jeu)-1] = '\0';
	upperLower(jeu);

	idJeu = rechercheJeu(head, jeu, nbJeux, &trouve);

	if (trouve == -1)
	{
		printf("Ce jeu n'existe pas, voulez vous en choisir un autre ? (oui/non) : ");
		scanf("%s%*c", choix);
			if (strcmp(choix, "oui") == 0)	
				goto Jeu;
			else
				return tabEmp;
	}
	else
	{	
		if (rechercheJeuxAct(tabEmp, idJeu, idAdh+1, *nbEmp) == 1)
		{
			printf("Cet adhérent à déjà emprunté ce jeu, voulez vous en choisir un autre ? (oui/non) : ");
			scanf("%s%*c", choix);
			if (strcmp(choix, "oui") == 0)	
				goto Jeu;
			else
				return tabEmp;
		}
		else
		{
			if (trouve-comptNbEmp(tabEmp, idJeu, *nbEmp) == 0)
			{
				printf("Ce jeu n'est plus disponible, veuillez en faire une réservation, voulez vous en choisir un autre ? (oui/non) : ");
				scanf("%s%*c", choix);
				if (strcmp(choix, "oui") == 0)	
					goto Jeu;
				else
					return tabEmp;
			}
			else
				return ajoutEmpResa(tabEmp, tabAdh, nbEmp, nbEmpmax, idEmp, idAdh, idJeu, date);
		}
	}
}

reservation *saisieNouvelResa(adherent *tabAdh[], reservation tabResa[], emprunt tabEmp[], nodeType* head, char date[], int *nbAdh, int *nbResa,  int *idResa, int *nbResaMax, int nbEmp, int nbJeux, int max) // saisie les informations pour une nouvelle reservation (FONCTION N° 3)
{
	char nom[16], prenom[16], jeu[26], choix[4];
	int idAdh, idJeu, trouve;

	printf("Entrez le nom de l'adhérent souhaitant réserver : ");
	fgets(nom,16,stdin);
	nom[strlen(nom)-1] = '\0';
	upper(nom);

	printf("Le Prénom : ");
	fgets(prenom,16,stdin);
	prenom[strlen(prenom)-1] = '\0';
	upperLower(prenom);

	idAdh = rechercheAdherent(tabAdh, nom, prenom, *nbAdh);
		
	if (idAdh == -1)
	{
		printf("%s %s n'a jamais été enregistrée, voulez vous l'ajouter ? (oui/non) : ", nom, prenom);
		scanf("%s%*c", choix);
		if (strcmp(choix, "oui") == 0)
		{	
			printf("Cette personne a-t-elle payée sa cotisation ? (oui/non) : ");
			scanf("%s%*c", choix);
			if (strcmp(choix, "oui") == 0)
				nouvelAdherent(tabAdh, nom, prenom, date, nbAdh, max);
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
			printf("L'adhérent %s %s est arrivé à expiration, a-t-il payée sa cotisation ? (oui/non) : ",  tabAdh[idAdh]->nom, tabAdh[idAdh]->prenom);
			scanf("%s%*c", choix);
			if (strcmp(choix, "oui") == 0)
			{	
				strcpy(tabAdh[idAdh]->dateIns, date);
			}
			else
				return tabEmp;
		}
		else if (RetardEmp(tabEmp, date, nbEmp, idAdh+1) == -1)		
		{
			printf("\nL'adhérent %s %s a un emprunt de plus de 1 mois, retour au menu ...\n",tabAdh[idAdh]->nom, tabAdh[idAdh]->prenom);
			return tabResa;
		}
	}
	
	Jeu :
	printf("Entrez le Nom du jeu à réserver : ");
	fgets(jeu,26,stdin);
	jeu[strlen(jeu)-1] = '\0';
	upperLower(jeu);

	idJeu = rechercheJeu(head, jeu, nbJeux, &trouve);

	if (trouve == -1)
	{
		printf("Ce jeu n'existe pas, voulez vous en choisir un autre ? (oui/non) : ");
		scanf("%s%*c", choix);
			if (strcmp(choix, "oui") == 0)	
				goto Jeu;
			else
				return tabEmp;
	}
	else
	{	
		if (rechercheJeuxAct(tabResa, idJeu, idAdh+1, *nbResa) == 1)
		{
			printf("Cet adhérent à déjà réservé ce jeu, voulez vous en choisir un autre ? (oui/non) : ");
			scanf("%s%*c", choix);
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
				return ajoutEmpResa(tabResa, tabAdh, nbResa, nbResaMax, idResa, idAdh, idJeu, date);
		}
	}
}


int			printEmpResa(empResa tabEmpResa[], int nbEmpResa, int id) // affiche les emprunt ou réservations d'un adherent.
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

empResa 	*decalerGauche(empResa tabEmpResa[], int *nbEmpResa, int val) // décaler un tableau à gauche
{
	while (val < *nbEmpResa-1)
	{
		tabEmpResa[val] = tabEmpResa[val+1];
		val++; 
	}

	*nbEmpResa-=1;
	
	return tabEmpResa;
}

int			rechercheResa(reservation tabResa[], int nbResa, int idjeu) // recherche si une réservation corespond au nom du jeu
{
	int i;
	for (i = 0; i < nbResa ; i++)
		if (tabResa[i].idJeu == idjeu)
			return i;
	return -1;
}

emprunt		*RetourJeu(adherent *tabAdh[], emprunt tabEmp[], emprunt tabResa[], int *nbEmp, int *idEmp, int *nbResa, int nbAdh, char date[]) // retourne un jeu et regarde si une réservation peu passer en emprunt (FONCTION N°6)
{
	int idjeu, idAdh, idResa, val;
	char nom[16], prenom[16];

	printf("Entrez le NOM de l'emprunteur : ");
	fgets(nom,16,stdin);
	nom[strlen(nom)-1] = '\0';
	upper(nom);

	printf("Entrez le Prénom : ");
	fgets(prenom,16,stdin);
	prenom[strlen(prenom)-1] = '\0';
	upperLower(prenom);

	idAdh = rechercheAdherent(tabAdh, nom, prenom, nbAdh);

	if (idAdh == -1)
	{
		printf("%s %s n'a jamais été enregistrée, retour au menu ...\n", nom, prenom);
		return tabEmp;
	}
	else 
	{
		if (printEmpResa(tabEmp, *nbEmp, idAdh+1) == 0)
		{
			printf("Aucun emprunt en cours pour cet adhérent\n");
			return tabEmp;
		}
		else
		{
			printf("Choissisez un numéro d'emprunt pour cet adhérent : ");
			scanf("%d", &val);

			while (tabEmp[val-1].idAdherent != idAdh+1)
			{
				printf("Votre numéro d'emprunt pour cet adhérent n'est pas correct veuillez resaisir : ");
				scanf("%d", &val);
			}

			idjeu = tabEmp[val-1].idJeu;
			tabEmp = decalerGauche(tabEmp, nbEmp, val-1);
			printf("\nRetour bien effectué\n");
			
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
					printf("\nLa réservation numéro %d au nom de %s %s est passée en emprunt\n", tabResa[idResa].idEmpResa, tabAdh[(tabResa[idResa].idAdherent)-1]->nom, tabAdh[(tabResa[idResa].idAdherent)-1]->prenom);
					tabResa = decalerGauche(tabResa, nbResa, idResa);
				}
			}
		}		
	}
	return tabEmp;	
}

reservation	*AnnulationResa(adherent *tabAdh[], reservation tabResa[], int *nbResa, int nbAdh) // Annule une réservation (FONCTION N°7)
{
	int idAdh, val;
	char nom[16], prenom[16];
	
	printf("Entrez le NOM de la personne qui souhaite annuler une réservation : ");
	fgets(nom,16,stdin);
	nom[strlen(nom)-1] = '\0';
	upper(nom);

	printf("Le Prénom : ");
	fgets(prenom,16,stdin);
	prenom[strlen(prenom)-1] = '\0';
	upperLower(prenom);

	idAdh = rechercheAdherent(tabAdh, nom, prenom, nbAdh);

	if (idAdh == -1)
	{
		printf("%s %s n'a jamais été enregistrée, retour au menu ...\n", nom, prenom);
		return tabResa;
	}
	else
	{
		if (printEmpResa(tabResa, *nbResa, idAdh+1) == 0)
			{
				printf("Aucun emprunt en cours pour cet adhérent, ...\n");
				return tabResa;
			}
		else
		{		
			printf("Choissisez un numéro de réservation pour cet adhérent : ");
			scanf("%d", &val);

			while (tabResa[val-1].idAdherent != idAdh+1)
			{
				printf("Votre numéro de réservation n'est pas correct pour cet adhérent veuillez resaisir : ");
				scanf("%d", &val);
			}

			tabResa = decalerGauche(tabResa, nbResa, val-1);
			printf("Annulation bien effectuée\n");

			return tabResa;
		}	
	}
}	


void 		saveJeux(nodeType *head, int nbJeux) //sauvegarde la liste de jeux dans son File/fichier binaire
{
	nodeType* currentNode = head;
	jeu	currentGame;
	FILE *flot;

	flot = fopen ("File/Fichier bin/jeux.bin","wb");
	
	if (flot == NULL)
	{
		printf("Problème d'ouverture du fichier jeux.bin en écriture");
		return;
	}

	fwrite(&nbJeux, sizeof(int), 1, flot);

	while (currentNode != NULL) 
	{
		currentGame = currentNode->game;
		formatBin(currentGame.nom);
		fwrite(&currentGame, sizeof(jeu), 1, flot);
		currentNode = currentNode->next;
    }

	fclose(flot);
}

void 		saveAdh(adherent **tabAdh, int nbAdh) // sauvegarde le tableau de pointeur d'adherents dans son File/fichier binaire
{
	FILE *flot;
	int i;

	flot = fopen ("File/Fichier bin/adherents.bin","wb");

	if (flot == NULL)
	{
		printf("Problème d'ouverture du fichier adherents.bin en écriture");
		return;
	}
	
	fwrite(&nbAdh, sizeof(int), 1, flot);

	for (i = 0; i < nbAdh; i++)
	{	
		formatBin(tabAdh[i]->nom);
		fwrite(tabAdh[i], sizeof(adherent), 1, flot);
	}

	fclose(flot);
}

void 		saveEmp(emprunt *tabEmp, int nbEmp) // sauvegarde le tableau d'emprunt dans son File/fichier binaire
{
	FILE *flot;

	flot = fopen("File/Fichier bin/emprunts.bin","wb");

	if (flot == NULL)
	{
		printf("Problème d'ouverture du fichier emprunt.bin en écriture");
		return;
	}

	fwrite(&nbEmp, sizeof(int), 1, flot);
	fwrite(tabEmp, sizeof(emprunt), nbEmp, flot);

	fclose(flot);
}

void 		saveResa(reservation *tabResa, int nbResa) // sauvegarde le tableau de reservation dans son File/fichier binaire
{
	FILE *flot;

	flot = fopen("File/Fichier bin/reservations.bin","wb");

	if (flot == NULL)
	{
		printf("Problème d'ouverture du fichier reservation.bin en écriture");
		return;
	}

	fwrite(&nbResa, sizeof(int),1,flot);
	fwrite(tabResa, sizeof(reservation),nbResa,flot);

	fclose(flot);
}

void 		sauvegarder(nodeType *head, adherent **tabAdh, emprunt *tabEmp, reservation *tabResa, int nbJeux, int nbAdh, int nbEmp, int nbResa) // sauvegarde tout les tableaux  dans un File/fichier binaire
{
	saveJeux(head, nbJeux);
	saveAdh(tabAdh, nbAdh);
	saveEmp(tabEmp, nbEmp);
	saveResa(tabResa, nbResa);
	
	printf("Sauvegarde des jeux terminée\n");
	printf("Sauvegarde des adhérents terminée\n");
	printf("Sauvegarde des emprunts terminée\n");
	printf("Sauvegarde des réservations terminée\n");
}

void 		saveJeuxTxt(nodeType *head, int nbJeux, char src[]) //sauvegarde la liste de jeux dans son fichier texte
{
	nodeType*	currentNode = head;
	jeu			currentGame;
	FILE *flot;
	char dest[100];

	strcpy(dest,src);
	strcat(dest,"/jeux.txt");

	flot = fopen(dest,"w");
	
	if (flot == NULL)
	{
		printf("Problème d'ouverture du fichier jeux.txt en écriture");
		return;
	}

	fprintf(flot, "Nombre total de jeux : %d\n",nbJeux);
	fprintf(flot, "\nID JEU |            NOM            |     TYPE     | NB EXEMPL\n");

	while (currentNode != NULL) 
	{
		currentGame = currentNode->game;
		fprintf(flot, "%-6d | %-25s | %-12s | %d\n", currentGame.idJeu, currentGame.nom, currentGame.type, currentGame.nbExp);
		currentNode = currentNode->next;
    }

	fclose(flot);
}

void 		saveAdhTxt(adherent **tabAdh, int nbAdh, char src[]) //sauvegarde le tableau de pointeur d'adherents dans son fichier texte
{
	FILE *flot;
	char dest[100];
	int i;

	strcpy(dest,src);
	strcat(dest,"/adherents.txt");

	flot = fopen(dest,"w");

	if (flot == NULL)
	{
		printf("Problème d'ouverture du fichier adherents.txt en écriture");
		return;
	}

	fprintf(flot, "Nombre total d'adherents : %d\n", nbAdh);
	fprintf(flot, "\nID ADHERENT | CIVILITE |       NOM       |     PRENOM     |   DATE\n");

	for (i = 0; i < nbAdh; i++)
		fprintf(flot,"%-11d | %-8s | %-15s | %-14s | %s\n", tabAdh[i]->idAdherent, tabAdh[i]->civilite, tabAdh[i]->nom, tabAdh[i]->prenom, tabAdh[i]->dateIns);

	fclose(flot);	
}

void 		saveEmpTxt(emprunt *tabEmp, int nbEmp, char src[]) //sauvegarde le tableau d'emprunt dans son fichier texte
{
	FILE *flot;
	char dest[100];
	int i;

	strcpy(dest,src);
	strcat(dest,"/emprunts.txt");

	flot = fopen(dest,"w");

	if (flot == NULL)
	{
		printf("Problème d'ouverture du fichier emprunts.txt en écriture");
		return;
	}

	fprintf(flot, "Nombre d'emprunt en cours : %d\n", nbEmp);
	fprintf(flot, "\nID EMPRUNT | ID ADHERENT | ID JEU |   DATE\n");

	for ( i = 0; i < nbEmp; i++)
		fprintf(flot, "%-10d | %-11d | %-6d | %s\n", tabEmp[i].idEmpResa, tabEmp[i].idAdherent, tabEmp[i].idJeu, tabEmp[i].dateEmpResa);

	fclose(flot);
}

void 		saveResaTxt(reservation *tabResa, int nbResa, char src[]) //sauvegarde le tableau de reservation dans son fichier texte
{
	FILE *flot;
	char dest[100];
	int i;

	strcpy(dest,src);
	strcat(dest,"/reservations.txt");

	flot = fopen(dest,"w");
	
	if (flot == NULL)
	{
		printf("Problème d'ouverture du fichier reservation.txt en écriture");
		return;
	}

	fprintf(flot, "Nombre d'emprunt en cours : %d\n", nbResa);
	fprintf(flot, "\nID RESERVATION | ID ADHERENT | ID JEU |   DATE\n");

	for ( i = 0; i < nbResa; i++)
		fprintf(flot, "%-14d | %-11d | %-6d | %s\n", tabResa[i].idEmpResa, tabResa[i].idAdherent, tabResa[i].idJeu, tabResa[i].dateEmpResa);

	fclose(flot);
}

void 		sauvegarderTxt(nodeType *head, adherent **tabAdh, emprunt *tabEmp, reservation *tabResa, int nbJeux, int nbAdh, int nbEmp, int nbResa) // sauvegarde totu les tableaux en binaire et txt
{
	char dest[100] = "File/Fichier txt";
	char choix[4];

	printf("Voulez-vous changez de répertoire pour les sauvegardes (File/Fichier txt) ? (oui/non) : ");
	scanf("%s%*c", choix);

	if (strcmp(choix, "oui") == 0)
	{
		printf("Entrez le répertoire : ");
		fgets(dest,100,stdin);
		dest[strlen(dest)-1] = '\0';
	}

	saveJeuxTxt(head, nbJeux, dest);
	saveAdhTxt(tabAdh, nbAdh, dest);
	saveEmpTxt(tabEmp, nbEmp, dest);
	saveResaTxt(tabResa, nbResa, dest);
	sauvegarder (head, tabAdh, tabEmp, tabResa, nbJeux, nbAdh, nbEmp, nbResa);
	printf("\nSauvegarde des jeux en texte terminée\n");
	printf("Sauvegarde des adhérents en texte terminée\n");
	printf("Sauvegarde des emprunts en texte terminée\n");
	printf("Sauvegarde des réservation en texte terminée\n");
}

int			menuSave(void) // menu affichage save (FONCTION N° 8)
{
	int choix;

	system(CLEAN_SCREEN);
	printf("\n-----------------------------------------------------------------\n");
	printf("| 1.	Sauvegarder en File/fichier binaire.			|\n");
	printf("| 2.	Sauvegarder en File/fichier binaire et texte.		|\n");
	printf("| 3.	QUITTER							|\n");
	printf("-----------------------------------------------------------------\n");
    scanf("%d",&choix);
	system(CLEAN_SCREEN);

    return choix;
}