/****************************************************************************************
* Dossier Blasons : Creation de la Database
*
* Creation et gestion de la Database
*
* Programmation procedurale 2024 - A.Umina - Version 0
****************************************************************************************/
#include "DB_Main.h"

/****************************************************************************************
* Creation de la database sur base des constantes NR_*
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Create_DB(dbb *db)
{
    int i;
    pays  pay;
    duch  duc;
    ville vil;
    citoyen cit;
    noblesse nob;
    charge cha;
    richesse rich;
    blason blaz;
    index_nom_cit ind_ct;
    index_id_cit ind_id_ct;
    index_id_cit_cha  ind_charge;
    index_id_cit_arb ind_cha_arb;
    memset(&db->hdr, 0, sizeof(hder));

    db->fp_db = fopen("DB_Data\\DB_Blasons.dat", "wb");
    db->fp_lg = fopen("DB_Data\\DB_Blasons.log", "a");

    // Creation du header ----------------------------

    strcpy(db->hdr.db_name, "DB_Blasons");

    db->hdr.sz_pay        = NBR_PAY;
    db->hdr.sz_duc        = NBR_DUC;
    db->hdr.sz_ville      = NBR_VILLE;
    db->hdr.sz_nob        = NBR_NOB;
    db->hdr.sz_charge     = NBR_CHARG;
    db->hdr.sz_cit        = NBR_CIT;
    db->hdr.sz_ind_cit    = NBR_CIT;
    db->hdr.sz_ind_id_cit = NBR_CIT;

    db->hdr.sz_ind_charge = NBR_CHARG;
    db->hdr.sz_ind_charge_arb = NBR_CHARG;
    db->hdr.sz_rich       = NBR_RICH;
    db->hdr.sz_blaz       = NBR_BLAZ;


    db->hdr.off_pay            = sizeof(hder); // 1er
    db->hdr.off_duc            = db->hdr.off_pay         + NBR_PAY   * sizeof(pays);
    db->hdr.off_ville          = db->hdr.off_duc         + NBR_DUC   * sizeof(duch);
    db->hdr.off_nob            = db->hdr.off_ville       + NBR_VILLE * sizeof(ville);
    db->hdr.off_charge         = db->hdr.off_nob         + NBR_NOB   * sizeof(noblesse);
    db->hdr.off_cit            = db->hdr.off_charge      + NBR_CHARG * sizeof(charge);
    db->hdr.off_ind_cit        = db->hdr.off_cit         + NBR_CIT   * sizeof(citoyen);
    db->hdr.off_ind_cit_id     = db->hdr.off_ind_cit     + NBR_CIT   * sizeof(index_nom_cit);
    db->hdr.off_ind_charge     = db->hdr.off_ind_cit_id  + NBR_CIT   * sizeof(index_id_cit);
    db->hdr.off_ind_cha_arb    = db->hdr.off_ind_charge  + NBR_CHARG * sizeof(index_id_cit_cha);
    db->hdr.off_rich           = db->hdr.off_ind_cha_arb + NBR_CHARG * sizeof(index_id_cit_arb);
    db->hdr.off_blaz           = db->hdr.off_rich        + NBR_RICH  * sizeof(richesse);
    db->hdr.db_size            = db->hdr.off_blaz        + NBR_BLAZ  * sizeof(blason);


    db->hdr.nbr_cit        = 0;
    db->hdr.nbr_nob        = 0;
    db->hdr.nbr_charge     = 0;
    db->hdr.nbr_pay        = 0;
    db->hdr.nbr_duc        = 0;
    db->hdr.nbr_ville      = 0;
    db->hdr.nbr_richesse   = 0;
    db->hdr.nbr_blaz       = 0;
    db->hdr.nbr_ind_cit    = 0;
    db->hdr.nbr_ind_cit_id = 0;
    db->hdr.nbr_ind_charge = 0;
    db->hdr.nbr_ind_cha_arb= 0;

    fwrite(&db->hdr, sizeof(db->hdr), 1, db->fp_db);


        // ---------------------------------------Creation de la table pays ----------------------------
    memset(&pay, 0, sizeof(pays));
    strcpy(pay.tp_rec, "PAYS");

    for (i=0; i<NBR_PAY; i++)
        fwrite(&pay, sizeof(pays), 1, db->fp_db);
    printf("Table Pays      : %-8d enregistrements vides.\n", NBR_PAY);

       //--------------------------------------- Creation de la table duches ----------------------------
    memset(&duc, 0, sizeof(duch));
    strcpy(duc.tp_rec, "DUCHE");

    for (i=0; i<NBR_DUC; i++)
        fwrite(&duc, sizeof(duch), 1, db->fp_db);
    printf("Table Duches    : %-8d enregistrements vides.\n", NBR_DUC);


    //------------------------------------------ Creation de la table villes ----------------------------
    memset(&vil, 0, sizeof(vil));
    strcpy(vil.tp_rec, "VILLE");

    for (i=0; i<NBR_VILLE; i++)
    {
         fwrite(&vil, sizeof(vil), 1, db->fp_db);
    }

    printf("Table Villes    : %-8d enregistrements vides.\n", NBR_VILLE);


        //-------------------------------------- Creation de la table noblesse ----------------------------
    memset(&nob, 0, sizeof(nob));
    strcpy(nob.tp_rec, "NOBLESSE");

    fseek(db->fp_db, db->hdr.off_nob, SEEK_SET);

    for (i = 0; i < NBR_NOB; i++)
    {
        fwrite(&nob, sizeof(nob), 1, db->fp_db);
    }

    printf("Table Noblesse  : %-8d enregistrements vides.\n", NBR_NOB);



    //------------------------------------ Creation de la table citoyens ----------------------------
    memset(&cit, 0, sizeof(cit)); // Initialise la structure `Cit` à zéro
    strcpy(cit.tp_rec, "CITOYEN");    // Définit le type d'enregistrement comme "CIT"

    fseek(db->fp_db, db->hdr.off_cit, SEEK_SET); // Place le curseur à l'emplacement des citoyens

    // Remplissage de la table des citoyens avec des enregistrements vides
    for (i = 0; i < NBR_CIT; i++)
    {
        fwrite(&cit, sizeof(cit), 1, db->fp_db); // Écrit la structure `Cit` vide dans la base de données
    }

    printf("Table Citoyens  : %-8d enregistrements vides.\n", NBR_CIT);



    //------------------------------------ Creation de l index la table citoyens ----------------------------
    memset(&ind_ct, 0, sizeof(ind_ct)); // Initialise la structure `Cit` à zéro
    strcpy(ind_ct.tp_rec, "INDEX_CITOYEN");

    fseek(db->fp_db, db->hdr.off_ind_cit, SEEK_SET); // Place le curseur à l'emplacement des citoyens

    // Remplissage de la table des citoyens avec des enregistrements vides
    for (i = 0; i < NBR_CIT; i++)
    {
        fwrite(&ind_ct, sizeof(ind_ct), 1, db->fp_db); // Écrit la structure `Cit` vide dans la base de données
    }

    printf("Index Citoyens  : %-8d enregistrements vides.\n", NBR_CIT);

    //------------------------------------ Creation de l index la table citoyens  id----------------------------
    memset(&ind_id_ct, 0, sizeof(ind_id_ct)); // Initialise la structure `Cit` à zéro
    strcpy(ind_id_ct.tp_rec, "INDEX_CIT_ID");

    fseek(db->fp_db, db->hdr.off_ind_cit_id, SEEK_SET); // Place le curseur à l'emplacement des citoyens

    // Remplissage de la table des citoyens avec des enregistrements vides
    for (i = 0; i < NBR_CIT; i++)
    {
        fwrite(&ind_id_ct, sizeof(ind_id_ct), 1, db->fp_db); // Écrit la structure `Cit` vide dans la base de données
    }

    printf("Index Citoyens id: %-8d enregistrements vides.\n", NBR_CIT);

    // ---------------------------------------Creation de la table charges ----------------------------
    memset(&cha, 0, sizeof(cha));
    strcpy(cha.tp_rec, "CHARGES");

    fseek(db->fp_db, db->hdr.off_charge, SEEK_SET);

    for (i = 0; i < NBR_CHARG; i++)
    {
        fwrite(&cha, sizeof(cha), 1, db->fp_db);
    }

    printf("Table Charges   : %-8d enregistrements vides.\n", NBR_CHARG);

    //------------------------------------ Creation de l index la table charges ----------------------------
    memset(&ind_charge, 0, sizeof(ind_charge)); // Initialise la structure `Cit` à zéro
    strcpy(ind_charge.tp_rec, "INDEX_CHARGES");

    fseek(db->fp_db, db->hdr.off_ind_charge, SEEK_SET); // Place le curseur

    // Remplissage de la table indices des charges avec des enregistrements vides
    for (i = 0; i < NBR_CHARG; i++)
    {
        fwrite(&ind_charge, sizeof(ind_charge), 1, db->fp_db); // Écrit la structure `Cit` vide dans la base de données
    }

    printf("Index charges   : %-8d enregistrements vides.\n", NBR_CHARG);


    //------------------------------------ Creation de l index la table charges  arbre ----------------------------
    memset(&ind_cha_arb, 0, sizeof(ind_cha_arb)); // Initialise la structure `Cit` à zéro
    strcpy(ind_cha_arb.tp_rec, "INDEX_CHA_ARB");

    fseek(db->fp_db, db->hdr.off_ind_cha_arb, SEEK_SET); // Place le curseur

    // Remplissage de la table indices des charges avec des enregistrements vides
    for (i = 0; i < NBR_CHARG; i++)
    {
        fwrite(&ind_cha_arb, sizeof(ind_cha_arb), 1, db->fp_db); // Écrit la structure `Cit` vide dans la base de données
    }

    printf("Index charges arbres : %-8d enregistrements vides.\n", NBR_CHARG);
    //------------------------------------------ Creation de la table richesses ----------------------------
    memset(&rich, 0, sizeof(richesse)); // Initialise la structure `richesse` à zéro
    strcpy(rich.tp_rec, "RICHESSE");    // Définit le type d'enregistrement comme "RICHESSE"
    fseek(db->fp_db, db->hdr.off_rich, SEEK_SET); // Place le curseur à l'emplacement des RICHESSE

    // Remplissage de la table des richesses avec des enregistrements vides
    for (i = 0; i < NBR_RICH; i++)
    {
        fwrite(&rich, sizeof(richesse), 1, db->fp_db); // Écrit la structure `RICHESSE` vide dans la base de données
    }
    printf("Table Richesses : %-8d enregistrements vides.\n", NBR_RICH);

    // -------------------------------------------------Creation de la table blason ----------------------------
    memset(&blaz, 0, sizeof(blaz)); // Initialise la structure `blason` à zéro
    fseek(db->fp_db, db->hdr.off_blaz, SEEK_SET); // Place le curseur à l'emplacement des blasons


    // Remplissage de la table des richesses avec des enregistrements vides
    for (i = 0; i < NBR_BLAZ; i++)
    {
        fwrite(&blaz, sizeof(blaz), 1, db->fp_db);     }
    printf("Table blason    : %-8d enregistrements vides.\n", NBR_BLAZ);


    fprintf(db->fp_lg, "Database %s created\n", db->hdr.db_name);
    fclose(db->fp_db);
    fclose(db->fp_lg);
    printf("Databse %s created \n", db->hdr.db_name);

    return;
}

/****************************************************************************************
* Ouverture de la database
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Open_DB(dbb *db)
{
    db->fp_db = fopen("DB_Data\\DB_Blasons.dat", "rb+");
    if (db->fp_db == NULL)
    {
        printf("Erreur d'ouverture du fichier DB_Blasons.dat\n");
        return;
    }
    db->status = 1;
    printf("Base de donnees imperiale V3 : DB_Blasons OPEN\n\n");

    db->fp_lg = fopen("DB_Data\\DB_Blasons.log", "a");
    if (db->fp_lg == NULL)
    {
        printf("Erreur d'ouverture du fichier DB_Blasons.log\n");
        return;
    }
    fread(&db->hdr, sizeof(hder), 1, db->fp_db);

}

/****************************************************************************************
* Fermeture de la database
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Close_DB(dbb *db)
{
    fclose(db->fp_db);
    fclose(db->fp_lg);
}

/****************************************************************************************
* Mise à jour du header de la base de données
* Paramètre :
*   - db : pointeur vers la base de données
****************************************************************************************/
void Update_DB_Header(dbb *db)
{
    // Positionne le curseur au début du fichier
    fseek(db->fp_db, 0, SEEK_SET);

    // Écrit le header mis à jour dans le fichier
    fwrite(&db->hdr, sizeof(db->hdr), 1, db->fp_db);

    printf("Header de la base de données mis à jour avec succès.\n");
}
