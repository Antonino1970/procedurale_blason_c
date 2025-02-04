/****************************************************************************************
* Dossier Blasons : Rapport
*
*
* Programmation procedurale 2024 - A.Umina - Version 3
****************************************************************************************/
#include "menu.h"
#include "DB_Main.h"
#include <time.h>
void Creer_Rap_Roi_Periode(dbb *db)
{
    int rang;
    int nbr;
    tab_record_Dyn(db,10);
    rang = Recuperer_Rois_par_Zone(db);
    tab_record_free(db);
    tab_record_Dyn(db,300);
    nbr = Recuperer_Id_Rois_Charge(db,rang);
    Recuperer_off_cit_roi(db, nbr);
    Remplir_data_TabRois(db, nbr);
    Remplir_nob_TabRois(db, nbr);
    Afficher_Rois_Pagination_Periode(db, nbr);
    tab_record_free(db);
}
void Creer_Rap_Marquiq_Periode(dbb *db)
{
    int rang;
    int nbr;
    tab_record_Dyn(db,300000);
    rang = Recuperer_Marquis_par_Zone(db);
    tab_record_free(db);
    tab_record_Dyn(db,300000);
    nbr = Recuperer_Id_Rois_Charge(db,rang);
    printf("Nombres de Marquis : %d\n",nbr);
    system("pause");
    Recuperer_off_cit_roi(db, nbr);
    Remplir_data_TabRois(db, nbr);
    Remplir_nob_TabRois(db, nbr);
    Afficher_Rois_Pagination_Periode(db, nbr);
    tab_record_free(db);
}

