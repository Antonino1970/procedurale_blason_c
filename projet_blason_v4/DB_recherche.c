/****************************************************************************************
* Dossier Blasons : recherche
*
* algorithme de recherche
*
* Programmation procedurale 2024 - A.Umina - Version 0
****************************************************************************************/
#include "DB_Main.h"
#include <conio.h>
void Recuperer_cit_roi_test(dbb *db, int nbr_record);
/****************************************************************************************
* allocation de zone de tri et desallocation
* parametre:- db pointeur vers la base de donnees dbb
*           - nbr nombre d enregistrements traite
****************************************************************************************/
void tab_tri_alloc(dbb *db, int nbr)
{
    db->tri = (tab_tri*)malloc(nbr*sizeof(tab_tri));
    memset(db->tri, 0, nbr*sizeof(tab_tri));
    return;
}
void tab_tri_free(dbb *db)
{
    free(db->tri);
    return;
}

void tab_record_Dyn(dbb *db, int nbr)
{
    db->reco = (record*)malloc(nbr*sizeof(record));
    memset(db->reco, 0, nbr*sizeof(record));
    return;
}
void tab_record_free(dbb *db)
{
    free(db->reco);
    return;
}
/****************************************************************************************
* tri bulle nom
* parametre:- db pointeur vers la base de donnees dbb
*           - nbr nombre d enregistrements traite
****************************************************************************************/
void tri_bulle_nom(dbb *db, int nbr)
{
    int plus_grant = 0;
    int non_triee  = 1;
    int i = 0;
    int j = 0;
    tab_tri tmp;
    while (non_triee)
    {
        non_triee= 0;
        i++;
        for (j=0; j<nbr-j; j++)
        {
            plus_grant = strcmp(db->tri[j].nom, db->tri[j+1].nom) > 0 ? 1 : 0;
            if (plus_grant)
            {
                tmp          = db->tri[j+1];
                db->tri[j+1] = db->tri[j];
                db->tri[j]   = tmp;
                non_triee    = 1;
            }
        }
    }
    return;
}
/****************************************************************************************
* Tri rapide des noms dans la table d'index
* Paramètres :
*   - db : pointeur vers la base de données (structure dbb)
*   - left : indice de début du sous-tableau
*   - right : indice de fin du sous-tableau
****************************************************************************************/
void tri_rapide_nom(dbb *db, int left, int right)
{
    if (left >= right)
    {
        // Cas de base : un seul élément ou sous-tableau vide
        return;
    }
    int i = left;
    int j = right;
    char pivot[64];        // Pivot pour comparer les noms
    tab_tri tmp;
    // Sélection du pivot : élément central
    strcpy(pivot, db->tri[(left + right) / 2].nom);
    // Partitionnement
    do
    {
        // Avancer `i` tant que les noms sont inférieurs au pivot
        while (strcmp(db->tri[i].nom, pivot) < 0)
        {
            i++;
        }
        // Reculer `j` tant que les noms sont supérieurs au pivot
        while (strcmp(db->tri[j].nom, pivot) > 0)
        {
            j--;
        }
        // Échanger les éléments mal positionnés
        if (i <= j)
        {
            tmp = db->tri[i];
            db->tri[i] = db->tri[j];
            db->tri[j] = tmp;
            i++;
            j--;
        }
    }
    while (i <= j);
    // Tri récursif des sous-tableaux gauche et droit
    if (left < j)
    {
        tri_rapide_nom(db, left, j);
    }
    if (i < right)
    {
        tri_rapide_nom(db, i, right);
    }
}


/****************************************************************************************
* Tri rapide des id dans la table d'index
* Paramètres :
*   - db : pointeur vers la base de données (structure dbb)
*   - left : indice de début du sous-tableau
*   - right : indice de fin du sous-tableau
****************************************************************************************/
void tri_rapide_id(dbb *db, int left, int right)
{
    if (left >= right)
    {
        return;
    }
    int i     = left;
    int j     = right;
    int pivot;        // Pivot pour comparer les id
    tab_tri tmp;
    // Sélection du pivot : élément central
    pivot = db->tri[(left + right) / 2].indice;
    // Partitionnement
    do
    {
        // Avancer `i` tant que les id sont inférieurs au pivot
        while (db->tri[i].indice < pivot)
        {
            i++;
        }
        // Reculer `j` tant que les id sont supérieurs au pivot
        while (db->tri[j].indice > pivot)
        {
            j--;
        }
        // Échanger les éléments mal positionnés
        if (i <= j)
        {
            tmp = db->tri[i];
            db->tri[i] = db->tri[j];
            db->tri[j] = tmp;
            i++;
            j--;
        }
    }
    while (i <= j);
    // Tri récursif des sous-tableaux gauche et droit
    if (left < j)
    {
        tri_rapide_id(db, left, j);
    }
    if (i < right)
    {
        tri_rapide_id(db, i, right);
    }
}

