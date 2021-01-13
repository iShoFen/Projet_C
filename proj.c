#include"proj.h"

int 		menu(void)
{
	int choix;

    printf("\n");
	printf("----------------------------------------------------------------\n");
	printf("| 1.	Afficher les jeux disponibles				|\n");
	printf("| 2.	Afficher la liste des emprunts				|\n");
	printf("| 3.	Afficher les reservations pour un jeu			|\n");
	printf("| 4.	Saisir un nouvel emprunt				|\n");
	printf("| 5.	Saisir un nouvelle reservation				|\n"); 
	printf("| 6.	Retour d'un jeu						|\n");
	printf("| 7.	Annuler une reservation					|\n");
	printf("| 8.	SAUVEGARDER						|\n");
	printf("| 9.	QUITTER							|\n");                  
	printf("-----------------------------------------------------------------\n");
    scanf("%d",&choix);
	
    return choix;
}


date 		dateNow(date date) //récupère la date du jour en format local (france)
{
	char j[3], m[3], a[5];

	time_t  now = time (NULL);
	strftime(j, sizeof(j), "%d", localtime(&now));
 	strftime(m, sizeof(m), "%m", localtime(&now));
	strftime(a, sizeof(a), "%Y", localtime(&now));
	strcpy(date.jours,j);
	strcpy(date.mois,m);
	strcpy(date.annee,a);
	return date;
}


void 		upper(char tab[]) // met toutes les lettres d'une chaine de charactère en maj
{
	int 	i;
	for (i = 0; i<strlen(tab);i++)	
		tab[i] = toupper(tab[i]);
}

void 		upperLower(char tab[]) // met la première lettre d'une chaine de charactère en maj et toutes les autres en min
{
	int 	i;
	tab	[0] = toupper(tab[0]);
	for (i = 1; i<strlen(tab);i++)
		tab[i] = tolower(tab[i]);
}


nodeType	*createNode(jeu	game) // crée un maillon de liste a partir du jeu passé en paramètre, next = NULL pour éviter les erreurs de segmentation
{
	nodeType *node;
	node = (nodeType*)malloc(sizeof(nodeType));

	if (node == NULL)
	{
		printf("Probleme d'allocation pour la liste chainee\n");
		exit(-1);
	}

	node->game = game;
	node->next = NULL;

	return node;
}

nodeType	*lastNode(nodeType *head) // renvoie le dernier maillon d'une liste
{
	nodeType	*current;

	current = head;

	if (current != NULL)
	{
		while (current->next != NULL)
			current = current->next;
		return current;
	}

	return NULL;
}

void		createLastNode(nodeType **head, jeu game) // utilise les deux fonctions ci-dessus pour créer un maillon à la fin de la liste chainée
{
	if (*head != NULL)
		lastNode(*head)->next = createNode(game); // si *head existe, on prend le dernier maillon et on assigne a son next le maillon que l'on veut insérer
		
	else
		*head = createNode(game); // sinon, on crée le maillon à la place du premier
}