void Creer_Map_date(dbb *db)
{
    Gen_LDC *ldc_rec;
    ldc_rec = creeListe();
    int date = 2392;
    // Demander à l'utilisateur une date entre 2200 et 3306
    printf("Entrez une date comprise entre 2200 et 3306 : ");
    scanf("%d", &date);
    fflush(stdin); // Vider le tampon d'entrée
    if (date <= 2200 || date > 3305)
    {
        printf("Date invalide.\n");
        return;
    }
    Recuperer_Charge_date_ldc(db, date, ldc_rec);
    Recuperer_Off_Ldc_dico(db, ldc_rec);
    Recuperer_data_Cit_Ldc(db, ldc_rec);
    Recuperer_Nob_Ldc_seq(db, ldc_rec);
    //printDir(ldc_rec, Print_record);
    Init_Image_rap1(db, ldc_rec, date);

    supprimerListe(&ldc_rec);

    printf("Appuyez sur Entrée pour continuer...\n");
    getchar(); // Attendre une entrée de l'utilisateur avant de terminer
}
void Creer_Map_date2(dbb *db)
{
    Gen_LDC *ldc_rec;
    ldc_rec = creeListe();
    int date = 2392;
    // Demander à l'utilisateur une date entre 2200 et 3306
    printf("Entrez une date comprise entre 2200 et 3306 : ");
    scanf("%d", &date);
    fflush(stdin); // Vider le tampon d'entrée
    if (date <= 2200 || date > 3305)
    {
        printf("Date invalide.\n");
        return;
    }
    Recuperer_Charge_date_ldc(db, date, ldc_rec);
    Recuperer_Off_Ldc_dico(db, ldc_rec);
    Recuperer_data_Cit_Ldc(db, ldc_rec);
    Recuperer_Nob_Ldc_seq(db, ldc_rec);
    Recuperer_Duc_Ldc_seq(db, ldc_rec);
    Load_Buffers_duch(db);
    image *img = Colorer_Carte(db);
    Init_Image_rap2(db, ldc_rec, date, img);
    //printDir(ldc_rec, Print_record);

    supprimerListe(&ldc_rec);

    printf("Appuyez sur Entrée pour continuer...\n");
    getchar(); // Attendre une entrée de l'utilisateur avant de terminer
}
/******************************************************************************************
* Fonction : Afficher_Rois_Pagination_Periode
* Description :
*   Affiche les informations des rois (nom, ID de pays, début et fin de règne) par pages
*   de 30 éléments, mais uniquement pour les rois dont le règne chevauche la période
*   spécifiée par l'utilisateur.
*
* Paramètres :
*   - db (dbb *) :
*       Pointeur vers la base de données contenant les informations des rois.
*       La structure `dbb` doit contenir un tableau `roi` initialisé et des données valides.
*
*   - nbr_record (int) :
*       Nombre total de rois dans le tableau `roi`. Cela détermine le nombre d'enregistrements
*       potentiellement à afficher.
*
* Retour :
*   Cette fonction ne retourne pas de valeur (void).
*   Elle affiche directement les informations paginées dans la console.
******************************************************************************************/
void Afficher_Rois_Pagination_Periode(dbb *db, int nbr_record)
{
    if (db == NULL || db->reco == NULL)
    {
        fprintf(stderr, "Erreur : base de données ou tableau des rois non alloué.\n");
        return;
    }

    // Demander à l'utilisateur de saisir une période
    int debut_periode, fin_periode;
    printf("Entrez l'année de début de la période : ");
    scanf("%d", &debut_periode);
    printf("Entrez l'année de fin de la période : ");
    scanf("%d", &fin_periode);

    if (debut_periode > fin_periode)
    {
        fprintf(stderr, "Erreur : l'année de début doit être inférieure ou égale à l'année de fin.\n");
        return;
    }

    // Filtrer les rois dont le règne chevauche la période spécifiée
    int filtered_indices[nbr_record];
    int filtered_count = 0;

    for (int i = 0; i < nbr_record; i++)
    {
        if (db->reco[i].cha.yr_out >= debut_periode && db->reco[i].cha.yr_in <= fin_periode)
        {
            filtered_indices[filtered_count++] = i;
        }
    }

    if (filtered_count == 0)
    {
        printf("Aucun roi trouvé pour la période %d à %d.\n", debut_periode, fin_periode);
        return;
    }

    // Pagination
    int page = 0;
    int total_pages = (filtered_count + ROIS_PAR_PAGE - 1) / ROIS_PAR_PAGE; // Arrondi supérieur
    while (1)
    {
        system("cls"); // Efface l'écran (Windows).
        printf("Page %d / %d\n", page + 1, total_pages);
        printf("----------------------------------------------------------------------\n");
        printf("| %-4s | %-20s | %-20s | %-6s | %-6s |\n", "ID", "Nom du Roi", "Zone", "Debut ", "Fin");
        printf("----------------------------------------------------------------------\n");

        int start = page * ROIS_PAR_PAGE;
        int end = (start + ROIS_PAR_PAGE < filtered_count) ? start + ROIS_PAR_PAGE : filtered_count;

        for (int i = start; i < end; i++)
        {
            int idx = filtered_indices[i];
            printf("| %-4d | %-20s | %-20s | %-6d | %-6d |\n",
                   idx + 1,
                   db->reco[idx].cit.nom_cit,
                   db->reco[idx].nob.zone_nob,
                   db->reco[idx].cha.yr_in,
                   db->reco[idx].cha.yr_out);
        }

        printf("----------------------------------------------------------------------\n");
        printf("[P] Page précédente  |  [N] Page suivante  |  [Q] Quitter\n");
        char choix = _getch(); // Lit une touche sans appuyer sur "Entrée".
        if (choix == 'N' || choix == 'n')
        {
            if (page < total_pages - 1)
            {
                page++;
            }
        }
        else if (choix == 'P' || choix == 'p')
        {
            if (page > 0)
            {
                page--;
            }
        }
        else if (choix == 'Q' || choix == 'q')
        {
            break; // Quitte la boucle
        }
    }

    // Sauvegarde du rapport
    printf("Voulez-vous sauvegarder ce rapport ? (O/N) : ");
    char save_choice = _getch();
    if (save_choice == 'O' || save_choice == 'o')
    {
        // Récupérer la date actuelle
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char date_str[11];
        strftime(date_str, sizeof(date_str), "%Y-%m-%d", t); // Format AAAA-MM-JJ

        // Créer le dossier "rapport" s'il n'existe pas
        char folder_name[] = "rapport";
        _mkdir(folder_name); // Crée le dossier s'il n'existe pas

        // Nom du fichier
        char file_name[256];
        snprintf(file_name, sizeof(file_name), "%s/Roi_%s_de_%d_a_%d_%s.txt",
                 folder_name,
                 db->reco[filtered_indices[0]].nob.zone_nob,
                 debut_periode,
                 fin_periode,
                 date_str);

        // Sauvegarder dans le fichier
        FILE *file = fopen(file_name, "w");
        if (!file)
        {
            fprintf(stderr, "Erreur : impossible de créer le fichier %s.\n", file_name);
            return;
        }

        fprintf(file, "Rapport des rois pour la période %d à %d\n", debut_periode, fin_periode);
        fprintf(file, "----------------------------------------------------------------------\n");
        fprintf(file, "| %-4s | %-20s | %-20s | %-6s | %-6s |\n", "ID", "Nom du Roi", "Zone", "Debut ", "Fin");
        fprintf(file, "----------------------------------------------------------------------\n");

        for (int i = 0; i < filtered_count; i++)
        {
            int idx = filtered_indices[i];
            fprintf(file, "| %-4d | %-20s | %-20s | %-6d | %-6d |\n",
                    idx + 1,
                    db->reco[idx].cit.nom_cit,
                    db->reco[idx].nob.zone_nob,
                    db->reco[idx].cha.yr_in,
                    db->reco[idx].cha.yr_out);
        }

        fclose(file);
        printf("Rapport sauvegardé avec succès dans le fichier : %s\n", file_name);
    }
}


