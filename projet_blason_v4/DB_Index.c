/****************************************************************************************
* Dossier Blasons : index
*
* creation des index
*
* Programmation procedurale 2024 - A.Umina - Version 0
****************************************************************************************/
#include "DB_Main.h"

/**********************************************************************************
* Fonction de creation des index générale
* parametre : - db : pointeur vers la base de donnees a indexer
**********************************************************************************/
void Creer_index(dbb *db)
{
    tab_tri_alloc(db, db->hdr.nbr_cit); // Alloue la table de tri
    Creer_index_nom(db);               // Crée l'index des noms
    tab_tri_free(db);

    tab_tri_alloc(db, db->hdr.nbr_cit); // Alloue la table de tri
    Creer_index_cit_id(db);               // Crée l'index des noms
    tab_tri_free(db);

    tab_tri_alloc(db, db->hdr.nbr_charge);
    Creer_index_id(db);
    tab_tri_free(db);

    uint off;
    tab_tri_alloc(db, db->hdr.nbr_charge);
    Creer_index_id_arbre(db);
    Update_DB_Header(db);
    tab_tri_free(db);
    Afficher_Index_Cha_arb(db);
    off=Rechercher_Charge_Arbre_Id(db,15500059);
    printf("off dans la table charges : %u\n",off) ;

}
/**********************************************************************************
* Fonction de creation des index par nom des citoyens
* parametre : - db : pointeur vers la base de donnees a indexer
**********************************************************************************/
void Creer_index_nom(dbb *db)
{
    int i;
    uint pt_cit = 0; // Offset du citoyen dans le fichier
    citoyen cit;    // Structure temporaire pour lire les citoyens

    printf("Creation de l index par nom\n");

    for (i = 0; i < db->hdr.nbr_cit; i++)
    {
        memset(&cit, 0, sizeof(citoyen));
        // Calcul de l'offset pour chaque citoyen
        pt_cit = db->hdr.off_cit + i * sizeof(citoyen);
        // Positionne le curseur dans le fichier à l'emplacement du citoyen
        if (fseek(db->fp_db, pt_cit, SEEK_SET) != 0)
        {
            fprintf(stderr, "Erreur : Impossible de positionner le curseur pour le citoyen %d.\n", i);
            continue; // Passe au citoyen suivant en cas d'erreur
        }
        // Lit le citoyen dans la structure temporaire
        if (fread(&cit, sizeof(citoyen), 1, db->fp_db) != 1)
        {
            fprintf(stderr, "Erreur : Impossible de lire les donnees pour le citoyen %d.\n", i);
            continue; // Passe au citoyen suivant en cas d'erreur
        }
        // Remplit l'entrée dans la table de tri
        db->tri[i].obj_offset = pt_cit;                 // Enregistre l'offset du citoyen
        strncpy(db->tri[i].nom, cit.nom_cit, 64 - 1);   // Copie le nom du citoyen
        db->tri[i].nom[63] = '\0';                      // Assure que la chaîne est terminée
    }
    // Tri de la table `tri` par ordre alphabétique des noms
    tri_rapide_nom(db, 0, db->hdr.nbr_cit - 1);
    Ecrire_Index_Cit(db);
    printf("Index des noms cree avec succes.\n\n");
}
/**********************************************************************************
* Fonction de creation des index par id des citoyens
* parametre : - db : pointeur vers la base de donnees a indexer
**********************************************************************************/
void Creer_index_cit_id(dbb *db)
{
    int i;
    uint pt_cit = 0; // Offset du citoyen dans le fichier
    citoyen cit;    // Structure temporaire pour lire les citoyens

    printf("Creation de l index par id du citoyen\n");

    for (i = 0; i < db->hdr.nbr_cit; i++)
    {
        memset(&cit, 0, sizeof(citoyen));
        // Calcul de l'offset pour chaque citoyen
        pt_cit = db->hdr.off_cit + i * sizeof(citoyen);
        // Positionne le curseur dans le fichier à l'emplacement du citoyen
        if (fseek(db->fp_db, pt_cit, SEEK_SET) != 0)
        {
            fprintf(stderr, "Erreur : Impossible de positionner le curseur pour le citoyen %d.\n", i);
            continue; // Passe au citoyen suivant en cas d'erreur
        }
        // Lit le citoyen dans la structure temporaire
        if (fread(&cit, sizeof(citoyen), 1, db->fp_db) != 1)
        {
            fprintf(stderr, "Erreur : Impossible de lire les donnees pour le citoyen %d.\n", i);
            continue; // Passe au citoyen suivant en cas d'erreur
        }
        // Remplit l'entrée dans la table de tri
        db->tri[i].obj_offset = pt_cit;
        db->tri[i].indice = cit.id_cit;
    }
    // Tri de la table `tri`
    tri_rapide_id(db, 0, db->hdr.nbr_cit - 1);
    Ecrire_Index_Cit_id(db);
    printf("Index des noms cree avec succes.\n\n");
}
/**********************************************************************************
* Fonction de creation des index par id des charges
* parametre : - db : pointeur vers la base de donnees a indexer
**********************************************************************************/
void Creer_index_id(dbb *db)
{
    int i;
    uint pt_id_cit = 0;  // Offset id du citoyen dans le fichier
    charge cha;          // Structure temporaire de charge

    printf("Creation de l index des charges par id\n");

    for (i = 0; i < db->hdr.nbr_charge; i++)
    {
        memset(&cha, 0, sizeof(charge));
        // Calcul de taille offset pour chaque charge
        pt_id_cit = db->hdr.off_charge + i * sizeof(charge);
        // Positionne le curseur dans le fichier à l'emplacement charge
        if (fseek(db->fp_db, pt_id_cit, SEEK_SET) != 0)
        {
            fprintf(stderr, "Erreur : Impossible de positionner le curseur pour la charge %d.\n", i);
            continue; // Passe au  suivant en cas d'erreur
        }
        // Lit charge dans la structure temporaire
        if (fread(&cha, sizeof(cha), 1, db->fp_db) != 1)
        {
            fprintf(stderr, "Erreur : Impossible de lire les donnees pour le charge %d.\n", i);
            continue; // Passe au  suivant en cas d'erreur
        }
        // Remplit l'entrée dans la table de tri
        db->tri[i].obj_offset = pt_id_cit;              // Enregistre l'offset de charge
        db->tri[i].indice = cha.id_indi;              // Copie id_cit

    }

    // Tri de la table `tri` par ordre alphabétique des noms
    tri_rapide_id(db, 0, db->hdr.nbr_charge - 1);
    Ecrire_Index_Id(db);
    printf("Index des charges cree avec succes.\n\n");
}
/**********************************************************************************
* Fonction de creation des index par id des charges type arbre
* parametre : - db : pointeur vers la base de donnees a indexer
**********************************************************************************/
void Creer_index_id_arbre(dbb *db)
{
    int i;
    uint pt_id_cit_arb = 0;  // Offset id du citoyen dans le fichier
    charge cha;              // Structure temporaire de charge

    printf("Creation de l index arbres des charges par id\n");

    for (i = 0; i < db->hdr.nbr_charge; i++)
    {
        memset(&cha, 0, sizeof(charge));
        // Calcul de taille offset pour chaque charge
        pt_id_cit_arb = db->hdr.off_charge + i * sizeof(charge);
        // Positionne le curseur dans le fichier à l'emplacement charge
        if (fseek(db->fp_db, pt_id_cit_arb, SEEK_SET) != 0)
        {
            fprintf(stderr, "Erreur : Impossible de positionner le curseur pour la charge %d.\n", i);
            continue; // Passe au  suivant en cas d'erreur
        }
        // Lit charge dans la structure temporaire
        if (fread(&cha, sizeof(cha), 1, db->fp_db) != 1)
        {
            fprintf(stderr, "Erreur : Impossible de lire les donnees pour le charge %d.\n", i);
            continue; // Passe au  suivant en cas d'erreur
        }

        // Remplit l'entrée dans la table de tri
        db->tri[i].obj_offset = pt_id_cit_arb;        // Enregistre l'offset de charge
        db->tri[i].indice = cha.id_indi;              // Copie id_cit
    }
    //printf("Index des charges cree avec succes.\n\n");
    tri_rapide_id(db, 0, db->hdr.nbr_charge - 1);
    Ecrire_Index_Id_Arbre(db);
    db->hdr.rac_ind_charge_arb = build_tree_charge(db, (db->hdr.nbr_charge-1)/2, db->hdr.nbr_charge);
    printf("Index arbre des charges cree avec succes.\n\n");
}
/****************************************************************************************
* Construction de l'arbre
* Paramètre :
*   - db : pointeur vers la base de données
*   - r : racine actuelle (index dans la table de tri +- le milieu)
*   - n : nombre total de nœuds
* Retourne :
*   - Offset de la racine dans la base de données
****************************************************************************************/
uint build_tree_charge(dbb *db, uint r, int n)
{
    int nbr_G, nbr_D;
    int nbr_tot = n - 1;
    uint cur, rg, rd;
    index_id_cit_arb in_ch;
    // Calcul du nombre de nœuds dans les sous-arbres gauche et droit
    nbr_G = nbr_tot / 2;
    nbr_D = nbr_tot - nbr_G;

    // Indices des racines des sous-arbres gauche et droit
    rg = r - nbr_G + (nbr_G - 1) / 2;
    rd = r + (nbr_D - 1) / 2 + 1;
    memset(&in_ch, 0, sizeof(index_id_cit_arb));
    // Calcule l'offset de la racine actuelle
    cur = db->hdr.off_ind_cha_arb + r * sizeof(index_id_cit_arb);

    // Lit la racine actuelle
    fseek(db->fp_db, cur, SEEK_SET);
    fread(&in_ch, sizeof(index_id_cit_arb), 1, db->fp_db);

    // Construction des sous-arbres gauche et droit si nécessaire
    if (nbr_G > 0)
        in_ch.ng_offset = build_tree_charge(db, rg, nbr_G); // Met à jour l'offset du nœud gauche
    if (nbr_D > 0)
        in_ch.nd_offset = build_tree_charge(db, rd, nbr_D); // Met à jour l'offset du nœud droit

    // Écrit les modifications dans la base de données
    fseek(db->fp_db, cur, SEEK_SET);
    fwrite(&in_ch, sizeof(index_id_cit_arb), 1, db->fp_db);

    return cur; // Retourne l'offset de la racine
}