nodeType	*ChargementJeux(nodeType *head, int *nbJeux) // utilise les trois fonctions ci-dessus pour charger le fichier 'jeux.txt' dans la liste chainée
{
	FILE 		*flot;
	jeu			currentGame;
	int			i;

	flot = fopen("jeux.bin","rb");

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

void		freeListe(nodeType *head) // libère la liste chainé
{
	nodeType*	currentNode = head;
	jeu			currentGame;
	int			nbEmpCurrent;
	
	while (currentNode != NULL) 
	{
		currentGame = currentNode->game;
		free(currentNode);
		currentNode = currentNode->next;
    }
}

int			comptNbEmp(emprunt tabEmp[], int idJeu, int nbEmp) // compte le nombre d'emprunts en cours d'un jeu donné en paramètre
{
	int i, nbEmpJeu = 0;

	for (i=0; i < nbEmp; i++)
	{
		if (tabEmp[i].idJeu == idJeu)
			nbEmpJeu++;
	}
	
	return nbEmpJeu;
}

void		printJeu(nodeType* head, emprunt tabEmp[], int nbEmp) // affiche une liste de jeux
{
	nodeType*	currentNode = head;
	jeu			currentGame;
	int			nbEmpCurrent;

	printf("\nID JEU |            NOM            |      TYPE      | NB EXEMPL\n");

    while (currentNode != NULL) 
	{
		currentGame = currentNode->game;
		nbEmpCurrent = comptNbEmp(tabEmp, currentGame.idJeu, nbEmp);
		currentGame.nbExp -= nbEmpCurrent;
		if (currentGame.nbExp > 0)
			printf("%-6d | %-25.25s | %-14.14s | %d\n", currentGame.idJeu, currentGame.nom, currentGame.type, currentGame.nbExp);
		currentNode = currentNode->next;
    }

	printf("\n"); // ordre alphabetique et par type
}

void    	triAlphaJeu(nodeType *head, emprunt tabEmp[], int nbEmp) // affiche les jeux disponibles dans l'ordre alphabétique (FONCTION N°1.1 du menu)
{
    nodeType  	*current;
	nodeType	*headbis;
    jeu    		temp_jeu;

	headbis = copyList(head);
 
    if (headbis == 0)
		exit(-4);
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

	printJeu(headbis, tabEmp, nbEmp);

	freeListe(headbis);
}

void    	triTypeJeu(nodeType *head, emprunt tabEmp[], int nbEmp) // affiche les jeux disponibles triés par type (FONCTION N°1.2 du menu)
{
    nodeType  	*current;
	nodeType	*headbis;
    jeu    		temp_jeu;

	headbis = copyList(head);
 
    if (headbis == 0)
        exit(-4);
		
    current = headbis;
	
    while (current != 0 && current->next != 0)
    {
        if (strcmp(current->game.type, current->next->game.type) > 0)
        {
            temp_jeu = current->game;
            current->game = current->next->game;
            current->next->game = temp_jeu;
            current = headbis;
        }

        else
            current = current->next;
    }

	printJeu(headbis, tabEmp, nbEmp);

	freeListe(headbis);
}

nodeType 	*copyList(nodeType *head) // renvoie une liste copiée a partir de celle passée en paramètre
{ 
	nodeType *newNode;

    if (head == NULL) 
        return NULL; 

    else 
	{
		newNode = (nodeType*)malloc(sizeof(nodeType)); 
        newNode->game = head->game;
        newNode->next = copyList(head->next);
        return newNode; 
    } 
} 

int			menuJeu(void) // menu affichage jeu (FONCTION N° 1)
{
	int choix;

    printf("\n");
	printf("-----------------------------------------------------------------\n");
	printf("| 1.	Afficher les jeux tries par nom				|\n");
	printf("| 2.	Afficher les jeux tries par type			|\n");
	printf("| 3.	QUITTER							|\n");
	printf("-----------------------------------------------------------------\n");
    scanf("%d",&choix);

    return choix;
}


int 		ChargementAdherents(adherent *tabAdh[], int max)  //charge le contenu du fichier adherent en mémoire 
{
	FILE *flot;
	int i = 0, val;
	
	flot = fopen("adherents.bin","rb");

	if (flot == NULL)
	{
			printf("pb d'ouverture du fichier adherents\n");
			exit(1);
	}

	fread(&val,sizeof(int),1,flot);

	if (val > max)
	{
		printf("Tableaux d'adherent trop petit");
		exit(-2);
	}
	
	tabAdh[i] = (adherent *) malloc (sizeof(adherent));
	if (tabAdh[i] == NULL)
		{
			printf("Probleme d'allocation en mémoire à la %de place pour les adherents",i);
			exit(-1);
		}
	fread(tabAdh[i],sizeof(adherent),1,flot);

	while (!feof(flot))
	{
		i++;
		tabAdh[i] = (adherent *) malloc (sizeof(adherent));
		if (tabAdh[i] == NULL)
		{
			printf("Probleme d'allocation en mémoire à la %de place pour les adherents",i);
			exit(-1);
		}
		
		fread(tabAdh[i],sizeof(adherent),1,flot);
	}
	
	fclose(flot);

	return i;
}


empResa		*lectureEmpResa (empResa tabEmpResa[], int *nbEmpResa, FILE *flot)
{
	fread(tabEmpResa,sizeof(emprunt),*nbEmpResa,flot);
	fclose (flot);

	return tabEmpResa;
}

emprunt		*ChargementEmprunt(int *nbEmp) // charge le contenu du fichier emprunt en mémoire
{
	emprunt *tab;
	int i;
	FILE *flot;

	flot = fopen("emprunt.bin","rb");
	if (flot == NULL)
	{	
		printf("Probleme d'ouverture du fichier emprunt.bin en lecture");
		exit(1);
	}
	
	fread(nbEmp,sizeof(int),1,flot);
	tab = (emprunt *) malloc (*nbEmp * sizeof (emprunt));

	if (tab == NULL)
	{
			printf ("Probleme d'allocation en mémoire pour les emprunts");
			exit(-1);
	}

	return lectureEmpResa (tab, nbEmp, flot);
}

reservation *ChargementReservation(int *nbResa) // charge le contenu du fichier reservation en mémoire
{
	reservation *tab;
	int i;
	FILE *flot;
	
	flot = fopen("reservation.bin","rb");

	if (flot == NULL)
	{	
		printf("Probleme d'ouverture du fichier reservation.bin en lecture");
		exit(1);
	}

	fread(nbResa,sizeof(int),1,flot);
	tab = (reservation *) malloc (*nbResa * sizeof (reservation));

	if (tab == NULL)
	{
			printf ("Probleme d'allocation en mémoire pour les reservations");
			exit(-1);
	}

	return lectureEmpResa (tab, nbResa, flot);
}


void 		printEmprunt(emprunt *tab, date date, int max) // affiche tout les emprunt en cours (FONCTION N° 2)
{
	int i;
	int j, jn, m, mn, a, an;

	jn = atoi(date.jours);
	mn = atoi(date.mois);
	an = atoi(date.annee);

	printf ("\nID EMPRUNT | ID ADHERENT | ID JEU | DATE EMPRUNT\n");

	for (i = 0; i<max; i++)
	{
		printf ("%-10d | %-11d | %-6d | %s/%s/%s", tab[i].idEmpResa, tab[i].idAdherent, tab[i].idJeu, tab[i].dateEmpResa.jours, tab[i].dateEmpResa.mois, tab[i].dateEmpResa.annee);

		j = atoi(tab[i].dateEmpResa.jours);
		m = atoi(tab[i].dateEmpResa.mois);
		a = atoi(tab[i].dateEmpResa.annee);

		if (a < an)
		{
			if (mn > 1 || m < 12)
				printf("\tRetard du retour de l'emprunt");
			else
				if (j < jn)
					printf("\tRetard du retour de l'emprunt");
		}
		else
			if (m < mn)
			{	
				if (j < jn)
					printf("\tRetard du retour de l'emprunt");
				else
					if (m-mn <-1)
						printf("\tRetard du retour de l'emprunt");
			}
		printf("\n");
	}

	printf("\n");
}

void 		printResa(reservation *tabResa, nodeType *head, int nbResa, int nbJeux) // affiche toute les reservation en cours en focntion du jeux donné (FONCTION N° 3)  /!\ mathys, modifie cette fonction pour l'affichage des resa
{
	int i, idJeu, trouve = -1, k = 0;
	char nomJeu[25];

	printf("Quelle(s) reservation(s) voulez-vous afficher ? ");
	scanf("%s%*c",nomJeu);
	upperLower(nomJeu);

	idJeu = rechercheJeu(head, nomJeu, &trouve, nbJeux);

	if (trouve == -1)
	{
		printf("Le jeu recherche n'existe pas\n");
		return;
	}

	else
	{
		for (i = 0; i < nbResa; i++)
			if (tabResa[i].idJeu == idJeu)
			{
				k++;
				if (k == 1)
					printf ("\nID RESERVATION | ID ADHERENT | ID JEU | DATE EMPRUNT\n");
				printf ("%-14d | %-11d | %-6d | %s/%s/%s \n", tabResa[i].idEmpResa, tabResa[i].idAdherent, tabResa[i].idJeu, tabResa[i].dateEmpResa.jours, tabResa[i].dateEmpResa.mois, tabResa[i].dateEmpResa.annee);
			}

		printf("\n");

		if (k == 0)
			printf("Aucune reservation pour ce jeu, retour au menu...\n");
	}
}


void 		nouvelAdherent(adherent *tabAdh[], date date, int *nbAdh, int max) // crée un nouvel adherent
{
	char civ[4], nom[15], prenom[15];

	if (*nbAdh == max)
	{
		printf("Tableau d'adherent pleins, impossible d'en ajouter un nouveau.");
		exit(-2);
	}

	tabAdh[*nbAdh] = (adherent *) malloc (sizeof (adherent));

	if (tabAdh[*nbAdh] == NULL)
	{
		printf("Probleme d'allocation memoire pour un nouvel adherent\n");
		exit(-1);
	}

	tabAdh[*nbAdh]->idAdherent = *nbAdh+1;
	printf ("Civilite, NOM, Prenom : ");
	scanf ("%s%s%s%*c", civ, nom, prenom);

	upperLower (civ);

	if (strcmp(civ, "Mr") != 0 && strcmp (civ, "Mme") != 0)
		while (strcmp(civ, "Mr") != 0 && strcmp (civ, "Mme") != 0)
		{
			printf ("Cette civilite n'existe pas, veuillez choisir entre Mr ou Mme : ");
			scanf ("%s%*c",civ);
			upperLower (civ);
		}

	upper (nom);
	upperLower (prenom);

	strcpy (tabAdh[*nbAdh]->civilite, civ);
	strcpy (tabAdh[*nbAdh]->nom, nom);
	strcpy (tabAdh[*nbAdh]->prenom, prenom);
	strcpy (tabAdh[*nbAdh]->dateIns.jours, date.jours);
	strcpy (tabAdh[*nbAdh]->dateIns.mois, date.mois);
	strcpy (tabAdh[*nbAdh]->dateIns.annee, date.annee);

	printf ("Nouvel adherent ajoute avec succes\n");
	*nbAdh+=1;
}

int			rechercheJeu(nodeType* head, char name[25], int *trouve, int nbJeux) // permet de savoir si un jeu existe ou non
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
			break;		
		}
		currentNode = currentNode->next;
		i++;
    }

	return currentGame.idJeu;
}