/****************************************************************************************
* Fonction de récupération des id ,year in et out  du citoyens
* Paramètres :
*   - db     : pointeur vers la base de données
*   - id_nob : noblesse recherché
* Retour :
*   - nombre de records trouvés
****************************************************************************************/
int Recuperer_Id_Charge(dbb *db, int id_nob)
{
    int result = 0;  // Compteur pour les résultats
    charge cha;
    if (!db->reco)
    {
        fprintf(stderr, "Erreur : mémoire non allouée \n");
        return 0;
    }
    // Parcourir la table des charges indexée
    for (int i = 0; i < db->hdr.nbr_charge; i++)
    {
        fseek(db->fp_db, db->hdr.off_charge + sizeof(charge) * i, SEEK_SET);
        memset(&cha, 0, sizeof(charge));

        if (fread(&cha, sizeof(charge), 1, db->fp_db) != 1)
        {
            fprintf(stderr, "Erreur : lecture de la charge %d échouée.\n", i);
            break; // Arrêtez en cas d'erreur de lecture
        }
        // Si l'id correspond, ajouter au tableau
        if (cha.id_nob == id_nob)
        {
            db->reco[result].cha = cha;
            result++;
        }
    }
    // Redimensionner le tableau pour contenir uniquement les résultats valides
    if (result > 0)
    {
        record *temp = realloc(db->reco, result * sizeof(record));
        if (!temp)
        {
            perror("Erreur de réallocation mémoire");
            free(db->reco); // Libérer la mémoire initiale
            db->reco = NULL;
            return 0; // Retourner 0 pour indiquer une erreur
        }
        db->reco = temp;
    }
    else
    {
        // Libérer la mémoire si aucun résultat n'est trouvé
        free(db->reco);
        db->reco = NULL;
    }

    //printf("Nombre trouvé : %d\n", result);
    return result;
}
/****************************************************************************************
* Fonction de récupération des id ,year in et out  du citoyens
* Paramètres :
*   - db     : pointeur vers la base de données
*   - id_nob : noblesse recherché
* Retour :
*   - nombre de records trouvés
****************************************************************************************/
int Recuperer_Id_Charge_empreur_date(dbb *db, int date)
{
    int result = 0;  // Compteur pour les résultats
    charge cha;
    if (!db->reco)
    {
        fprintf(stderr, "Erreur : mémoire non allouée \n");
        return 0;
    }
    fseek(db->fp_db, db->hdr.off_charge, SEEK_SET);
    memset(&cha, 0, sizeof(charge));
    while (fread(&cha, sizeof(charge), 1, db->fp_db) == 1)
    {
        // Vérifier les critères de correspondance
        if (cha.id_nob == 1 && cha.yr_in <= date && cha.yr_out >= date)
        {
            db->reco[result].cha = cha;  // Ajouter le résultat dans le tableau
            result++;
        }
    }
    // Si aucun résultat n'a été trouvé, libérer la mémoire
    if (result == 0)
    {
        fprintf(stderr, "Aucun enregistrement trouve pour la date spécifiee.\n");
        free(db->reco);
        db->reco = NULL;
    }
    printf("Nombre trouvé : %d\n", result);
    return result;
}
/****************************************************************************************
* Dessiner cartouche de l empreur dans l image
*   db : pointeur vers la base de données
*   cx, cy : coordonnées du centre de l'ellipse
*   largeur : largeur de l'ellipse (diamètre horizontal)
*   hauteur : hauteur de l'ellipse (diamètre vertical)
*   col : couleur de l'ellipse
****************************************************************************************/
void Dessiner_Emp_Cart(dbb *db,char tab1[32],char tab2[32], int in, int out, int date )
{
    Generer_Police();
    cart_emp emp;
    blason mask;
    blason bla1;
    // Créer l'image principale
    image *img = Creer_Image("cartouche", 1600, 1200, BLEU_6);
    if (img == NULL)
    {
        printf("Erreur lors de la création de l'image.\n");
        return;
    }

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "rapport Empreur et Roi an %d", date);
    // Écrire une chaîne de texte dans l'image
    Ecrire_Chaine(img, buffer, 2, 0, 50, 1150, NOIR, 10);

    // Initialiser les blasons et le masque, et les écrire dans l'image
    Lire_Blason_db(db, &bla1, 0);
    Lire_Mask(&mask, 0); // Charger le masque pour le blason
    Ecrire_Blason(&mask, "T2"); // Appliquer un texte dans le masque
    // Initialiser les cartouches avec les blasons, les couleurs de fond spécifiques, et un masque pour l'empereur
    Initialiser_Cartouche_emp(&emp, &bla1, &mask, Get_Col(ROUGE_5), Get_Col(ROUGE), tab1, tab2, in, out);
    Ecrire_Cartouche_emp(img, &emp, 800, 1100);
    // Définir les points d'ancrage pour dessiner les lignes de liaison entre les cartouches
    int pt1_x = 300;
    int pt1_y = 800;
    int pt2_x = emp.ancre_x;
    int pt2_y = emp.ancre_y;
    Dessiner_Ligne_Points(img, pt1_x, pt1_y, pt2_x, pt2_y, NOIR, 5);

    pt1_x = 500;
    pt1_y = 800;
    pt2_x = emp.ancre_x;
    pt2_y = emp.ancre_y;
    Dessiner_Ligne_Points(img, pt1_x, pt1_y, pt2_x, pt2_y, NOIR, 5);

    pt1_x = 700;
    pt1_y = 800;
    pt2_x = emp.ancre_x;
    pt2_y = emp.ancre_y;
    Dessiner_Ligne_Points(img, pt1_x, pt1_y, pt2_x, pt2_y, NOIR, 5);

    pt1_x = 900;
    pt1_y = 800;
    pt2_x = emp.ancre_x;
    pt2_y = emp.ancre_y;
    Dessiner_Ligne_Points(img, pt1_x, pt1_y, pt2_x, pt2_y, NOIR, 5);

    pt1_x = 1100;
    pt1_y = 800;
    pt2_x = emp.ancre_x;
    pt2_y = emp.ancre_y;
    Dessiner_Ligne_Points(img, pt1_x, pt1_y, pt2_x, pt2_y, NOIR, 5);

    pt1_x = 1300;
    pt1_y = 800;
    pt2_x = emp.ancre_x;
    pt2_y = emp.ancre_y;
    Dessiner_Ligne_Points(img, pt1_x, pt1_y, pt2_x, pt2_y, NOIR, 5);

    //Écrire à nouveau le cartouche empereur pour le positionnement
    Ecrire_Cartouche_emp(img, &emp, 800, 1100);

    // Sauvegarder l'image avec les cartouches insérés
    Ecrire_Image(img, "_cart_1");

    // Libérer la mémoire de l'image principale
    Free_Image(img);
}
/****************************************************************************************
* Dessiner cartouche de l empreur dans l image
*   db : pointeur vers la base de données
*   cx, cy : coordonnées du centre de l'ellipse
*   largeur : largeur de l'ellipse (diamètre horizontal)
*   hauteur : hauteur de l'ellipse (diamètre vertical)
*   col : couleur de l'ellipse
****************************************************************************************/
void Init_Image_rap1(dbb *db, Gen_LDC *liste,int date)
{
    Generer_Police();
    cart_emp emp;
    cart_roi roi;
    blason mask;
    blason bla1;
    blason bla2;
    // Créer l'image principale
    image *img = Creer_Image("Rapport_Graphique", 1600, 1200, BLEU_6);
    if (img == NULL)
    {
        printf("Erreur lors de la création de l'image.\n");
        return;
    }
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "rapport Empreur et Roi an %d", date);
    // Écrire une chaîne de texte dans l'image
    Ecrire_Chaine(img, buffer, 2, 0, 50, 1150, NOIR, 10);
    int i = 0;
    int j = 0 ;
    for (Gen_LDC *it = liste->suiv; it != liste; it = it->suiv)
    {
        record *reco = (record *)it->rec;

        if (i == 0)
        {
            // Initialiser les blasons et le masque, et les écrire dans l'image
            Lire_Blason_db(db, &bla1, 0);
            Lire_Mask(&mask, 0); // Charger le masque pour le blason
            Ecrire_Blason(&mask, "T2"); // Appliquer un texte dans le masque
            // Initialiser les cartouches avec les blasons, les couleurs de fond spécifiques, et un masque pour l'empereur
            Initialiser_Cartouche_emp(&emp, &bla1, &mask, Get_Col(ROUGE_5), Get_Col(ROUGE), reco->nob.zone_nob, reco->cit.nom_cit, reco->cha.yr_in, reco->cha.yr_out );
            Ecrire_Cartouche_emp(img, &emp, 800, 1100);
            // Dessiner les lignes d'ancrage
            for (int k = 0; k < 6; k++) {
            int pt1_x = 150 + 250 * k; // Positions horizontales équidistantes
            int pt1_y = 780;
            int pt2_x = emp.ancre_x;
            int pt2_y = emp.ancre_y;
            Dessiner_Ligne_Points(img, pt1_x, pt1_y, pt2_x, pt2_y, NOIR, 2);
            }

            //Écrire à nouveau le cartouche empereur pour le positionnement
            Ecrire_Cartouche_emp(img, &emp, 800, 1100);
            i++;
        }
        else
            {
            Lire_Blason_db(db, &bla2, i);
            Initialiser_Cartouche_roi(&roi, &bla2, &mask, Get_Col(VERT_5), Get_Col(VERT), reco->nob.zone_nob, reco->cit.nom_cit, reco->cha.yr_in, reco->cha.yr_out );
            Ecrire_Cartouche_roi(img, &roi, 150 + 250*j, 700);
            i++;
            j++;
            }
    }
    // Sauvegarder l'image avec les cartouches insérés
    Ecrire_Image_Rap_1(img, date);
    // Libérer la mémoire de l'image principale
    Free_Image(img);
}
/****************************************************************************************
* Dessiner cartouche de l empreur dans l image
*   db : pointeur vers la base de données
*   cx, cy : coordonnées du centre de l'ellipse
*   largeur : largeur de l'ellipse (diamètre horizontal)
*   hauteur : hauteur de l'ellipse (diamètre vertical)
*   col : couleur de l'ellipse
****************************************************************************************/
void Init_Image_rap2(dbb *db, Gen_LDC *liste,int date,image *carte)
{
    Generer_Police();
    cart_emp emp;
    cart_roi roi;
    blason mask;
    blason bla1;
    blason bla2;
    // Créer l'image principale
    image *img = Creer_Image("Rapport_Graphique", 1600, 1600, BLEU_6);
    if (img == NULL)
    {
        printf("Erreur lors de la création de l'image.\n");
        return;
    }
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "rapport Empreur et Roi an %d", date);
    // Écrire une chaîne de texte dans l'image
    Ecrire_Chaine(img, buffer, 2, 0, 50, 1150+400, NOIR, 10);
    int i = 0;
    int j = 0 ;
    for (Gen_LDC *it = liste->suiv; it != liste; it = it->suiv)
    {
        record *reco = (record *)it->rec;

        if (i == 0)
        {
            // Initialiser les blasons et le masque, et les écrire dans l'image
            Lire_Blason_db(db, &bla1, 0);
            Lire_Mask(&mask, 0); // Charger le masque pour le blason
            Ecrire_Blason(&mask, "T2"); // Appliquer un texte dans le masque
            // Initialiser les cartouches avec les blasons, les couleurs de fond spécifiques, et un masque pour l'empereur
            Initialiser_Cartouche_emp(&emp, &bla1, &mask, Get_Col(ROUGE_5), Get_Col(ROUGE), reco->nob.zone_nob, reco->cit.nom_cit, reco->cha.yr_in, reco->cha.yr_out );
            Ecrire_Cartouche_emp(img, &emp, 800, 1100+400);
            // Dessiner les lignes d'ancrage
            for (int k = 0; k < 6; k++) {
            int pt1_x = 150 + 250 * k; // Positions horizontales équidistantes
            int pt1_y = 780+400;
            int pt2_x = emp.ancre_x;
            int pt2_y = emp.ancre_y;
            Dessiner_Ligne_Points(img, pt1_x, pt1_y, pt2_x, pt2_y, NOIR, 2);
            }

            //Écrire à nouveau le cartouche empereur pour le positionnement
            Ecrire_Cartouche_emp(img, &emp, 800, 1100+400);
            i++;
        }
        else
            {
            Lire_Blason_db(db, &bla2, i);
            Initialiser_Cartouche_roi(&roi, &bla2, &mask, Get_Col(VERT_5), Get_Col(VERT), reco->nob.zone_nob, reco->cit.nom_cit, reco->cha.yr_in, reco->cha.yr_out );
            Ecrire_Cartouche_roi(img, &roi, 150 + 250*j, 700+400);
            i++;
            j++;
            }
    }
    Positionner_Image(img,carte,350,30);
    // Sauvegarder l'image avec les cartouches insérés
    Ecrire_Image_Rap_2(img, date);

    // Libérer la mémoire de l'image principale
    Free_Image(img);
}
/****************************************************************************************
* Fonction de récupération des enregistrements par id_nob (de 1 à 7) et année dans charges
* Paramètres :
*   - db     : pointeur vers la base de données
*   - date   : année de recherche
*   - ldc    : liste chaînée où les résultats seront ajoutés
* Retour :
*   - nombre total d'enregistrements trouvés
****************************************************************************************/
int Recuperer_Charge_date_ldc(dbb *db, int date, Gen_LDC *ldc)
{
    int result = 0;  // Compteur pour les résultats
    charge cha;
    record rec;
    // Vérification des arguments
    if (!ldc)
    {
        fprintf(stderr, "Erreur : la liste chainee n est pas initialisee.\n");
        return 0;
    }
    // Parcourir les `id_nob` de 1 à 7
    for (int id_nob = 1; id_nob <= 7; id_nob++)
    {
        fseek(db->fp_db, db->hdr.off_charge, SEEK_SET);
        while (fread(&cha, sizeof(charge), 1, db->fp_db) == 1)
        {
            if (cha.id_nob == id_nob && cha.yr_in <= date && cha.yr_out >= date)
            {
                memset(&rec, 0, sizeof(record));
                rec.cha = cha;
                ajouterEnQueue(ldc, &rec, sizeof(record));
                result++;
            }
        }
    }
    if (result == 0)
    {
        fprintf(stderr, "Aucun enregistrement trouvé pour la date spécifiée.\n");
    }
    else
    {
        printf("Nombre total d'enregistrements trouvés : %d\n", result);
    }

    return result;
}