/****************************************************************************************
* Écriture de l'index des citoyens dans la base de données
* Paramètre :
*   - db : pointeur vers la base de données
****************************************************************************************/
void Ecrire_Index_Cit(dbb *db)
{
    // Vérifie qu'il y a des citoyens dans la base
    if (db->hdr.nbr_cit == 0)
    {
        printf("Aucun citoyen a indexer.\n");
        return;
    }
    // Structure temporaire pour écrire l'index
    index_nom_cit idx;

    // Positionne le curseur à l'emplacement du bloc index des citoyens
    fseek(db->fp_db, db->hdr.off_ind_cit, SEEK_SET);

    printf("Ecriture de l index des citoyens dans la base de donnees...\n");

    for (int i = 0; i < db->hdr.nbr_cit; i++)
    {
        // Prépare l'entrée d'index
        memset(&idx, 0, sizeof(index_nom_cit));

        // Ajoute le type pour chaque champ
        strncpy(idx.tp_rec, "CI_IND", sizeof(idx.tp_rec) - 1); // Type de record
        idx.ptr_offset = db->tri[i].obj_offset;               // Offset du citoyen
        strncpy(idx.nom_cit, db->tri[i].nom, sizeof(idx.nom_cit) - 1); // Nom du citoyen
        idx.nom_cit[sizeof(idx.nom_cit) - 1] = '\0';          // Assure une terminaison
        memset(idx.fill, 0, sizeof(idx.fill));                // Remplissage pour alignement

        // Écrit l'index dans la base de données
        fwrite(&idx, sizeof(index_nom_cit), 1, db->fp_db);
    }

    printf("Index des citoyens écrit avec succès dans la base de données.\n\n");
}