int			comptNbEmpAdh(emprunt tabEmp[], int idAdh, int nbEmp) // compte le nombre d'emprunts en cours d'un jeu donné en paramètre
{
	int i, nbEmpAdh = 0;

	for (i=0; i<nbEmp; i++)
	{
		if (tabEmp[i].idAdherent == idAdh)
			nbEmpAdh++;
	}
	
	return nbEmpAdh;
}

int 		dateIns (adherent *tabAdh[], date date, int i, int *exp) // vérifie la validité de l'adherent
{
	int ji, jn, mi, mn, ai, an;
	
	ji = atoi(tabAdh[i]->dateIns.jours);
	jn = atoi(date.jours);
	mi = atoi(tabAdh[i]->dateIns.mois);
	//mn = atoi(date.mois);
	mn = 3;
	ai = atoi(tabAdh[i]->dateIns.annee);
	an = atoi(date.annee);
	
	if (ai < an || ai-an < -1)
		if (mi < mn || mi == mn)
		{	
			if (ji < jn)
				*exp = -1;
			else
			{
				if (mi-mn <-1)
					*exp = -1;
			}
		}
	else
		*exp = -1;
}

int			rechercheJeuxAct (empResa tabEmpResa[], int idJeu, int idAdh, int nbEmp)
{
	int i, act;

	for (i = 0; i < nbEmp; i++)
		if (tabEmpResa[i].idJeu == idJeu && tabEmpResa[i].idAdherent == idAdh)
			return act = 1;

	return act;
}

