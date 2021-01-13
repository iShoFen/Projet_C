#include"proj.h"

void global(void)
{
	adherent *tabAdh[1000];

    emprunt *tabEmp;

    reservation *tabResa;

    nodeType* head = NULL;
    nodeType* doublon = NULL;

    date date;

	char c;
	int choix, choixJeu, choixSave;
    int nbAdh, nbEmp, nbResa, nbJeux;
    int nbEmpMax, nbResaMax;

    head = ChargementJeux(head, &nbJeux); // charge le fichier jeux.txt dans une liste chainée
    nbAdh = ChargementAdherents(tabAdh, 1000); // charge le fichier adherent.txt dans un tableau de pointeurs
    tabEmp = ChargementEmprunt(&nbEmp); // charge le fichier emprunt.txtx dans un tableau tableau alloué dynamiquement
    tabResa = ChargementReservation(&nbResa); // charge le fichier reservation.txt dans un tableau alloué dynamiquement
    date = dateNow(date); // permet de connaitre la date du jours en format français jj/mm/aaaa
    nbEmpMax = nbEmp; 
    nbResaMax = nbResa;

    choix = menu(); // permet de connaitre le choix de l'utilsiateur
	while (choix!=9)
    {
        switch (choix)
        {
            case 1 : choixJeu = menuJeu(); // permet de connaitre choix de l'utilisateur pour l' affichage des jeux 
                    while (choixJeu != 3)
                    {
                        switch (choixJeu)
                        {
                            case 1 : triAlphaJeu(head, tabEmp, nbEmp); break; // affiche les jeux triés par ordre alphabétique
                            case 2 : triTypeJeu(head, tabEmp, nbEmp); break; // affiche les jeux triés par type
                        }
                        fflush(stdin); // fonction permettant de vider le buffer 
                        printf("\ntapez entree pour continuer ...");
                        c = getchar();
                        choixJeu = menuJeu();
                    } break;
            case 2 : printEmprunt(tabEmp, date, nbEmp); break; // affiche tous les emprunts en cours
            case 3 : printResa(tabResa, head, nbResa, nbJeux); break; // affiche toutes les reservation en cours en fonction d'un jeu donné
            case 4 : tabEmp = saisieNouvelEmprunt(tabAdh, tabEmp, head, date, &nbAdh, &nbEmp, &nbEmpMax, nbJeux, 1000); break; // permet de saisir un nouvel emprunt
            case 5 : tabResa = saisieNouvelResa(tabAdh, tabResa, tabEmp, head, date, &nbAdh, &nbResa, &nbResaMax, nbEmp, nbJeux, 1000); break; // saisie d'une nouvelle reservation
            case 6 : tabEmp = RetourJeu(tabAdh, tabEmp, tabResa, &nbEmp, &nbResa, nbAdh, date); break; // effectue le retour d'un jeu, sa suppression du fichier emprunts.txt et l'actualisation d'une reservation si possible
            case 7 : tabResa = AnnulationResa(tabAdh, tabResa, &nbResa, nbAdh); break;// annulationresa()
            case 8 : choixSave = menuSave(); // permet de connaitre choix de l'utilisateur pour la saauvegarde 
                    while (choixSave != 3)
                    {
                        switch (choixSave)
                        {
                            case 1 : sauvegarder(head, tabAdh, tabEmp, tabResa, nbJeux, nbAdh, nbEmp, nbResa); break; // sauvegarde en fichier binaire
                            case 2 : sauvegarderTxt(head, tabAdh, tabEmp, tabResa, nbJeux, nbAdh, nbEmp, nbResa); break; // sauvegarde en fichier binaire et texte   
                        }
                        fflush(stdin); // fonction permettant de vider le buffer 
                        printf("\ntapez entree pour continuer ...");
                        c = getchar();
                        choixSave = menuSave();
                    } break;
        }
        fflush(stdin);
        printf("\ntapez entree pour continuer ...");
        c = getchar();
        choix = menu();
    }
    freeListe(head);
    free(*tabAdh);
    free(tabEmp);
    free(tabResa);
}

int 	main(void)
{
	global();
	return 0;
}