/****************************************************************************************
* Écriture de l'index des id citoyens dans la base de données
* Paramètre :
*   - db : pointeur vers la base de données
****************************************************************************************/
void Ecrire_Index_Cit_id(dbb *db)
{
    // Vérifie qu'il y a des citoyens dans la base
    if (db->hdr.nbr_cit == 0)
    {
        printf("Aucun citoyen a indexer.\n");
        return;
    }
    // Structure temporaire pour écrire l'index
    index_id_cit idx;

    // Positionne le curseur à l'emplacement du bloc index des citoyens
    fseek(db->fp_db, db->hdr.off_ind_cit_id, SEEK_SET);

    printf("Ecriture de l index des id citoyens dans la base de donnees...\n");

    for (int i = 0; i < db->hdr.nbr_cit; i++)
    {
        // Prépare l'entrée d'index
        memset(&idx, 0, sizeof(index_id_cit));

        // Ajoute le type pour chaque champ
        strncpy(idx.tp_rec, "CIT_IN", sizeof(idx.tp_rec) - 1); // Type de record
        idx.ptr_offset = db->tri[i].obj_offset;               // Offset du citoyen

        idx.id_cit  =  db->tri[i].indice  ;// Assure une terminaison
        memset(idx.fill, 0, sizeof(idx.fill));                // Remplissage pour alignement

        // Écrit l'index dans la base de données
        fwrite(&idx, sizeof(index_id_cit), 1, db->fp_db);
    }

    printf("Index des id citoyens écrit avec succès dans la base de données.\n\n");
}
/****************************************************************************************
* Écriture de l'index de id citoyens des charges dans la base de données
* Paramètre :
*   - db : pointeur vers la base de données
****************************************************************************************/
void Ecrire_Index_Id(dbb *db)
{
    // Vérifie qu'il y a des charges dans la base
    if (db->hdr.nbr_charge == 0)
    {
        printf("Aucune charge a indexer.\n");
        return;
    }
    // Structure temporaire pour écrire l'index
    index_id_cit_cha idx;
    // Positionne le curseur à l'emplacement du bloc index des charges
    fseek(db->fp_db, db->hdr.off_ind_charge, SEEK_SET);

    printf("Ecriture de l index des charges dans la base de donnees...\n");

    for (int i = 0; i < db->hdr.nbr_charge; i++)
    {
         memset(&idx, 0, sizeof(index_id_cit_cha));
        // Ajoute le type pour chaque champ
        strncpy(idx.tp_rec, "CH_IND", sizeof(idx.tp_rec) - 1); // Type de record
        idx.ptr_offset = db->tri[i].obj_offset;                // Offset charge
        idx.id_cit = db->tri[i].indice;                        // id charge
        memset(idx.fill, 0, sizeof(idx.fill));                // Remplissage pour alignement
        // Écrit l'index dans la base de données
        fwrite(&idx, sizeof(index_id_cit_cha), 1, db->fp_db);
    }
    printf("Index des charges écrit avec succès dans la base de données.\n\n");
}
/****************************************************************************************
* Écriture de l'index de type arbre des charges dans la base de données
* Paramètre :
*   - db : pointeur vers la base de données
****************************************************************************************/
void Ecrire_Index_Id_Arbre(dbb *db){
    // Vérifie qu'il y a des charges dans la base
    if (db->hdr.nbr_charge == 0)
    {
        printf("Aucune charge à indexer.\n");
        return;
    }
    // Structure temporaire pour écrire l'index
    index_id_cit_arb idx;
    // Positionne le curseur à l'emplacement du bloc index des charges type arbre
    fseek(db->fp_db, db->hdr.off_ind_cha_arb, SEEK_SET);
    printf("Écriture de l'index de type arbre des charges dans la base de données...\n");

    for (int i = 0; i < db->hdr.nbr_charge; i++)
    {
        memset(&idx, 0, sizeof(index_id_cit_arb));
        // Remplit les champs de l'index
        strncpy(idx.tp_rec, "CH_ARB", sizeof(idx.tp_rec) - 1); // Type de record
        idx.tp_rec[sizeof(idx.tp_rec) - 1] = '\0';            // Assure la terminaison
        idx.id_pers_offset = db->tri[i].obj_offset;           // Offset de la charge
        idx.id_cit_arb = db->tri[i].indice;                   // ID citoyen
        idx.ng_offset = 0;
        idx.nd_offset = 0;
        memset(idx.fill, 0, sizeof(idx.fill));                // Alignement

        // Écrit l'index dans la base de données
        fwrite(&idx, sizeof(index_id_cit_arb), 1, db->fp_db);
    }

    printf("Index de type arbre des charges écrit avec succès dans la base de données.\n\n");
}