empResa		*ajoutEmpResa(empResa tabEmpResa[], adherent *tabAdh[], nodeType* head, char name[25], int nbJeux, int *nb, int *nbMax, date date, int i, int idJeu) // ajoute un/e emprunt/reservation en fonction de ce qui lui est passé en paramètre
{
	empResa *newtab;

	if (*nb == *nbMax)
	{
		*nbMax+=5;
		newtab = (empResa *) realloc (tabEmpResa, *nbMax * sizeof (empResa));
		if (newtab == NULL)
		{
			printf("Probleme de reallocation\n");
			return tabEmpResa;
		}
		tabEmpResa = newtab;
	}

	tabEmpResa[*nb].idEmpResa = *nb+1;
	tabEmpResa[*nb].idAdherent = tabAdh[i]->idAdherent;
	tabEmpResa[*nb].idJeu = idJeu;
	strcpy(tabEmpResa[*nb].dateEmpResa.jours, date.jours);
	strcpy(tabEmpResa[*nb].dateEmpResa.mois, date.mois);	
	strcpy(tabEmpResa[*nb].dateEmpResa.annee, date.annee);

	*nb+=1;
	printf ("nouvel ajout effectue\n");

	return tabEmpResa;
}

emprunt 	*saisieNouvelEmprunt(adherent *tabAdh[], emprunt tabEmp[], nodeType* head, date date, int *nbAdh, int *nbEmp, int *nbEmpmax, int nbJeux, int max) // saisie les informations pour un nouvel emprunt (FONCTION N° 2)
{
	char nom[15], jeu[25], choix[4];
	int i = 0, nbEmpAdh, exp = 0, idJeu, trouve = -1, act = 0;

	printf ("Entrez le nom de l'emprunteur : ");
	scanf ("%s%*c",nom);
	upper (nom);

	while (strcmp(nom,tabAdh[i]->nom) != 0 && i<*nbAdh)
		i++;
		
	if (strcmp(nom,tabAdh[i]->nom) != 0)
	{
		printf ("cette personne n'a jamais ete enregistree, voulez vous l'ajoter ? oui/non : \n");
		scanf("%s%*c", choix);
		if (strcmp(choix, "oui") == 0)
			nouvelAdherent(tabAdh, date,nbAdh, max);
		else
			return tabEmp;
	}

	else
	{
		nbEmpAdh = comptNbEmpAdh(tabEmp, i+1, *nbEmp);
		if (nbEmpAdh == 3)
		{
			printf ("L'adherent %s %s a deja 3 emprunts.\n", tabAdh[i]->nom, tabAdh[i]->prenom);
			return tabEmp;
		}

		else
		{
			dateIns (tabAdh, date, i, &exp);
			if (exp == -1)
			{
				printf("L'adherent %s %s n'as pas payer sa cotisation annuelle.\n",  tabAdh[i]->nom, tabAdh[i]->prenom);
				return tabEmp;
			}
		}
		
	}

	printf ("Entrez le nom du jeu emprunte : ");
	scanf ("%s%*c", jeu);
	upperLower (jeu);

	idJeu = rechercheJeu(head, jeu, &trouve, nbJeux);

	if (trouve == -1)
	{
		printf("Ce jeu n'existe pas.\n");
		return tabEmp;
	}

	else
	{	
		act = rechercheJeuxAct (tabEmp, idJeu, i+1, *nbEmp);
		if (act == 1)
		{
			printf("Cet adherent a deja emprunte ce jeu.\n");
			return tabEmp;
		}
		else
		{
			trouve -= comptNbEmp(tabEmp, idJeu, *nbEmp);
			if (trouve == 0)
			{
				printf("Ce jeu n'est plus disponible, veuillez en faire une reservation.\n");
				return tabEmp;
			}
			else
				return ajoutEmpResa(tabEmp, tabAdh, head, jeu, nbJeux, nbEmp, nbEmpmax, date, i, idJeu);
		}
	}
}