/*****************************************************************************************
 * Associe chaque reco dans la liste LDC avec l'offset de son citoyen correspondant.
 *
 * Cette fonction effectue une recherche dichotomique directe dans le fichier DB
 * pour trouver l'offset d'un citoyen correspondant à l'ID de chaque reco.
 *
 * param db          Pointeur vers la structure de la base de données (dbb).
 * param liste       Pointeur vers la liste LDC contenant les records.
 ****************************************************************************************/
void Recuperer_Off_Ldc_dico(dbb *db, Gen_LDC *liste)
{
    if (liste->suiv == liste)
    {
        fprintf(stderr, "Erreur : liste vide.\n");
        return;
    }
    for (Gen_LDC *it = liste->suiv; it != liste; it = it->suiv)
    {
        record *reco = (record *)it->rec;
        if (reco->cha.id_indi == 0)
        {
            fprintf(stderr, "Erreur : ID citoyen nul pour un reco.\n");
            continue; // Passer au prochain reco
        }
        // Effectuer une recherche dichotomique dans le fichier pour trouver l'offset
        uint offset = Recherche_Dichotomique_DB(
                          db,                           // Pointeur vers la base de données
                          db->hdr.off_ind_cit_id,      // Offset de base pour la table indexée
                          db->hdr.nbr_cit,             // Nombre total de citoyens dans la table
                          reco->cha.id_indi            // ID du citoyen à rechercher
                      );
        // Vérifier si un offset valide a été trouvé
        if (offset != 0)
        {
            // Associer l'offset trouvé au reco
            reco->off_cit = offset;
        }
        else
        {
            // Afficher une erreur si le citoyen correspondant n'est pas trouvé
            fprintf(stderr, "Erreur : ID citoyen %d introuvable.\n", reco->cha.id_indi);
        }
    }
}
/****************************************************************************************
* Fonction de récupération des données des citoyens associés aux rois dans la liste LDC.
* Paramètres :
*   - db         : pointeur vers la base de données.
*   - liste      : pointeur vers la liste LDC contenant les records.
****************************************************************************************/
void Recuperer_data_Cit_Ldc(dbb *db, Gen_LDC *liste)
{
    if (liste->suiv == liste)
    {
        fprintf(stderr, "Erreur : liste vide.\n");
        return;
    }
    for (Gen_LDC *it = liste->suiv; it != liste; it = it->suiv)
    {
        record *reco = (record *)it->rec;
        if (reco->off_cit == 0)
        {
            fprintf(stderr, "Erreur : offset nul pour un reco.\n");
            continue; // Passer au prochain reco
        }
        citoyen cit;
        fseek(db->fp_db, reco->off_cit, SEEK_SET);
        if (fread(&cit, sizeof(citoyen), 1, db->fp_db) != 1)
        {
            fprintf(stderr, "Erreur : impossible de lire les donnees du citoyen a l offset %d.\n", reco->off_cit);
            continue;
        }

        // Remplir les données du reco avec les informations du citoyen
        reco->cit = cit;
    }
}
/*****************************************************************************************
 * Associe chaque reco dans noblesse correspondant à id_nob dans charge.
 *
 * Cette fonction effectue une recherche séquentielle dans le fichier DB
 * pour trouver l'offset d'un citoyen correspondant à l'ID de chaque reco.
 *
 * param db          Pointeur vers la structure de la base de données (dbb).
 * param liste       Pointeur vers la liste LDC contenant les records.
 ****************************************************************************************/