/****************************************************************************************
* Fonction de recherche dans l'index arbre des charges par id
* Paramètres :
*   - db : pointeur vers la base de données
*   - id : identifiant à rechercher
* Retourne :
*   - Offset de la charge dans la base de données si trouvée
*   - 0 si l'identifiant n'est pas trouvé
****************************************************************************************/
uint Rechercher_Charge_Arbre_Id(dbb *db, int id)
{
    uint cur_offset = db->hdr.rac_ind_charge_arb; // Début à la racine de l'arbre
    index_id_cit_arb noeud; // Structure temporaire pour lire les nœuds de l'arbre
    if (db == NULL || db->fp_db == NULL)
    {
        fprintf(stderr, "Erreur : Base de données ou fichier non initialisé.\n");
        return 0;
    }
    if (cur_offset >= db->hdr.db_size )
    {
        fprintf(stderr, "Erreur : Offset invalide %u (hors des limites de la base de données).\n", cur_offset);
        return 0;
    }
    while (cur_offset != 0)
    {
        // Lecture du nœud courant
        fseek(db->fp_db, cur_offset, SEEK_SET);
        if (fread(&noeud, sizeof(index_id_cit_arb), 1, db->fp_db) != 1)
        {
            fprintf(stderr, "Erreur : Impossible de lire le noeud à l offset %u.\n", cur_offset);
            return 0;
        }
        // Comparaison de l'id avec la valeur du nœud
        if (id == noeud.id_cit_arb)
        {
            //printf("Identifiant trouve : %d ,Offset  = %u , Offset gauche  = %u ,Offset droite  = %u\n", noeud.id_cit_arb,noeud.id_pers_offset, noeud.ng_offset,noeud.nd_offset);
            Afficher_Charge_Par_Offset(db, noeud.id_pers_offset);
            return noeud.id_pers_offset; // Identifiant trouvé, retourne l'offset de la charge
        }
        else if (id < noeud.id_cit_arb)
        {
            cur_offset = noeud.ng_offset; // Aller à gauche
        }
        else
        {
            cur_offset = noeud.nd_offset; // Aller à droite
        }
    }
    printf("Identifiant %d non trouve.\n", id);
    return 0;
}

/****************************************************************************************
* Fonction pour afficher une charge à partir de son offset
* Paramètres :
*   - db : pointeur vers la base de données
*   - off_charge : offset de la charge dans la base de données
* Retour :
*   - Aucun
****************************************************************************************/
void Afficher_Charge_Par_Offset(dbb *db, uint off_charge)
{
    if (db == NULL || db->fp_db == NULL)
    {
        fprintf(stderr, "Erreur : Base de donnees ou fichier non initialise.\n");
        return;
    }
    if (off_charge >= db->hdr.db_size || off_charge < 0)
    {
        fprintf(stderr, "Erreur : Offset invalide %u (hors des limites de la base de donnees).\n", off_charge);
        return;
    }
    struct Charge charge;
    fseek(db->fp_db, off_charge, SEEK_SET);
    if (fread(&charge, sizeof(struct Charge), 1, db->fp_db) != 1)
    {
        fprintf(stderr, "Erreur : Impossible de lire la charge a l offset %u.\n", off_charge);
        return;
    }
    printf("Details de la charge :\n");
    printf("ID Noble      : %d\n", charge.id_nob);
    printf("ID Individu   : %d\n", charge.id_indi);
    printf("Annee debut   : %d\n", charge.yr_in);
    printf("Annee fin     : %d\n", charge.yr_out);
}
/****************************************************************************************
* Fonction de récupération du rang des rois
* Paramètres :
*   - db : pointeur vers la base de données
*
* Retour :
*   - rang du citoyen
****************************************************************************************/
int Recuperer_Rois_Rang(dbb *db)
{
    int rang = 0 ;
    // Parcourir la table des noblesse pour trouver un roi
    for (int i = 0; i < db->hdr.nbr_nob; i++)
    {
        fseek(db->fp_db,db->hdr.off_nob + sizeof(noblesse) * i, SEEK_SET);
        noblesse nob;
        memset(&nob, 0, sizeof(noblesse));
        fread(&nob, sizeof(noblesse), 1, db->fp_db);
        if (strcmp(nob.titre_nob, "Roi") == 0)
        {
            rang = nob.rang_nob;  // On récupère le rang du premier roi trouvé
            return rang;
        }
    }
    return rang;
}

