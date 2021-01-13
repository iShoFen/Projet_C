#include"proj.h"

void global(void)
{
	adherent *tabAdh[100];

    emprunt *tabEmp;

    reservation *tabResa;

    nodeType* head = NULL;
    nodeType* doublon = NULL;

    char date[11], c;
	int choix;
    int nbAdh, nbEmp, nbResa, nbJeux;
    int idEmp, nbEmpMax, idResa, nbResaMax;

    head = ChargementJeux(head, &nbJeux); // charge le fichier jeux.txt dans une liste chainée
    nbAdh = ChargementAdherents(tabAdh, 100); // charge le fichier adherent.txt dans un tableau de pointeurs
    tabEmp = ChargementEmprunt(&nbEmp, &idEmp); // charge le fichier emprunt.txtx dans un tableau tableau alloué dynamiquement
    tabResa = ChargementReservation(&nbResa, &idResa); // charge le fichier reservation.txt dans un tableau alloué dynamiquement
    dateNow(date); // permet de connaitre la date du jours en format français jj/mm/aaaa
    nbEmpMax = nbEmp; 
    nbResaMax = nbResa;

    choix = menu(); // permet de connaitre le choix de l'utilsiateur
	while (choix != 10)
    {
        switch (choix)
        {
            case 1 : choix = menuJeu(); // permet de connaitre choix de l'utilisateur pour l' affichage des jeux 
                    while (choix != 3)
                    {
                        switch (choix)
                        {
                            case 1 : triAlphaJeu(head, tabEmp, nbEmp); break; // affiche les jeux triés par ordre alphabétique
                            case 2 : printAllJeu(head, tabEmp, nbEmp); break; // affiche tous les jeux
                        }
                        fflush(stdin); // fonction permettant de vider le buffer 
                        printf("\ntapez entrée pour continuer ...");
                        c = getchar();
                        choix = menuJeu();
                    } break;
            case 2 : printEmprunt(tabEmp, date, nbEmp) ; break; // affiche tous les emprunts en cours
            case 3 : printResa(tabResa, head, nbResa, nbJeux); break; // affiche toutes les reservation en cours en fonction d'un jeu donné
            case 4 : head = saisieNouveauJeu(head, &nbJeux); break; // crée un nouveau jeu
            case 5 : tabEmp = saisieNouvelEmprunt(tabAdh, tabEmp, head, date, &nbAdh, &nbEmp, &idEmp, &nbEmpMax, nbJeux, 100); break; // permet de saisir un nouvel emprunt
            case 6 : tabResa = saisieNouvelResa(tabAdh, tabResa, tabEmp, head, date, &nbAdh, &nbResa, &idResa, &nbResaMax, nbEmp, nbJeux, 100); break; // saisie d'une nouvelle reservation
            case 7 : tabEmp = RetourJeu(tabAdh, tabEmp, tabResa, &nbEmp, &idEmp, &nbResa, nbAdh, date); break; // effectue le retour d'un jeu, sa suppression du fichier emprunts.txt et l'actualisation d'une reservation si possible
            case 8 : tabResa = AnnulationResa(tabAdh, tabResa, &nbResa, nbAdh); break;// annulationresa()
            case 9 : choix = menuSave(); // permet de connaitre choix de l'utilisateur pour la saauvegarde 
                    while (choix != 3)
                    {
                        switch (choix)
                        {
                            case 1 : sauvegarder(head, tabAdh, tabEmp, tabResa, nbJeux, nbAdh, nbEmp, nbResa); break; // sauvegarde en fichier binaire
                            case 2 : sauvegarderTxt(head, tabAdh, tabEmp, tabResa, nbJeux, nbAdh, nbEmp, nbResa); break; // sauvegarde en fichier binaire et texte   
                        }
                        fflush(stdin);
                        printf("\ntapez entrée pour continuer ...");
                        c = getchar();
                        choix = menuSave();
                    } break;
        }
        fflush(stdin);
        printf("\ntapez entrée pour continuer ...");
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