void Recuperer_Nob_Ldc_seq(dbb *db, Gen_LDC *liste)
{
    noblesse nob;
    if (liste->suiv == liste)
    {
        fprintf(stderr, "Erreur : liste vide.\n");
        return;
    }
    // on démarre la boucle à l'élément suivant du nœud "sentinelle tant que it n'est pas revenu au nœud "sentinelle"
    for (Gen_LDC *it = liste->suiv; it != liste; it = it->suiv)
    {
        record *reco = (record *)it->rec;

        if (reco->cha.id_nob == 0)
        {
            fprintf(stderr, "Erreur : ID noble nul pour un reco.\n");
            continue; // Passer au prochain reco
        }

        fseek(db->fp_db, db->hdr.off_nob, SEEK_SET);  // Aller au début de la table noblesse

        int found = 0;
        // Recherche séquentielle dans le fichier des charges
        while (fread(&nob, sizeof(noblesse), 1, db->fp_db) == 1)
        {
            if (nob.id_nob == reco->cha.id_nob)  // Si on trouve la noblesse correspondante
            {
                reco->nob = nob;  // Associer la charge au reco
                found = 1;
                break;
            }
        }
        if (!found)
        {
            fprintf(stderr, "Erreur : ID noble %d non trouvé dans les noblesses pour le reco.\n", reco->cha.id_nob);
        }
    }
}
/*****************************************************************************************
 * Associe chaque reco dans duché correspondant à id_duche dans charge.
 *
 * Cette fonction effectue une recherche séquentielle dans le fichier DB
 * pour trouver l'offset d'un duché correspondant à l'ID de chaque reco.
 *
 * param db          Pointeur vers la structure de la base de données (dbb).
 * param liste       Pointeur vers la liste LDC contenant les records.
 ****************************************************************************************/
