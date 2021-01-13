#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>

typedef struct  // crée la strcuture pour le fichier jeux
{
    int idJeu;
    char nom[25];
    char type[13];
    int nbExp;
} jeu;

typedef struct
{
    char jours[3];
    char mois[3];
    char annee[5];
} date;


typedef struct  // crée la structure pour le fichier adherents
{
    int idAdherent;
    char civilite[4];
    char nom[15];
    char prenom[15];
    date dateIns;
} adherent;

typedef struct  // crée la structure pour le fichier adherents et reservation
{
    int idEmpResa;
    int idAdherent;
    int idJeu;
    date dateEmpResa;
} emprunt, reservation, empResa;

typedef struct node
{
    jeu game;
    struct node *next;
} nodeType;
//maillon de liste chainée

// prototype menu
int         menu(void);

// prototypes date du jour
date 		dateNow(date date);

// prototypes upper et lower 
void 		upper (char tab[]);
void 		upperLower (char tab[]);

// prototypes  chargement jeux
nodeType   	*createNode(jeu game);
nodeType   	*lastNode(nodeType *head);
void       	createLastNode(nodeType **head, jeu game);
nodeType   	*ChargementJeux(nodeType* head, int *nbJeux);
void		freeListe(nodeType *head);

// prototype affichage jeux
int			comptNbEmp(emprunt tabEmp[], int idJeu, int nbEmp); 
void		printJeu(nodeType* head, emprunt tabEmp[], int nbEmp);
nodeType 	*copyList(nodeType *head);
void       	triAlphaJeu(nodeType *head, emprunt tabEmp[], int nbEmp);
void       	triTypeJeu(nodeType *head, emprunt tabEmp[], int nbEmp);
int		   	menuJeu(void);

// prototypes  chargement adherents
int        	ChargementAdherents (adherent *tab[], int max);

// prototypes  chargement emprunts/reservation
empResa		*lectureEmpResa (empResa tabEmpResa[], int *nbEmpResa, FILE *flot);
emprunt    	*ChargementEmprunt (int *nbEmp);
reservation	*ChargementReservation (int *nbResa);

// prototype affichage emprunts/reservations
void        printEmprunt (emprunt tab[], date date, int max);
void 		printResa(reservation *tabResa, nodeType *head, int nbResa, int nbJeux);

// prototypes pour nouvel emprunt/reservation
void 		nouvelAdherent(adherent *tabAdh[], date date,int *nbAdh, int max);
int			rechercheJeu(nodeType* head, char name[25], int *trouve, int nbJeux);
int			comptNbEmpAdh(emprunt tabEmp[], int idAdh, int nbEmp);
int 		dateIns (adherent *tabAdh[], date date, int i, int *exp);
int			rechercheJeuxAct (emprunt tabEmp[], int idJeu, int idAdh, int nbEmp);
empResa 	*ajoutEmpResa(empResa tabEmpresa[], adherent *tabAdh[], nodeType* head, char name[25], int nbJeux, int *nb, int *nbMax, date date, int i, int idJeu);
emprunt 	*saisieNouvelEmprunt(adherent *tabAdh[], emprunt tabEmp[],nodeType* head, date date, int *nbAdh, int *nbEmp, int *nbEmpmax, int nbJeu, int max);
reservation *saisieNouvelResa(adherent *tabAdh[], reservation tabResa[], emprunt tabEmp[], nodeType* head, date date, int *nbAdh, int *nbResa, int *nbResaMax, int nbEmp, int nbJeux, int max);

// prototype retour jeu/annulation reservation
empResa 	*decalerGauche(empResa tabEmpResa[], int *nbEmpResa, int val);
emprunt		*RetourJeu(adherent *tabAdh[], emprunt tabEmp[], emprunt tabResa[], int *nbEmp, int *nbResa, int nbAdh, date date);
reservation	*AnnulationResa(adherent *tabAdh[], reservation tabResa[], int *nbResa, int nbAdh);

// free pour la liste
void		freeListe(nodeType *head);

// prototypes sauvegarde
void        saveJeux(nodeType *head, int nbJeux);
void        saveAdh(adherent **tabAdh, int nbAdh);
void        saveEmp(emprunt *tabEmp, int nbEmp);
void        saveResa(reservation *tabResa, int nbResa);
void        sauvegarder(nodeType *head, adherent **tabAdh, emprunt *tabEmp, reservation *tabResa, int nbJeux, int nbAdh, int nbEmp, int nbResa);
void        saveJeuxTxt(nodeType *head, int nbJeux);
void        saveAdhTxt(adherent **tabAdh, int nbAdh);
void        saveEmpTxt(emprunt *tabEmp, int nbEmp);
void        saveResaTxt(reservation *tabResa, int nbResa);
void        sauvegarderTxt(nodeType *head, adherent **tabAdh, emprunt *tabEmp, reservation *tabResa, int nbJeux, int nbAdh, int nbEmp, int nbResa);
int			menuSave(void);