/**********************************************************************************
* Fonction de comparaison pour le tri des noms
* Paramètres :
*   - a : pointeur vers une entrée de la table d'index
*   - b : pointeur vers une autre entrée de la table d'index
* Retourne :
*   - Valeur < 0 si `a->nom` précède `b->nom`
*   - Valeur > 0 si `a->nom` suit `b->nom`
*   - 0 si `a->nom` est identique à `b->nom`
**********************************************************************************/
int Comparer_Nom(const void *a, const void *b)
{
    const tab_tri *entry1 = (const tab_tri *)a;
    const tab_tri *entry2 = (const tab_tri *)b;
    return strcmp(entry1->nom, entry2->nom);
}

/**********************************************************************************
* Fonction de retour d'un record par offset donné
* Paramètres :
*   - db  : pointeur vers la base de données
*   - pos : position de l'enregistrement souhaité (index)
* Retourne :
*   - La structure citoyen correspondant à l'offset
**********************************************************************************/
citoyen read_cit_off(dbb *db, int pos)
{
    citoyen cit;
    // Calcule l'offset exact et positionne le curseur dans le fichier
    //fseek(db->fp_db, db->hdr.off_cit + pos * sizeof(citoyen), SEEK_SET);
     fseek(db->fp_db, pos , SEEK_SET);
    // Lit la structure citoyen à partir du fichier
    if (fread(&cit, sizeof(citoyen), 1, db->fp_db) != 1) {
        printf("Erreur : Impossible de lire le citoyen à la position %d.\n", pos);
        memset(&cit, 0, sizeof(citoyen)); // Retourne une structure vide en cas d'erreur
    } else {
        // Affiche le nom du citoyen lu
        printf("Citoyen : %s retourné.\n", cit.nom_cit);
    }
    return cit;
}
/**********************************************************************************
* Fonction afficher les 10 premiers index pour tester
* Paramètres :
*   - db  : pointeur vers la base de données
* Retourne : rien
**********************************************************************************/
void Afficher_Index_Cit(dbb *db)
{
    index_nom_cit idx;
    printf("Verification des 10 premier index des citoyens :\n");
    fseek(db->fp_db, db->hdr.off_ind_cit, SEEK_SET);
    for (int i = 0; i < 10 ; i++) {
        fread(&idx, sizeof(index_nom_cit), 1, db->fp_db);
        printf("Nom: %s, Offset: %u\n", idx.nom_cit, idx.ptr_offset);
    }
}
void Afficher_Index_Cit_id(dbb *db)
{
    index_id_cit idx;
    printf("Verification des 10 premier index des ID citoyens :\n");
    fseek(db->fp_db, db->hdr.off_ind_cit_id, SEEK_SET);
    for (int i = 0; i < 10 ; i++) {
        fread(&idx, sizeof(index_id_cit), 1, db->fp_db);
        printf("ID: %d, Offset: %u\n", idx.id_cit , idx.ptr_offset);
    }
}
void Afficher_Index_Charge(dbb *db)
{
    index_id_cit_cha idx;
    printf("Verification des 10 premiers index des charges :\n");
    fseek(db->fp_db, db->hdr.off_ind_charge, SEEK_SET);
    for (int i = 0; i < 10 ; i++) {
        fread(&idx, sizeof(index_id_cit_cha), 1, db->fp_db);
        printf("ID_citoyens: %d, Offset: %u\n", idx.id_cit, idx.ptr_offset);
    }
}
void Afficher_Index_Cha_arb(dbb *db)
{
    index_id_cit_arb idx;
    printf("Verification des 20 premiers index des charges :\n");
    fseek(db->fp_db, db->hdr.rac_ind_charge_arb, SEEK_SET);
    for (int i = 0; i < 10 ; i++) {
        fread(&idx, sizeof(index_id_cit_arb), 1, db->fp_db);
        printf("ID_citoyens: %d, Offset: %u, noeud G: %u noeud D: %u\n", idx.id_cit_arb, idx.id_pers_offset,idx.ng_offset, idx.nd_offset);
    }
}