void Recuperer_Duc_Ldc_seq(dbb *db, Gen_LDC *liste)
{
    duch duc;

    if (liste->suiv == liste)
    {
        fprintf(stderr, "Erreur : liste vide.\n");
        return;
    }

    // Boucle à partir de l'élément suivant du nœud sentinelle
    for (Gen_LDC *it = liste->suiv; it != liste; it = it->suiv)
    {
        record *reco = (record *)it->rec;

        if (reco->nob.id_pays == 0)
        {
            fprintf(stderr, "Erreur : ID duché nul pour un reco.\n");
            continue; // Passer au prochain reco
        }

        fseek(db->fp_db, db->hdr.off_duc, SEEK_SET);  // Aller au début de la table duché

        int found = 0;
        // Recherche séquentielle dans le fichier des duchés
        while (fread(&duc, sizeof(duch), 1, db->fp_db) == 1)
        {
            if (duc.id_pays == reco->nob.id_pays )  // Si on trouve le duché correspondant
            {
                reco->duc = duc;  // Associer le duché au reco
                found = 1;
                break;
            }
        }
        if (!found)
        {
            fprintf(stderr, "Erreur : ID duché %d non trouvé dans les duchés pour le reco.\n", reco->duc.id_duche);
        }
    }
}
/*****************************************************************************************
 * Fonction pour colorer la carte en fonction des données des duchés dans la base de données.
 *
 * Cette fonction charge une image de carte, modifie les couleurs des duchés en fonction
 * des ID de pays associés, puis retourne l'image modifiée.
 *
 * param db              Pointeur vers la base de données `dbb` contenant les informations
 *                       sur les duchés et pays.
 *
 * retour                Un pointeur vers la structure `image` représentant la carte colorée.
 *****************************************************************************************/