/****************************************************************************************
* Fonction de récupération des id ,year in et out  du citoyens rois par zone dans l index
* Paramètres :
*   - db     : pointeur vers la base de données
*   - id_nob : noblesse recherché
* Retour :
*   - nombre de records trouvés
****************************************************************************************/
int Recuperer_Id_Rois_Charge(dbb *db, int id_nob)
{
    int result = 0;  // Compteur pour les résultats
    charge cha;

    if (!db->reco)
    {
        fprintf(stderr, "Erreur : mémoire non allouée pour db->marquis.\n");
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
* Fonction de récupération des id ,year in et out  du citoyens marquis par zone dans l index
* Paramètres :
*   - db     : pointeur vers la base de données
*   - id_nob : noblesse recherché
* Retour :
*   - nombre de records trouvés
****************************************************************************************/
int Recuperer_Id_Marquis_Charge(dbb *db, int id_nob)
{
    int result = 0;  // Compteur pour les résultats
    charge cha;

    if (!db->reco)
    {
        fprintf(stderr, "Erreur : mémoire non allouée pour db->reco.\n");
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
* Fonction de récupération  des zones,id blason des rois dans citoyen
* Paramètres :
*   - db : pointeur vers la base de données
*   - max_result : Taille maximale du tableau roi_result
* Retour :
*   - rang du citoyen
****************************************************************************************/
void Recuperer_cit_roi_test(dbb *db, int nbr_record)
{
    if (db->reco == NULL)
    {
        fprintf(stderr, "Erreur : tableau reco non alloué.\n");
        return;
    }
    for (int i = 0; i < nbr_record; i++)
    {
        if (db->reco[i].cha.id_indi == 0)
        {
            fprintf(stderr, "Erreur : ID citoyen nul pour reco indexé %d\n", i);
            continue;
        }
        // Parcourir les enregistrements de la table indexée id citoyens
        for (int j = 0; j < db->hdr.nbr_cit; j++)
        {
            fseek(db->fp_db, db->hdr.off_ind_cit_id + sizeof(index_id_cit) * j, SEEK_SET);
            index_id_cit cit;
            memset(&cit, 0, sizeof(index_id_cit));
            fread(&cit, sizeof(index_id_cit), 1, db->fp_db);

            // Vérifier si l'ID citoyen correspond
            if (cit.id_cit == db->reco[i].cha.id_indi)
            {
                db->reco[i].off_cit = cit.ptr_offset;
                //printf("Roi ID %d : offset : %d\n", db->reco[i].id_cit, db->reco[i].off_cit);
                break;  // Une fois trouvé, sortir de la boucle
            }
        }
    }
}
/*****************************************************************************************
 * Associe chaque reco de la base de données avec l'offset de son citoyen correspondant.
 *
 * Cette fonction effectue une recherche dichotomique directe dans le fichier DB
 * pour trouver l'offset d'un citoyen correspondant à l'ID de chaque reco.
 *
 * param db          Pointeur vers la structure de la base de données (dbb).
 * param nbr_record  Nombre total de rois enregistrés dans la table des rois.
 ****************************************************************************************/
void Recuperer_off_cit_roi(dbb *db, int nbr_record)
{
    // Vérifier si le tableau des rois est alloué
    if (db->reco == NULL)
    {
        fprintf(stderr, "Erreur : tableau reco non allou.\n");
        return;
    }
    // Parcourir tous les enregistrements des rois
    for (int i = 0; i < nbr_record; i++)
    {
        // Vérifier si l'ID du citoyen est valide
        if (db->reco[i].cha.id_indi == 0)
        {
            fprintf(stderr, "Erreur : ID citoyen nul pour le reco indexé %d.\n", i);
            continue; // Passer au prochain reco
        }

        // Effectuer une recherche dichotomique dans le fichier pour trouver l'offset
        uint offset = Recherche_Dichotomique_DB(
                          db,                            // Pointeur vers la base de données
                          db->hdr.off_ind_cit_id,       // Offset de base pour la table indexée
                          db->hdr.nbr_cit,              // Nombre total de citoyens dans la table
                          db->reco[i].cha.id_indi             // ID du citoyen à rechercher
                      );
        // Vérifier si un offset valide a été trouvé
        if (offset != 0)
        {
            // Associer l'offset trouvé au reco
            db->reco[i].off_cit = offset;
        }
        else
        {
            // Afficher une erreur si le citoyen correspondant n'est pas trouvé
            fprintf(stderr, "Erreur : ID citoyen %d introuvable.\n", db->reco[i].cha.id_indi);
        }
    }
}
/*****************************************************************************************
 * Associe chaque reco de la base de données avec l'offset de son citoyen correspondant.
 *
 * Cette fonction effectue une recherche dichotomique directe dans le fichier DB
 * pour trouver l'offset d'un citoyen correspondant à l'ID de chaque reco.
 *
 * param db          Pointeur vers la structure de la base de données (dbb).
 * param nbr_record  Nombre total de rois enregistrés dans la table des rois.
 ****************************************************************************************/
citoyen Recuperer_data_cit_roi(dbb *db, int pos)
{
    citoyen cit;
    // positionne le curseur dans le fichier
    fseek(db->fp_db, pos, SEEK_SET);
    // Lit la structure citoyen à partir du fichier
    if (fread(&cit, sizeof(citoyen), 1, db->fp_db) != 1)
    {
        printf("Erreur : Impossible de lire le citoyen à la position %d.\n", pos);
        memset(&cit, 0, sizeof(citoyen)); // Retourne une structure vide en cas d'erreur
    }
    else
    {
        // Affiche le nom du citoyen lu
        printf("Citoyen : %s retourné.\n", cit.nom_cit);
    }
    return cit;
}
/*****************************************************************************************
 * Récupère les données d'un citoyen à partir de son offset dans le fichier DB.
 *
 * param db          Pointeur vers la structure de la base de données (dbb).
 * param offset      Position du citoyen dans le fichier (offset en octets).
 *
 * return citoyen    Structure citoyen récupérée à partir du fichier.
 ****************************************************************************************/
citoyen Recuperer_Citoyen_Par_Offset(dbb *db, int offset)
{
    citoyen cit;

    // Positionne le curseur dans le fichier DB à l'offset spécifié
    fseek(db->fp_db, offset, SEEK_SET);

    // Lit la structure citoyen depuis le fichier
    if (fread(&cit, sizeof(citoyen), 1, db->fp_db) != 1)
    {
        printf("Erreur : Impossible de lire le citoyen à l'offset %d.\n", offset);
        memset(&cit, 0, sizeof(citoyen)); // Retourne une structure vide en cas d'erreur
    }
//    else
//    {
//        // Affiche le nom du citoyen lu (peut être retiré si non nécessaire)
//        printf("Citoyen récupéré : %s\n", cit.nom_cit);
//    }

    return cit;
}

/****************************************************************************************
* Fonction de récupération des données des citoyens associés aux rois.
* Paramètres :
*   - db         : pointeur vers la base de données.
*   - nbr_record : nombre total de rois dans le tableau reco.
****************************************************************************************/
void Remplir_data_TabRois(dbb *db, int nbr_record)
{
    if (db->reco == NULL)
    {
        fprintf(stderr, "Erreur : tableau reco non alloué.\n");
        return;
    }
    for (int i = 0; i < nbr_record; i++)
    {
        // Vérifier si un offset valide est présent
        if (db->reco[i].off_cit == 0)
        {
            fprintf(stderr, "Erreur : offset nul pour le reco indexé %d.\n", i);
            continue;
        }
        // Récupérer les données du citoyen à partir de l'offset
        citoyen cit;
        fseek(db->fp_db, db->reco[i].off_cit, SEEK_SET);
        if (fread(&cit, sizeof(citoyen), 1, db->fp_db) != 1)
        {
            fprintf(stderr, "Erreur : impossible de lire les données du citoyen à l'offset %d.\n", db->reco[i].off_cit);
            continue;
        }

        // Remplir les données du reco à partir des informations du citoyen
        db->reco[i].cit = cit ;
    }
}
/****************************************************************************************
* Fonction de récupération des données de noblesse associés aux rois.
* Paramètres :
*   - db         : pointeur vers la base de données.
*   - nbr_record : nombre total de rois dans le tableau reco.
****************************************************************************************/
void Remplir_nob_TabRois(dbb *db, int nbr_record)
{
    if (db->reco == NULL)
    {
        fprintf(stderr, "Erreur : tableau reco non alloue.\n");
        return;
    }
    for (int i = 0; i < nbr_record; i++)
    {
        int count;
        noblesse nob;
        for (int j = 0; j <db->hdr.sz_nob; j++)
        {
            fseek(db->fp_db, db->hdr.off_nob+ sizeof(noblesse) * j, SEEK_SET);
            memset(&nob, 0, sizeof(noblesse));
            if (fread(&nob, sizeof(noblesse), 1, db->fp_db) != 1)
            {
                fprintf(stderr, "Erreur : impossible de lire les donnees de noblesse %d.\n", db->reco[i].cit.id_pays);
                continue;
            }
            if (nob.id_pays == db->reco[i].cit.id_pays && (strcmp(nob.titre_nob, "Roi") == 0))
            {
                // Remplir les données du reco à partir des informations de noblesse
                db->reco[i].nob = nob ;
            }
            count++;
        }

    }
}
/******************************************************************************************
* Fonction : Afficher_Rois_Pagination
* Description :
*   Affiche les informations des rois (nom, ID de pays, début et fin de règne) par pages
*   de 30 éléments. Permet à l'utilisateur de naviguer entre les pages ou de quitter.
*
* Paramètres :
*   - db (dbb *) :
*       Pointeur vers la base de données contenant les informations des rois.
*       La structure `dbb` doit contenir un tableau `reco` initialisé et des données valides.
*
*   - nbr_record (int) :
*       Nombre total de rois dans le tableau `reco`. Cela détermine le nombre d'enregistrements
*       à afficher et donc le nombre de pages.
*
* Retour :
*   Cette fonction ne retourne pas de valeur (void).
*   Elle affiche directement les informations paginées dans la console.
******************************************************************************************/
void Afficher_Rois_Pagination(dbb *db, int nbr_record)
{
    if (db == NULL || db->reco == NULL)
    {
        fprintf(stderr, "Erreur : base de donnees ou tableau des rois non alloue.\n");
        return;
    }
    int page = 0;
    int total_pages = (nbr_record + ROIS_PAR_PAGE - 1) / ROIS_PAR_PAGE; // Arrondi supérieur
    while (1)
    {
        system("cls"); // Efface l'écran (Windows).
        printf("Page %d / %d\n", page + 1, total_pages);
        printf("----------------------------------------------------------------------\n");
        printf("| %-4s | %-20s | %-20s | %-6s | %-6s |\n", "ID", "Nom du Roi", "Zone", "Debut ", "Fin");
        printf("----------------------------------------------------------------------\n");
        int start = page * ROIS_PAR_PAGE;
        int end = (start + ROIS_PAR_PAGE < nbr_record) ? start + ROIS_PAR_PAGE : nbr_record;
        for (int i = start; i < end; i++)
        {
            printf("| %-4d | %-20s | %-20s | %-6d | %-6d |\n",
                   i + 1,
                   db->reco[i].cit.nom_cit,
                   db->reco[i].nob.zone_nob,
                   db->reco[i].cha.yr_in,
                   db->reco[i].cha.yr_out);
        }
        printf("----------------------------------------------------------------------\n");
        printf("[P] Page precedente  |  [N] Page suivante  |  [Q] Quitter\n");
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
}
/*****************************************************************************************
 * Effectue une recherche dichotomique directement dans un fichier DB trié.
 *
 * param db            Pointeur vers la structure de la base de données (dbb).
 *       offset_base   Offset de base dans le fichier où commence la table.
 *       taille        Nombre total d'éléments dans la table.
 *       id_recherche  Identifiant à rechercher dans la table.
 *
 * return int          Index de l'élément trouvé dans la table, ou -1 si l'élément n'existe pas.
 *                      - Si trouvé, l'index est relatif à l'ordre dans la table.
 *                      - Si non trouvé, retourne -1.
 ****************************************************************************************/
int Recherche_Dichotomique_DB(dbb *db, uint offset_base, uint taille, int id_recherche)
{
    uint gauche = 0;           // Initialiser l'index de gauche à 0
    uint droite = taille - 1;  // Initialiser l'index de droite à la taille de la table moins 1

    index_id_cit buffer;      // Variable temporaire pour stocker un élément lu depuis le fichier
    // Boucle jusqu'à ce que la plage de recherche soit réduite
    while (gauche <= droite)
    {
        // Calculer l'index du milieu pour la recherche dichotomique
        int milieu = gauche + (droite - gauche) / 2;
        // Calculer l'offset dans le fichier pour atteindre l'élément du milieu
        uint offset = offset_base + milieu * sizeof(index_id_cit);
        // Se déplacer à l'offset correspondant dans le fichier
        fseek(db->fp_db, offset, SEEK_SET);
        // Lire l'entrée au milieu depuis le fichier
        fread(&buffer, sizeof(index_id_cit), 1, db->fp_db);
        // Comparer l'ID de la table avec l'ID recherché
        if (buffer.id_cit == id_recherche)
        {
            return buffer.ptr_offset; // Si trouvé, retourner l'index
        }
        else if (buffer.id_cit < id_recherche)
        {
            // Si l'ID au milieu est inférieur à l'ID recherché, déplacer l'index gauche
            gauche = milieu + 1;
        }
        else
        {
            // Sinon, déplacer l'index droit
            droite = milieu - 1;
        }
    }
    return -1; // Retourner -1 si l'élément n'est pas trouvé
}

/*****************************************************************************************
 * Effectue une recherche dichotomique insensible à la casse sur les noms des citoyens
 * dans un fichier DB trié.
 *
 * param db            Pointeur vers la structure de la base de données (dbb).
 *       offset_base   Offset de base dans le fichier où commence la table indexée.
 *       taille        Nombre total d'éléments dans la table.
 *       nom_recherche Chaîne de caractères contenant le nom à rechercher.
 *
 * return int          Offset de la structure  trouvée, ou -1 si le nom n'existe pas.
 ****************************************************************************************/
int Recherche_Dichotomique_Nom(dbb *db, uint offset_base, uint taille, const char *prefix)
{
    uint gauche = 0;           // Initialiser l'index de gauche à 0
    uint droite = taille - 1;  // Initialiser l'index de droite à la taille de la table moins 1
    index_nom_cit buffer;      // Variable temporaire pour stocker un élément lu depuis le fichier
    int longueur_prefix = strlen(prefix);
    // Boucle jusqu'à ce que la plage de recherche soit réduite
    while (gauche <= droite)
    {
        // Calculer l'index du milieu pour la recherche dichotomique
        uint milieu = gauche + (droite - gauche) / 2;
        // Calculer l'offset dans le fichier pour atteindre l'élément du milieu
        uint offset = offset_base + milieu * sizeof(index_nom_cit);
        // Se déplacer à l'offset correspondant dans le fichier
        fseek(db->fp_db, offset, SEEK_SET);
        // Lire l'entrée au milieu depuis le fichier
        memset(&buffer, 0, sizeof(index_nom_cit));
        fread(&buffer, sizeof(index_nom_cit), 1, db->fp_db);
        // Comparer le nom dans la table avec le nom recherché (insensible à la casse)
        //int comparaison = strncasecmp(buffer.nom_cit, nom_recherche, sizeof(buffer.nom_cit));
        int comparaison = strncasecmp(buffer.nom_cit, prefix, longueur_prefix);
        if (comparaison == 0)
        {
            return offset; // Si trouvé, retourner l'offset
        }
        else if (comparaison < 0)
        {
            // Si le nom au milieu est avant le nom recherché, déplacer l'index gauche
            gauche = milieu + 1;
        }
        else
        {
            // Sinon, déplacer l'index droit
            droite = milieu - 1;
        }
    }
    return -1; // Retourner -1 si le nom n'est pas trouvé
}

/*****************************************************************************************
 * Recherche tous les citoyens dans la base de données dont le nom commence par un préfixe
 * donné, en utilisant une recherche dichotomique pour optimiser la localisation initiale.
 *
 * param db            Pointeur vers la structure de la base de données (dbb).
 *       prefix        Chaîne de caractères contenant le début du nom à rechercher.
 *
 * return void         Affiche tous les noms trouvés correspondant au préfixe.
 ****************************************************************************************/
void Recherche_Noms_Commencant_Par(dbb *db, const char *prefix)
{
    index_nom_cit buffer;                  // Structure temporaire pour les noms
    int longueur_prefix = strlen(prefix);  // Longueur du préfixe
    int count;
    // Étape 1 : Localiser une entrée correspondant au préfixe
    int index_initial = Recherche_Dichotomique_Nom(db, db->hdr.off_ind_cit, db->hdr.nbr_cit, prefix);
    if (index_initial == -1)
    {
        printf("Aucun nom correspondant trouvé pour le préfixe : %s\n", prefix);
        return;
    }
    // Étape 2 : Parcourir les entrées précédentes correspondant au préfixe
    int i = index_initial ;
    int j = 0;

    while (i >= 0)
    {
        int offset = index_initial + j * sizeof(index_nom_cit);
        fseek(db->fp_db, offset, SEEK_SET);
        memset(&buffer, 0, sizeof(index_nom_cit));
        fread(&buffer, sizeof(index_nom_cit), 1, db->fp_db);
        // Vérifier si le préfixe correspond
        if (strncasecmp(buffer.nom_cit, prefix, longueur_prefix) != 0)
            break;
        // Récupérer et afficher les informations du citoyen
        citoyen cit = Recuperer_Citoyen_Par_Offset(db, buffer.ptr_offset);
        printf("Nom : %s, Prénom : %s\n", cit.nom_cit, cit.pre_cit);

        i--; // Passer à l'entrée précédente
        j--;
        count++;
    }
    //system("pause");

// Étape 3 : Parcourir les entrées suivantes correspondant au préfixe
    i = 0;
    j = 1; // Commence après l'index initial
    while (1) // Boucle infinie, à limiter avec un break
    {
        int offset = index_initial + j * sizeof(index_nom_cit);
        memset(&buffer, 0, sizeof(index_nom_cit));

        // Lire l'entrée correspondante
        fseek(db->fp_db, offset, SEEK_SET);
        if (fread(&buffer, sizeof(index_nom_cit), 1, db->fp_db) != 1)
        {
            // Fin du fichier ou erreur de lecture
            break;
        }

        // Vérifier si le préfixe correspond
        if (strncasecmp(buffer.nom_cit, prefix, longueur_prefix) != 0)
        {
            break; // Sort de la boucle si le préfixe ne correspond plus
        }

        // Récupérer et afficher les informations du citoyen
        citoyen cit = Recuperer_Citoyen_Par_Offset(db, buffer.ptr_offset);
        printf("Nom : %s, Prénom : %s\n", cit.nom_cit, cit.pre_cit);
        //system("pause");

        i++; // Passer à l'entrée suivante
        j++;
        count++;
    }
            printf("Total : %d\n", count);
            system("pause");
}

    /*****************************************************************************************
     * Fonction de recherche par préfixe pour afficher les citoyens trouvés.
     *
     * param db  Pointeur vers la structure de la base de données (dbb).
     ****************************************************************************************/
    void Recherche_Par_Prefix(dbb *db)
    {
        char prefix[64]; // Préfixe de recherche

        printf("Entrez le début du nom à rechercher (insensible à la casse) : ");
        scanf("%63s", prefix); // Limite la saisie à 31 caractères pour éviter les débordements
        Recherche_Noms_Commencant_Par(db, prefix);
    }

    /****************************************************************************************
    * Fonction de récupération du rang, des zones, des rois dans noblesse
    * Paramètres :
    *   - db : pointeur vers la base de données
    *   - max_result : Taille maximale du tableau roi_result
    * Retour :
    *   - rang du citoyen choisi par l utilisateur
    ****************************************************************************************/
    int Recuperer_Rois_par_Zone(dbb *db)
    {
        int rang = 0;
        int choix = -1;
        // En-tête de tableau pour un affichage aligné
        printf("----------------------------------------------------------------------\n");
        printf("| %-5s | %-10s | %-15s | %-5s |\n", "ID", "Titre", "Zone", "Rang");
        printf("----------------------------------------------------------------------\n");
        // Stocker temporairement les `id_nob` pour permettre le choix
        int ids_nob[db->hdr.nbr_nob];
        int count = 0;
        // Parcourir la table des noblesse pour trouver les rois
        for (int i = 0; i < db->hdr.nbr_nob; i++)
        {
            fseek(db->fp_db, db->hdr.off_nob + sizeof(noblesse) * i, SEEK_SET);
            noblesse nob;
            memset(&nob, 0, sizeof(noblesse));
            fread(&nob, sizeof(noblesse), 1, db->fp_db);
            // Si le titre est "Roi", l'ajouter au tableau des rois
            if (strcmp(nob.titre_nob, "Roi") == 0)
            {
                if (rang == 0)
                {
                    rang = nob.rang_nob; // On récupère le rang du premier reco trouvé
                }
                // Ajouter le reco au tableau et afficher
                db->reco[count].nob = nob;
                ids_nob[count] = nob.id_nob;
                printf("| %-5d | %-10s | %-15s | %-5d |\n",
                       nob.id_nob, nob.titre_nob, nob.zone_nob, nob.rang_nob);
                count++;
            }
        }
        if (count == 0)
        {
            printf("Aucun roi trouve.\n");
            return -1; // Indique qu'aucun roi n'a été trouvé
        }
        printf("----------------------------------------------------------------------\n");
        // Demander à l'utilisateur de choisir un ID parmi ceux affichés
        printf("Entrez l ID d un roi parmi ceux listes : ");
        while (1)
        {
            scanf("%d", &choix);

            // Vérifier si l'ID choisi est dans la liste des ID trouvés
            for (int i = 0; i < count; i++)
            {
                if (choix == ids_nob[i])
                {
                    printf("Vous avez choisi le roi avec l ID : %d\n", choix);
                    return choix; // Retourner l'ID sélectionné
                }
            }
            // Si l'ID n'est pas valide, demander de réessayer
            printf("ID invalide. Veuillez entrer un ID valide parmi ceux affiches : ");
        }
    }
/****************************************************************************************
    * Fonction de récupération du rang, des zones, des marquis dans noblesse
    * Paramètres :
    *   - db : pointeur vers la base de données
    *   - max_result : Taille maximale du tableau roi_result
    * Retour :
    *   - rang du citoyen choisi par l utilisateur
    ****************************************************************************************/
    int Recuperer_Marquis_par_Zone(dbb *db)
    {
        int rang = 0;
        int choix = -1;
        // En-tête de tableau pour un affichage aligné
        printf("----------------------------------------------------------------------\n");
        printf("| %-5s | %-10s | %-15s | %-5s |\n", "ID", "Titre", "Zone", "Rang");
        printf("----------------------------------------------------------------------\n");
        // Stocker temporairement les `id_nob` pour permettre le choix
        int ids_nob[db->hdr.nbr_nob];
        int count = 0;
        // Parcourir la table des noblesse pour trouver les rois
        for (int i = 0; i < db->hdr.nbr_nob; i++)
        {
            fseek(db->fp_db, db->hdr.off_nob + sizeof(noblesse) * i, SEEK_SET);
            noblesse nob;
            memset(&nob, 0, sizeof(noblesse));
            fread(&nob, sizeof(noblesse), 1, db->fp_db);
            // Si le titre est "Marquis", l'ajouter au tableau des rois
            if (strcmp(nob.titre_nob, "Marquis") == 0)
            {
                if (rang == 0)
                {
                    rang = nob.rang_nob; // On récupère le rang du premier reco trouvé
                }
                // Ajouter le reco au tableau et afficher
                db->reco[count].nob = nob;
                ids_nob[count] = nob.id_nob;
                printf("| %-5d | %-10s | %-15s | %-5d |\n",
                       nob.id_nob, nob.titre_nob, nob.zone_nob, nob.rang_nob);
                count++;
            }
        }
        if (count == 0)
        {
            printf("Aucun marquis trouve.\n");
            return -1; // Indique qu'aucun roi n'a été trouvé
        }
        printf("----------------------------------------------------------------------\n");
        // Demander à l'utilisateur de choisir un ID parmi ceux affichés
        printf("Entrez l ID d un marquis parmi ceux listes : ");
        while (1)
        {
            scanf("%d", &choix);

            // Vérifier si l'ID choisi est dans la liste des ID trouvés
            for (int i = 0; i < count; i++)
            {
                if (choix == ids_nob[i])
                {
                    printf("Vous avez choisi les marquis avec l ID : %d\n", choix);
                    return choix; // Retourner l'ID sélectionné
                }
            }
            // Si l'ID n'est pas valide, demander de réessayer
            printf("ID invalide. Veuillez entrer un ID valide parmi ceux affiches : ");
        }
    }

    /****************************************************************************************
    * Fonction pour afficher les rois par pays choisi pa l utilisateur
    * Paramètres :
    *   - db : pointeur vers la base de données
    * Retour :
    *   - Aucun
    ****************************************************************************************/
    void Recherche_roi_pays(dbb *db)
    {
        int rang;
        int nbr;
        tab_record_Dyn(db,10);
        rang = Recuperer_Rois_par_Zone(db);
        tab_record_free(db);
        tab_record_Dyn(db,300);
        nbr = Recuperer_Id_Rois_Charge(db,rang);
        printf("Nombres de ROI : %d\n",nbr);
        system("pause");
        Recuperer_off_cit_roi(db, nbr);
        Remplir_data_TabRois(db, nbr);
        Remplir_nob_TabRois(db, nbr);
        Afficher_Rois_Pagination(db, nbr);
        tab_record_free(db);
    }
    /****************************************************************************************
    * Fonction pour  Rechercher charge par id dans arbre indexe
    * Paramètres :
    *   - db : pointeur vers la base de données
    * Retour :
    *   - Aucun
    ****************************************************************************************/
    void Recherche_Id_Cha_Arb(dbb *db)
    {
            uint id;
            printf("Entrez l identifiant a rechercher (ex 15500059) : ");
            if (scanf("%u", &id) != 1)
            {
                printf("Erreur : Veuillez entrer un entier valide.\n");
                getchar();  // Consomme l'entrée incorrecte

            }
            Rechercher_Charge_Arbre_Id(db,id);
            printf("off dans la table charges : %u\n",id) ;
            getchar();// Consomme le caractère de nouvelle ligne restant dans le buffer après scanf

            if (id != 0)
            {
                printf("Recherche reussie : Offset charge = %u\n", id);
            }
    }
    /****************************************************************************************
    * Fonction pour Recherche dichotomique de l offset du citoyens par id
    * Paramètres :
    *   - db : pointeur vers la base de données
    * Retour :
    *   - Aucun
    ****************************************************************************************/
    void Recherche_Off_Cit_Par_Id(dbb *db)
    {
            uint id;
            int nbr;
            printf("Entrez l identifiant a rechercher (ex 10000010) : ");
            if (scanf("%u", &id) != 1)
            {
                printf("Erreur : Veuillez entrer un entier valide.\n");
                getchar();  // Consomme l'entrée incorrecte

            }
            nbr = Recherche_Dichotomique_DB(db, db->hdr.off_ind_cit_id, db->hdr.nbr_cit, id);
            printf("off : %d\n",nbr);
            Rechercher_Charge_Arbre_Id(db,id);
            printf("off dans la table charges : %u\n",id) ;
            getchar();// Consomme le caractère de nouvelle ligne restant dans le buffer après scanf

            if (id != 0)
            {
                printf("Recherche reussie : Offset charge = %u\n", id);
            }
    }
    /****************************************************************************************
    * Fonction pour Recherche du citoyens par offset
    * Paramètres :
    *   - db : pointeur vers la base de données
    * Retour :
    *   - Aucun
    ****************************************************************************************/
    void Recherche_Cit_Par_off(dbb *db)
    {
            uint id;

            printf("Entrez l identifiant a rechercher (ex 10000010) : ");
            if (scanf("%u", &id) != 1)
            {
                printf("Erreur : Veuillez entrer un entier valide.\n");
                getchar();  // Consomme l'entrée incorrecte

            }
            read_cit_off(db, id);


    }
/****************************************************************************************
* Fonction de récupération des données de noblesse associés aux rois.
* Paramètres :
*   - db         : pointeur vers la base de données.
*   - nbr_record : nombre total de rois dans le tableau reco.
****************************************************************************************/
void Remplir_nob_TabEmp(dbb *db, int nbr_record)
{
    if (db->reco == NULL)
    {
        fprintf(stderr, "Erreur : tableau non alloué.\n");
        return;
    }
    noblesse nob;
    fseek(db->fp_db, db->hdr.off_nob, SEEK_SET);
    memset(&nob, 0, sizeof(noblesse));
            if (fread(&nob, sizeof(noblesse), 1, db->fp_db) != 1)
            {
                fprintf(stderr, "Erreur : impossible de lire les donnees de noblesse \n");
            }
                // Remplir les données du reco à partir des informations de noblesse
                db->reco[0].nob = nob ;
}