reservation *saisieNouvelResa(adherent *tabAdh[], reservation tabResa[], emprunt tabEmp[], nodeType* head, date date, int *nbAdh, int *nbResa, int *nbResaMax, int nbEmp, int nbJeux, int max) // saisie les informations pour une nouvelle reservation (FONCTION N° 3)
{
	char nom[15], jeu[25], choix[4];
	int i = 0, exp, idJeu, trouve = -1, act = 0;

	printf ("Entrez le nom de l'adherent souhaitant reserver : ");
	scanf ("%s%*c",nom);
	upper (nom);

	while (strcmp(nom,tabAdh[i]->nom) != 0 && i<*nbAdh)
		i++;
		
	if (strcmp(nom,tabAdh[i]->nom) != 0)
	{
		printf ("cette personne n'a jamais ete enregistree, voulez vous l'ajoter ? oui/non : \n");
		scanf("%s%*c", choix);
		if (strcmp(choix, "oui") == 0)
			nouvelAdherent(tabAdh, date,nbAdh, max);
		else
			return tabResa;
	}
	else
	{
		dateIns (tabAdh, date, i, &exp);
		if (exp == -1)
		{
				printf("L'adherent %s %s n'as pas payer sa cotisation annuelle.\n",  tabAdh[i]->nom, tabAdh[i]->prenom);
				return tabEmp;
		}
	}
	
	printf ("Entrez le nom du jeu reserve: ");
	scanf ("%s%*c", jeu);
	upperLower (jeu);

	idJeu = rechercheJeu(head, jeu, &trouve, nbJeux);

	if (trouve == -1)
	{
		printf("Ce jeu n'existe pas.\n");
		return tabResa;
	}

	else
	{	
		act = rechercheJeuxAct (tabResa, idJeu, i+1, *nbResa);
		if (act == 1)
		{
			printf("Cet adherent a deja reserve ce jeu.\n");
			return tabEmp;
		}
		else
		{
			trouve -= comptNbEmp(tabEmp, idJeu, nbEmp);
			if (trouve > 0)
			{
				printf("Ce jeu est disponible, veuillez en faire un emprunt.\n");
				return tabResa;
			}
			else
				return ajoutEmpResa(tabResa, tabAdh, head, jeu, nbJeux, nbResa, nbResaMax, date, i, idJeu);
		}
	}
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

emprunt		*RetourJeu(adherent *tabAdh[], emprunt tabEmp[], emprunt tabResa[], int *nbEmp, int *nbResa, int nbAdh, date date) // retourne un jeu et regarde si une reservation peu passer en emprunt (FONCTION N°6)
{
	int i = 0, j, k = 0, val, exp = 0;
	int jeu;
	char nom[15];

	printf("Entrez le nom de l'emprunteur qui rend un jeu: ");
	scanf("%s", nom);
	upper(nom);

	while (strcmp(nom,tabAdh[i]->nom) != 0 && i < nbAdh)
		i++;

	if (strcmp(nom,tabAdh[i]->nom) != 0)
	{
		printf ("cette personne n'a jamais ete enregistree.\n");
		return tabEmp;
	}

	for (j = 0; j < *nbEmp; j++)
	{
		if (tabEmp[j].idAdherent == i+1)
		{
			k++;
			if (k ==1)
				printf ("\nID RESERVATION | ID ADHERENT | ID JEU | DATE EMPRUNT\n");
			printf ("%-14d | %-11d | %-6d | %s/%s/%s\n", tabEmp[j].idEmpResa, tabEmp[j].idAdherent, tabEmp[j].idJeu, tabEmp[j].dateEmpResa.jours, tabEmp[j].dateEmpResa.mois, tabEmp[j].dateEmpResa.annee);	
		}	
	}

	if (k == 0)
	{
		printf("Aucun emprunt en cours pour cet adherent\n");
		return tabEmp;
	}

	printf("\nChoissisez un numero d'emprunt pour cet adherent : ");
	scanf("%d", &val);

	if (tabEmp[val-1].idAdherent != i+1)	
		while (tabEmp[val-1].idAdherent != i+1)
		{
			printf("Votre numero d'emprunt pour cet adherent n'est pas correct veuillez resaisir : ");
			scanf("%d", &val);
		}

	jeu = tabEmp[val-1].idJeu;
	tabEmp = decalerGauche(tabEmp, nbEmp, val-1);
	printf("Retour bien effectue\n");
	
	for (j = 0; j < *nbResa ; j++)
		if (tabResa[j].idJeu == jeu)
		{	
			dateIns (tabAdh, date, i, &exp);
			if (comptNbEmpAdh(tabEmp, tabResa[j].idAdherent, *nbEmp) < 3 && exp == 0)
			{
				tabEmp[*nbEmp].idEmpResa = *nbEmp+2;
				tabEmp[*nbEmp].idAdherent = tabResa[j].idAdherent;
				tabEmp[*nbEmp].idJeu = tabResa[j].idJeu;
				strcpy(tabEmp[*nbEmp].dateEmpResa.jours, date.jours);
				strcpy(tabEmp[*nbEmp].dateEmpResa.mois, date.mois);	
				strcpy(tabEmp[*nbEmp].dateEmpResa.annee, date.annee);
				*nbEmp+=1;
				printf("La reservation numero %d au nom de %s %s est passee en emprunt\n", tabResa[j].idEmpResa, tabAdh[(tabResa[j].idAdherent)-1]->nom, tabAdh[(tabResa[j].idAdherent)-1]->prenom);
				tabResa = decalerGauche(tabResa, nbResa, j);
				
				break;
			}
		}

	return tabEmp;	
}

reservation	*AnnulationResa(adherent *tabAdh[], reservation tabResa[], int *nbResa, int nbAdh) // Annule une reservation (FONCTION N°7)
{
	int i = 0, j, k = 0, val, test = 0;
	char nom[15];
	
	printf("Entrez le nom de l'emprunteur qui annule une reservation: ");
	scanf("%s", nom);
	upper(nom);

	while (strcmp(nom, tabAdh[i]->nom) != 0 && i < nbAdh)
		i++;

	if (strcmp(nom,tabAdh[i]->nom) != 0)
	{
		printf ("cette personne n'a jamais ete enregistree.\n");
		return tabResa;
	}

	for (j = 0; j < *nbResa; j++)
	{
		if (tabResa[j].idAdherent == i+1)
		{
			printf ("%-10d | %-11d | %-6d | %s/%s/%s\n", tabResa[j].idEmpResa, tabResa[j].idAdherent, tabResa[j].idJeu, tabResa[j].dateEmpResa.jours, tabResa[j].dateEmpResa.mois, tabResa[j].dateEmpResa.annee);
			k++;	
		}	
	}

	if (k == 0)
	{
		printf("Aucune reservation en cours pour cet adherent\n");
		return tabResa;
	}

	printf("Choissisez un numero de reservation pour cet adherent : ");
	scanf("%d", &val);

	if (tabResa[val-1].idAdherent != i+1)	
		while (tabResa[val-1].idAdherent != i+1)
		{
			printf("Votre numero de reservation n'est pas correct pour cet adherent veuillez resaisir : ");
			scanf("%d", &val);
		}

	tabResa = decalerGauche(tabResa, nbResa, val-1);
	printf("Annulation bien effectuee\n");

	return tabResa;	
}	


void 		saveJeux(nodeType *head, int nbJeux) //sauvegarde la liste de jeux dans son fichier binaire
{
	nodeType*	currentNode = head;
	jeu			currentGame;
	FILE *flot;

	flot = fopen ("jeux.bin","wb");
	
	if (flot == NULL)
	{
		printf("Probleme d'ouverture du fichier jeux.bin en écriture");
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
	printf("Sauvegarde des jeux terminee\n");
}

void 		saveAdh(adherent **tabAdh, int nbAdh) // sauvegarde le tableau de pointeur d'adherents dans son fichier binaire
{
	FILE *flot;
	int i;

	flot = fopen ("adherents.bin","wb");

	if (flot == NULL)
	{
		printf("Probleme d'ouverture du fichier adherents.bin en écriture");
		return;
	}

	fwrite(&nbAdh, sizeof(int), 1, flot);

	for (i = 0; i < nbAdh; i++)
		fwrite(tabAdh[i], sizeof(adherent), 1, flot);

	fclose(flot);
	printf("Sauvegarde des adherents terminee\n");
}

void 		saveEmp(emprunt *tabEmp, int nbEmp) // sauvegarde le tableau d'emprunt dans son fichier binaire
{
	FILE *flot;

	flot = fopen("emprunt.bin","wb");

	if (flot == NULL)
	{
		printf("Probleme d'ouverture du fichier emprunt.bin en écriture");
		return;
	}

	fwrite(&nbEmp, sizeof(int), 1, flot);
	fwrite(tabEmp, sizeof(emprunt), nbEmp, flot);
	fclose(flot);

	printf("Sauvegarde des emprunts terminee\n");
}

void 		saveResa(reservation *tabResa, int nbResa) // sauvegarde le tableau de reservation dans son fichier binaire
{
	FILE *flot;

	flot = fopen("reservation.bin","wb");

	if (flot == NULL)
	{
		printf("Probleme d'ouverture du fichier reservation.bin en écriture");
		return;
	}

	fwrite(&nbResa, sizeof(int),1,flot);
	fwrite(tabResa, sizeof(reservation),nbResa,flot);
	fclose(flot);

	printf("Sauvegarde des reservations terminee\n");
}

void 		sauvegarder (nodeType *head, adherent **tabAdh, emprunt *tabEmp, reservation *tabResa, int nbJeux, int nbAdh, int nbEmp, int nbResa) // sauvegarde tout les tableaux  dans un fichier binaire
{
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

	flot = fopen ("jeux.txt","w");
	
	if (flot == NULL)
	{
		printf("Probleme d'ouverture du fichier jeux.txt en écriture");
		return;
	}

	fprintf(flot, "Nombre total de jeux : %d\n",nbJeux);
	fprintf(flot, "\nID JEU |            NOM            |      TYPE     | NOMBRE D'EXEMPLAIRE\n");

	while (currentNode != NULL) 
	{
		currentGame = currentNode->game;
		fprintf(flot, "%-6d | %-25.25s | %-13.13s | %d\n", currentGame.idJeu, currentGame.nom, currentGame.type, currentGame.nbExp);
		currentNode = currentNode->next;
    }

	fclose(flot);
	printf("Sauvegarde des jeux en texte terminee\n");
}


void 		saveAdhTxt(adherent **tabAdh, int nbAdh) //sauvegarde le tableau de pointeur d'adherents dans son fichier texte
{
	FILE *flot;
	int i;

	flot = fopen ("adherents.txt","w");

	if (flot == NULL)
	{
		printf("Probleme d'ouverture du fichier adherents.txt en écriture");
		return;
	}

	fprintf(flot, "Nombre total d'adherents : %d\n", nbAdh);
	fprintf(flot, "\nID ADHERENT | CIVILITE |       NOM       |      PRENOM     |   DATE\n");

	for (i = 0; i < nbAdh; i++)
		fprintf(flot,"%-11d | %-8.8s | %-15.15s | %-15.15s | %s/%s/%s\n", tabAdh[i]->idAdherent, tabAdh[i]->civilite, tabAdh[i]->nom, tabAdh[i]->prenom, tabAdh[i]->dateIns.jours, tabAdh[i]->dateIns.mois, tabAdh[i]->dateIns.annee);

	fclose(flot);
	printf("Sauvegarde des adhenrents en texte terminee\n");
}

void 		saveEmpTxt(emprunt *tabEmp, int nbEmp) //sauvegarde le tableau d'emprunt dans son fichier texte
{
	FILE *flot;
	int i;

	flot = fopen("emprunt.txt","w");

	if (flot == NULL)
	{
		printf("Probleme d'ouverture du fichier emprunt.txt en écriture");
		return;
	}

	fprintf(flot, "Nombre d'emprunt en cours : %d\n", nbEmp);
	fprintf(flot, "\nID EMPRUNT | ID ADHERENT | ID JEU |   DATE\n");

	for ( i = 0; i < nbEmp; i++)
		fprintf(flot, "%-10d | %-11d | %-6d | %s/%s/%s\n", tabEmp[i].idEmpResa, tabEmp[i].idAdherent, tabEmp[i].idJeu, tabEmp[i].dateEmpResa.jours, tabEmp[i].dateEmpResa.mois, tabEmp[i].dateEmpResa.annee);

	fclose(flot);
	printf("Sauvegarde des emprunts en texte terminee\n");
}

void 		saveResaTxt(reservation *tabResa, int nbResa) //sauvegarde le tableau de reservation dans son fichier texte
{
	FILE *flot;
	int i;

	flot = fopen("reservation.txt","w");
	
	if (flot == NULL)
	{
		printf("Probleme d'ouverture du fichier emprunt.txt en écriture");
		return;
	}

	fprintf(flot, "Nombre d'emprunt en cours : %d\n", nbResa);
	fprintf(flot, "\nID RESERVATION | ID ADHERENT | ID JEU |   DATE\n");

	for ( i = 0; i < nbResa; i++)
		fprintf(flot, "%-14d | %-11d | %-6d | %s/%s/%s\n", tabResa[i].idEmpResa, tabResa[i].idAdherent, tabResa[i].idJeu, tabResa[i].dateEmpResa.jours, tabResa[i].dateEmpResa.mois, tabResa[i].dateEmpResa.annee);

	fclose(flot);
	printf("Sauvegarde des reservation en texte terminee\n");
}

void 		sauvegarderTxt (nodeType *head, adherent **tabAdh, emprunt *tabEmp, reservation *tabResa, int nbJeux, int nbAdh, int nbEmp, int nbResa) // sauvegarde totu les tableaux en binaire et txt
{
	sauvegarder (head, tabAdh, tabEmp, tabResa, nbJeux, nbAdh, nbEmp, nbResa);
	saveJeuxTxt(head, nbJeux);
	saveAdhTxt(tabAdh, nbAdh);
	saveEmpTxt(tabEmp, nbEmp);
	saveResaTxt(tabResa, nbResa);
}


int			menuSave(void) // menu affichage save (FONCTION N° 8)
{
	int choix;

    printf("\n");
	printf("-----------------------------------------------------------------\n");
	printf("| 1.	Sauvegarder en fichier binaire				|\n");
	printf("| 2.	Sauvegarde en fichier binaire et texte			|\n");
	printf("| 3.	QUITTER							|\n");
	printf("-----------------------------------------------------------------\n");
    scanf("%d",&choix);

    return choix;
}