image* Colorer_Carte(dbb *db)
{
    Load_Buffers_duch(db);            // Charger les buffers des duchés
    image *carte = NULL;
    carte = Lire_Image("Carte_Europalia", ""); // Lire l'image source depuis un fichier

    Modifier_Couleurs_Pays(db, carte, 30);    // Modifier les couleurs des pays sur la carte
    Ecrire_Image(carte, "_1");                // Sauvegarder l'image modifiée

    return carte;                            // Retourner l'image modifiée (pointeur)
}

/*****************************************************************************************
 * Fonction pour modifier la couleur des comtés en fonction de l'ID du pays.
 *
 * Cette fonction parcourt la liste des duchés et pour chaque duché, elle appelle
 * la fonction Modifier_Comte pour changer la couleur du comté correspondant.
 * Chaque pays est associé à une couleur spécifique, et la fonction Modifier_Comte
 * modifie la couleur de chaque comté basé sur l'ID du pays.
 *
 * param carte          Pointeur vers la structure `image` représentant la carte à modifier.
 * param table_duc      Pointeur vers la table des duchés, contenant les informations des pays.
 * param taille_table   Taille de la table des duchés (ici, 30).
 ****************************************************************************************/
void Modifier_Couleurs_Pays(dbb *db, image *carte, int taille_table)
{
    int couleurs[] = {BLANC, BLEU, VERT, ROUGE, JAUNE, MAGENTA}; // Blanc, Bleu, Vert, Rouge, Jaune, Magenta
    int coul,duc;
    for (int i = 1; i <= 30; i++)
        {
            coul  = db->duc[i].id_pays - 1 ;
            duc = db->duc[i].id_duche;
            Modifier_Comte(carte, duc, couleurs[coul] );

    }
}

