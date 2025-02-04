/****************************************************************************************
* menu
*
*
* Programmation procedurale 2024 - A.Umina - image blason version 1
****************************************************************************************/
#include "menu.h"
#include "DB_Main.h"
#include <sys/stat.h>

void menu(dbb *db);
void Afficher_Info_DB(const dbb *db);
void menu_DB(dbb *db);
void Menu_Lister(dbb *db);
void menu_rech_ind(dbb *db);

/**********************************************************************************
* Fonction pour gérer le menu principal
*      0.  Fin
*
*      1.  Ouvrir DB
*      2.  Cree BD vide
*      3.  Informations systeme
*
* Valeur de retour : aucune
**********************************************************************************/
void menu(dbb *db)
{
    int choix;

    do
    {
        system("cls");   // Commande Windows pour nettoyer l'écran

        // Affiche le statut actuel de la base de données en fonction de sa valeur
        printf("\n--- Base de donnees imperiale V3 : %s\n", db->status == 0 ? "Fermee ---" : "Ouverte ---");
        printf("\n");
        printf("1.  Ouvrir DB\n");
        printf("2.  Creer DB vide\n");
        printf("3.  Informations systeme\n");
        printf("\n");
        printf("0. Fin\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);
        getchar();  // Pour consommer le caractère de nouvelle ligne laissé par scanf

        // Traitement du choix de l'utilisateur
        switch (choix)
        {
        case OUVRIR_DB:
            Open_DB(db);
            db->status = 1;  // Marquer la base de données comme "ouverte"
            system("cls");
            menu_DB(db);
            break;
        case CREE_DB:
            Create_DB(db);
            db->status = 1;  // Marquer la base de données comme "ouverte"
            break;
        case INFO_SYST:
            system("cls");
            Afficher_Info_DB(db);
            break;
        case FIN:
            printf("Merci d'avoir utilise le programme !\n");
            break;
        default:
            printf("Choix invalide. Veuillez essayer à nouveau.\n");
            break;
        }

        // Pause avant d'afficher à nouveau le menu
        if (choix != 0)
        {
            printf("\nAppuyez sur Entree pour continuer...");
            getchar();  // Attend que l'utilisateur appuie sur Entrée
        }

    }
    while (choix != 0);    // Boucle jusqu'à ce que l'utilisateur choisisse de quitter
}

/**********************************************************************************
* Fonction information de la base de donnees
* Valeur de retour : aucune
**********************************************************************************/
void Afficher_Info_DB(const dbb *db)
{
    printf("informations systeme : base de donnees blason\n");
    printf("\n");
    if (db->status == 0)
    {
        printf("DB fermee\n");
    }
    else
    {
        printf("DB Status          : OPEN\n");
        printf("DB Name            : %s\n", db->hdr.db_name);
        printf("DB SIZE            : %u octets\n", db->hdr.db_size);
        printf("\n");
        printf("SIZE pays          : %d\n", db->hdr.sz_pay);
        printf("SIZE duche         : %d\n", db->hdr.sz_duc);
        printf("SIZE ville         : %d\n", db->hdr.sz_ville);
        printf("SIZE noblesse      : %d\n", db->hdr.sz_nob);
        printf("SIZE blason        : %d\n", db->hdr.sz_blaz);
        printf("SIZE charge        : %d\n", db->hdr.sz_charge);
        printf("SIZE individu      : %d\n", db->hdr.sz_cit);
        printf("\n");
        printf("NR of pays         : %d\n", db->hdr.nbr_pay);
        printf("NR of duche        : %d\n", db->hdr.nbr_duc);
        printf("NR of ville        : %d\n", db->hdr.nbr_ville);
        printf("NR of noblesse     : %d\n", db->hdr.nbr_nob);
        printf("NR of blason       : %d\n", db->hdr.nbr_blaz);
        printf("NR of charge       : %d\n", db->hdr.nbr_charge);
        printf("NR of individu     : %d\n", db->hdr.nbr_cit);


    }
}

/**********************************************************************************
* Fonction pour gérer labase de donnees
       0.  Fermer DB

       1.  Importer Fichier dans DB
       2.  Exporter BD dans fichier
       3.  Recherche sequentielle dans DB
       4.  Recherche indexee dans DB
       5.  Produire Rapports
       6.  Informations systeme
* Valeur de retour : aucune
**********************************************************************************/
void menu_DB(dbb *db)
{
    int choix;
    do
    {
        system("cls");   // Commande Windows pour nettoyer l'écran
        printf("\n--- Base de donnees imperiale V3 : OPEN ---\n");
        printf("1.  Importer Fichiers dans DB \n");
        printf("2.  Exporter DB dans fichiers\n");
        printf("3.  Cree index\n");
        printf("4.  Lister\n");
        printf("5.  Liste doublement chainee\n");
        printf("6.  Recherche indexee dans DB\n");
        printf("7.  Produire rapports\n");
        printf("8.  Informations systeme\n");

        printf("\n");

        printf("0. Fermer DB\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);
        getchar();  // Pour consommer le caractère de nouvelle ligne laissé par scanf

        switch (choix)
        {
        case IMPORT:
            system("cls");
            Import_Citoyens(db);
            Import_Noblesses(db);
            Import_Charges(db);
            Import_Pays(db);
            Import_Duches(db);
            Import_Villes(db);
            Import_Richesses(db);
            Import_Blason(db);
            break;
        case EXPORT:
            system("cls");
            Export_Citoyens(db);
            Export_Noblesses(db);
            Export_Charges(db);
            Export_Pays(db);
            Export_Duches(db);
            Export_Villes(db);
            Export_Richesses(db);
            Export_Blason(db);
            break;
        case CREE_INDEX:
            system("cls");
            Creer_index(db);
            Afficher_Index_Cit_id(db);
            break;
        case LISTER:
            system("cls");
            Menu_Lister(db);
            break;
        case LIST_DC:
            system("cls");
            menu_List_DC(db);
            break;
        case RECH_IND:
            system("cls");
            menu_rech_ind(db);
            break;
        case RAPPORT:
            menu_rapport(db);
            break;
        case INFO_DB:
            system("cls");
            Afficher_Info_DB(db);
            break;
        case FIN:
            Close_DB(db);
            break;
        default:
            printf("Choix invalide. Veuillez essayer à nouveau.\n");
            break;
        }
        // Pause avant d'afficher à nouveau le menu
        if (choix != 0)
        {
            printf("\nAppuyez sur Entrée pour continuer...");
            getchar();  // Attend que l'utilisateur appuie sur Entrée
        }

    }
    while (choix != 0);      // Boucle jusqu'à ce que l'utilisateur choisisse de quitter
}
/**********************************************************************************
* Fonction pour lister des donnees
       0.  Retour

       1.  Lister Pays
       2.  Lister Duches
       3.  Lister Villes
       4.  Lister Noblesse
       5.  Lister Charges
       6.  Lister Citoyens
* Valeur de retour : aucune
**********************************************************************************/
void Menu_Lister(dbb *db)
{
    int choix;
    do
    {
        system("cls");   // Commande Windows pour nettoyer l'écran
        printf("\n--- Menu Lister ---\n");
        printf("1.  Lister Pays \n");
        printf("2.  Lister Duches \n");
        printf("\n");
        printf("0. Retour \n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);
        getchar();  // Pour consommer le caractère de nouvelle ligne laissé par scanf

        switch (choix)
        {
        case PAYS:
            system("cls");
            Load_Buffers_pays(db);
            Lister_Pays_BUF(db);
            break;
        case DUCHER:
            system("cls");
            Load_Buffers_duch(db);
            Lister_duch_BUF(db);
            break;
        case FIN:

            break;
        default:
            printf("Choix invalide. Veuillez essayer à nouveau.\n");
            break;
        }
        // Pause avant d'afficher à nouveau le menu
        if (choix != 0)
        {
            printf("\nAppuyez sur Entrée pour continuer...");
            getchar();  // Attend que l'utilisateur appuie sur Entrée
        }

    }
    while (choix != 0);      // Boucle jusqu'à ce que l'utilisateur choisisse de quitter
}

/**********************************************************************************
* Fonction pour gérer les recherches indexées
*
*       0.  Retour
*
*       1.  Lister Roi Sequentiel
*       2.  Lister Dichotomique
*       3.  Recherche charge index arbre
*
*
* Valeur de retour : aucune
**********************************************************************************/
void menu_rech_ind(dbb *db)
{
    int choix;

    do
    {
        system("cls");   // Commande Windows pour nettoyer l'écran
        printf("\n--- Menu recherche indexee\n");
        printf("\n");
        printf("1. Lister Roi par Pays (Sequentiel et indexee)\n");
        printf("2. Recherche Citoyens par prefix\n");
        printf("3. Recherche charge index arbre\n");
        printf("4. Recherche dichotomique de l offset du citoyens pas id\n");
        printf("5. Recherche du citoyens par offset\n");
        printf("0. Retour\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);
        getchar();  // Pour consommer le caractère de nouvelle ligne laissé par scanf
        // Traitement du choix de l'utilisateur
        switch (choix)
        {
        case ROI_SEQ:
            Recherche_roi_pays(db);
            break;
        case CIT_IND:
            Recherche_Par_Prefix(db);
            break;
        case CHA_ARB:
            Recherche_Id_Cha_Arb(db);
            break;
        case 4:
            Recherche_Off_Cit_Par_Id(db);
            break;
        case 5:
            Recherche_Cit_Par_off(db);
            system("pause");
        case RETOUR2:
            printf("Retour au menu DB\n");
            break;
        default:
            printf("Choix invalide. Veuillez essayer à nouveau.\n");
            break;
        }

        // Pause avant d'afficher à nouveau le menu
        if (choix != 0)
        {
            printf("\nAppuyez sur Entree pour continuer...");
            getchar();  // Attend que l'utilisateur appuie sur Entrée
        }

    }
    while (choix != 0);    // Boucle jusqu'à ce que l'utilisateur choisisse de quitter
}
/**********************************************************************************
* Fonction pour gérer des rapports
*
*       0.  Retour
*
*       1.  Rapport Roi par zone et par date
*       2.  Rapport Marquis par zone et par periode
*       3.
*
*
* Valeur de retour : aucune
**********************************************************************************/
void menu_rapport(dbb *db)
{
    int choix;

    do
    {
        system("cls");   // Commande Windows pour nettoyer l'écran
        printf("\n--- Menu recherche indexee\n");
        printf("\n");
        printf("1. Rapport Roi par zone et par date(papier)\n");
        printf("2. Rapport Roi par zone et par periode(papier)\n");
        printf("3. Rapport Empereur et ses Rois par date MAP (LDC)\n");
        printf("4. Rapport Empereur et carte colore MAP (LDC)\n");
        printf("\n");
        printf("0. Retour\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);
        getchar();  // Pour consommer le caractère de nouvelle ligne laissé par scanf
        // Traitement du choix de l'utilisateur
        switch (choix)
        {
        case 1:
            Creer_Rap_Roi_Periode(db);
            break;
        case 2:
            Creer_Rap_Marquiq_Periode(db);
            break;
        case 3:
            Creer_Map_date(db);
            break;
        case 4:
            Creer_Map_date2(db);
            break;
        case RETOUR2:
            printf("Retour au menu DB\n");
            break;
        default:
            printf("Choix invalide. Veuillez essayer à nouveau.\n");
            break;
        }

        // Pause avant d'afficher à nouveau le menu
        if (choix != 0)
        {
            printf("\nAppuyez sur Entree pour continuer...");
            getchar();  // Attend que l'utilisateur appuie sur Entrée
        }

    }
    while (choix != 0);    // Boucle jusqu'à ce que l'utilisateur choisisse de quitter
}

/**********************************************************************************
* Demo liste chainee
*
*       0.  Retour
*
*       1.  Afficher Pays et Duche dans les 2 sens a partir d un buffer
*       2.  Afficher Pays et Duche dans les 2 sens et TRIE a partir d un buffer
*       3.  Load Citoyen
*
*
* Valeur de retour : aucune
**********************************************************************************/
void menu_List_DC(dbb *db)
{
    int choix;

    do
    {
        system("cls");   // Commande Windows pour nettoyer l'écran
        printf("\n--- Menu Liste Doublement chainee\n");
        printf("\n");
        printf("1. Afficher Pays et Duche dans les 2 sens a partir d un buffer\n");
        printf("2. Afficher Pays et Duche dans les 2 sens et TRIE a partir d un buffer\n");
        printf("3. Load 20 Citoyens\n");
        printf("4. Load 20 Citoyen , suppression du 1er et dernier element\n");
        printf("\n");
        printf("0. Retour\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);
        getchar();  // Pour consommer le caractère de nouvelle ligne laissé par scanf
        // Traitement du choix de l'utilisateur
        switch (choix)
        {
        case 1:
            Load_Buffers_pays(db);
            Load_Buffers_duch(db);
            Test_LDC1(db);
            break;
        case 2:
            Load_Buffers_pays(db);
            Load_Buffers_duch(db);
            Test_LDC2(db);
            break;
        case 3:
            Test_LDC3(db);
            break;
        case 4:
            Test_LDC4(db);
            break;
        case RETOUR2:
            printf("Retour au menu DB\n");
            break;
        default:
            printf("Choix invalide. Veuillez essayer à nouveau.\n");
            break;
        }

        // Pause avant d'afficher à nouveau le menu
        if (choix != 0)
        {
            printf("\nAppuyez sur Entree pour continuer...");
            getchar();  // Attend que l'utilisateur appuie sur Entrée
        }

    }
    while (choix != 0);    // Boucle jusqu'à ce que l'utilisateur choisisse de quitter
}
