#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h> //bibliotèque pour la lecture de la date système
#include<ctype.h> // bibliotqèue pour le upper et lower char

#if defined WIN32
#define CLEAN_SCREEN "cls"
#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
#define CLEAN_SCREEN "clear"
#endif


typedef struct  // crée la strcuture pour le fichier jeux
{
    int idJeu;
    char nom[26];
    char type[14];
    int nbExp;
} jeu;

typedef struct  // crée la structure pour le fichier adhérents
{
    int idAdherent;
    char civilite[4];
    char nom[16];
    char prenom[16];
    char dateIns[11];
} adherent;

typedef struct  // crée la structure pour le fichier adhérents et réservation
{
    int idEmpResa;
    int idAdherent;
    int idJeu;
    char dateEmpResa[11]; 
} emprunt, reservation, empResa;

typedef struct node //crée la structure pour la liste chaînée
{
    jeu game;
    struct node *next;
} nodeType;

// prototype menu
int         menu(void);

// prototype date du jour
void        dateNow(char date[]);

// prototypes chaine de caractère
void 		upper(char tab[]);
void 		upperLower(char tab[]);
void		formatTxt(char tab[]);
void		formatBin(char tab[]);

// prototypes chargement jeux
nodeType   	*createNode(jeu game);
nodeType   	*lastNode(nodeType *head);
void       	createLastNode(nodeType **head, jeu game);
nodeType   	*ChargementJeux(nodeType* head, int *nbJeux);

// prototypes chargement adhérents
int        	ChargementAdherents(adherent *tab[], int max);

// prototypes chargement emprunts/réservation
empResa		*lectureEmpResa(empResa tabEmpResa[], int nbEmpResa, FILE *flot);
emprunt    	*ChargementEmprunt (int *nbEmp, int *idEmp);
reservation	*ChargementReservation (int *nbResa, int *idResa);

// prototypes affichage jeux
nodeType 	*copyList(nodeType *head);
void        copierL(nodeType *headbis, int nbJ, nodeType **GD, nodeType **S);
nodeType    *fusionL(nodeType *G, nodeType *D, int choix);
nodeType    *triFusionL(nodeType *headbis, int nbJ, int choix);
void        triSwitchJeu(nodeType *head, int choix);
int			comptNbEmp(emprunt tabEmp[], int idJeu, int nbEmp); 
void		freeListe(nodeType *head);
void		printJeu(nodeType* head, emprunt tabEmp[], int nbJeux, int nbEmp);
void        printAllJeu(nodeType* head, emprunt tabEmp[], int nbEmp);
int		   	menuJeu(void);

//prototype affichage adhérents
void		printAdherent(adherent *tabAdh[],int nbAdh);

// prototypes affichage emprunts/réservations
int 		dateEmp(emprunt tabEmp[], char date[], int i);
void        printEmprunt (emprunt tabEmp[], char date[], int max);
void 		printResa(reservation *tabResa, nodeType *head, int nbResa, int nbJeux);

// prototype pour nouveau jeu
nodeType	*saisieNouveauJeu(nodeType *head, int *nbJeux);

// prototypes pour nouvel emprunt/réservation
int			rechercheAdherent(adherent *tabAdh[], char nom[], char prenom[], int nbAdh);
void 		nouvelAdherent(adherent *tabAdh[], char nom[], char prenom[], char date[], int *nbAdh, int max);
int 		dateIns(adherent *tabAdh[], char date[], int i);
int			comptNbEmpAdh(emprunt tabEmp[], int idAdh, int nbEmp);
int         RetardEmp(emprunt tabEmp[], char date[], int nbEmp, int idAdh);
int			rechercheJeu(nodeType* head, char name[25], int nbJeux, int *trouve);
int			rechercheJeuxAct(emprunt tabEmp[], int idJeu, int idAdh, int nbEmp);
empResa		*ajoutEmpResa(empResa tabEmpResa[], adherent *tabAdh[], int *nbEmpResa, int *nbEmpResaMax, int *idEmpResa, int idAdh, int idJeu, char date[]);
emprunt 	*saisieNouvelEmprunt(adherent *tabAdh[], emprunt tabEmp[],nodeType* head, char date[], int *nbAdh, int *nbEmp, int *idEmp, int *nbEmpmax, int nbJeu, int max);
reservation *saisieNouvelResa(adherent *tabAdh[], reservation tabResa[], emprunt tabEmp[], nodeType* head, char date[], int *nbAdh, int *nbResa, int *idResa,int *nbResaMax, int nbEmp, int nbJeux, int max);

// prototypes retour jeu/annulation réservation
int		    printEmpResa(empResa tabEmpResa[], int nbEmpResa, int id);
empResa 	*decalerGauche(empResa tabEmpResa[], int *nbEmpResa, int val);
int			rechercheResa(reservation tabResa[], int nbResa, int idjeu);
emprunt		*RetourJeu(adherent *tabAdh[], emprunt tabEmp[], emprunt tabResa[], int *nbEmp, int *idEmp, int *nbResa, int nbAdh, char date[]);
reservation	*AnnulationResa(adherent *tabAdh[], reservation tabResa[], int *nbResa, int nbAdh);

// prototypes sauvegarde
void        saveJeux(nodeType *head, int nbJeux);
void        saveAdh(adherent **tabAdh, int nbAdh);
void        saveEmp(emprunt *tabEmp, int nbEmp);
void        saveResa(reservation *tabResa, int nbResa);
void        sauvegarder(nodeType *head, adherent **tabAdh, emprunt *tabEmp, reservation *tabResa, int nbJeux, int nbAdh, int nbEmp, int nbResa);
void        saveJeuxTxt(nodeType *head, int nbJeux, char dest[]);
void        saveAdhTxt(adherent **tabAdh, int nbAdh, char dest[]);
void        saveEmpTxt(emprunt *tabEmp, int nbEmp, char dest[]);
void        saveResaTxt(reservation *tabResa, int nbResa, char dest[]);
void        sauvegarderTxt(nodeType *head, adherent **tabAdh, emprunt *tabEmp, reservation *tabResa, int nbJeux, int nbAdh, int nbEmp, int nbResa);
int			menuSave(void);