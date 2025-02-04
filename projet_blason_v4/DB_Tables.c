/****************************************************************************************
* Dossier Blasons : Creation de la Database
*
* Manipulations des tables Blasons, Charges, Citoyens, Duches,
* Noblesses, Pays, Richesses,Villes
* Programmation procedurale 2024 - A.Umina - Version 0
****************************************************************************************/
#include "DB_Main.h"
/****************************************************************************************
* Chargement du fichier original DB_Charges.csv dans la database
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Import_Charges(dbb *db)
{
    int i = 0; // Compteur
    char buf[BUF_LEN], fld[BUF_LEN]; // Buffer ligne et champ
    charge cha;
    FILE *fpi; // Pointeur vers CSV

    fpi = fopen("DB_Import\\DB_Charges.csv", "r");
        // Vérification de l'ouverture du fichier
    if (fpi == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier DB_Charges.csv\n");
        fprintf(db->fp_lg, "Erreur : ouverture du fichier DB_Charges.csv échouée\n");
        return;
    }

    printf("\nCharges : importing ... ");

    fgets(buf, 200, fpi);   // Suprimme entete
    fseek(db->fp_db, db->hdr.off_charge, SEEK_SET);

    while (fgets(buf, 200, fpi) != NULL)
    {
        memset(&cha, 0, sizeof(cha));
        strcpy(cha.tp_rec, "CHARGES");

        strcpy(fld, strtok(buf, ";")); // ID noblesse
        cha.id_nob = atoi(fld);

        strcpy(fld, strtok(NULL, ";")); // ID indice
        cha.id_indi = atoi(fld);

        strcpy(fld, strtok(NULL, ";")); // year in
        cha.yr_in = atoi(fld);

        strcpy(fld, strtok(NULL, ";")); // year out
        fld[strcspn(fld, "\n")] = 0;
        cha.yr_out = atoi(fld);


        fwrite(&cha, sizeof(cha), 1, db->fp_db);
        i++;
    }

    db->hdr.nbr_charge = i;

    printf("Done : %d", db->hdr.nbr_charge);

	fclose(fpi);

    fprintf(db->fp_lg, "Charges imported\n");

    // Réécriture de l'en-tête de la base de données pour enregistrer le nombre de villes
    fseek(db->fp_db, 0, SEEK_SET);  // Repositionner le curseur au début du fichier de la base
    fwrite(&db->hdr, sizeof(hder), 1, db->fp_db);  // Réécrire l'en-tête mis à jour

	return ;
}

/****************************************************************************************
* Generation d'un backup de la table Charges au format .csv
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Export_Charges(dbb *db)
{
    int i=0;
    charge cha;
	FILE *fpi;

    fpi = fopen("DB_Export\\DB_Charges.csv", "w");
        if (fpi == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier DB_Charges.csv\n");
        fprintf(db->fp_lg, "Erreur : ouverture du fichier DB_Charges.csv échouée\n");
        return;
    }

    printf("\nCharges  : exporting ... ");

    fprintf(fpi, "Id_Nobl;Id_Indi;Yr_In;Yr_Out\n");

    fseek(db->fp_db, db->hdr.off_charge, SEEK_SET);

    for (i=1; i<=(db->hdr.nbr_charge); i++)
    {
        memset(&cha, 0, sizeof(cha));
        fread(&cha, sizeof(cha), 1, db->fp_db);

        fprintf(fpi, "%d;%d;%d;%d\n", cha.id_nob,cha.id_indi,cha.yr_in,cha.yr_out);
    }

    printf("Done : %d", db->hdr.nbr_charge);

	fclose(fpi);

    fprintf(db->fp_lg, "Charges exported\n");

	return ;
}

/****************************************************************************************
* Chargement du fichier blason dans la database
*   db : pointeur de base de données blasons
****************************************************************************************/
void Import_Blason(dbb *db)
{
    int i = 0; // Compteur
    blason blaz;
    FILE *fpi; // Pointeur vers le fichier source de blasons

    fpi = fopen("DB_Import\\DB_Blason_image.dat", "rb");
    if (fpi == NULL) {
        printf("ERREUR : impossible d ouvrir le fichier DB_Blason_image.dat\n");
        fprintf(db->fp_lg, "ERREUR : ouverture du fichier DB_Blason_image.dat\n");
        return;
    }

    printf("\nBlason:   importing ... ");
    // Positionner le curseur à l'endroit où les blasons doivent être écrits dans la base
    fseek(db->fp_db, db->hdr.off_blaz, SEEK_SET);
    memset(&blaz, 0, sizeof(blaz));
    // Lire chaque blason dans le fichier source et l'écrire dans la base de données
    while (fread(&blaz, sizeof(blaz), 1, fpi) == 1)
    {
        fwrite(&blaz, sizeof(blaz), 1, db->fp_db);
        i++;
        memset(&blaz, 0, sizeof(blaz));
    }
    db->hdr.nbr_blaz = i;

    printf("Done : %d blasons\n", db->hdr.nbr_blaz);
    // Fermer le fichier source
    fclose(fpi);
    // Journaliser l'importation dans le fichier de log
    fprintf(db->fp_lg, "%d blasons imported\n", db->hdr.nbr_blaz);
    // Réécriture de l'en-tête de la base de données pour enregistrer le nombre de blasons
    fseek(db->fp_db, 0, SEEK_SET);  // Repositionner le curseur au début du fichier de la base
    fwrite(&db->hdr, sizeof(hder), 1, db->fp_db);  // Réécrire l'en-tête mis à jour

    return;
}


/****************************************************************************************
* Exportation des blasons de la base de données vers un fichier pour vérification
*   db : pointeur de base de données blasons
****************************************************************************************/
void Export_Blason(dbb *db)
{
    int i;
    blason blaz;
    FILE *fpo; // Pointeur vers le fichier de sortie

    // Ouvrir le fichier de sortie en mode binaire
    fpo = fopen("DB_Export\\Export_Blason.dat", "wb");
    if (fpo == NULL) {
        printf("Erreur : impossible de créer le fichier Export_Blason.dat\n");
        fprintf(db->fp_lg, "Erreur : création du fichier Export_Blason.dat échouée\n");
        return;  // Quitter la fonction si l'ouverture échoue
    }
    printf("\nBlason   : exporting ... ");
    // Positionner le curseur au début de la section des blasons dans la base
    fseek(db->fp_db, db->hdr.off_blaz, SEEK_SET);
    for (i = 0; i < db->hdr.nbr_blaz; i++)
    {
        memset(&blaz, 0, sizeof(blaz));
        // Lire un blason depuis la base de données
        if (fread(&blaz, sizeof(blason), 1, db->fp_db) != 1)
        {
            printf("Erreur : lecture du blason %d échouée\n", i);
            fprintf(db->fp_lg, "Erreur : lecture du blason %d échouée\n", i);
            break;
        }
        // Écrire le blason dans le fichier de sortie
        if (fwrite(&blaz, sizeof(blason), 1, fpo) != 1)
        {
            printf("Erreur : écriture du blason %d échouée\n", i);
            fprintf(db->fp_lg, "Erreur : écriture du blason %d échouée\n", i);
            break;
        }
    }
    printf("Done : %d", db->hdr.nbr_blaz);

    fclose(fpo);

    fprintf(db->fp_lg, "Blason exported\n");

    return ;
}

/****************************************************************************************
* Chargement du fichier original DB_Citoyens.csv dans la database
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Import_Citoyens(dbb *db)
{
    int i = 0; // Compteur
    char buf[BUF_LEN], fld[BUF_LEN]; // Buffer ligne et champ
    citoyen c;  // Structure Cit
    FILE *fpi; // Pointeur vers CSV

    fpi = fopen("DB_Import\\DB_Citoyens.csv", "r");
        // Vérification de l'ouverture du fichier
    if (fpi == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier DB_Cit.csv\n");
        fprintf(db->fp_lg, "Erreur : ouverture du fichier DB_Cit.csv\n");
        return;
    }

    printf("\nCitoyens: importing ... ");

    fgets(buf, 200, fpi);   // Suprimme entete

    fseek(db->fp_db, db->hdr.off_cit, SEEK_SET);

    while (fgets(buf, 200, fpi) != NULL)
    {
        memset(&c, 0, sizeof(c));
        strcpy(c.tp_rec, "CITOYENS");

                // Extraction des champs
        strcpy(fld, strtok(buf, ";")); // ID Citoyen
        c.id_cit = atoi(fld);

        strcpy(fld, strtok(NULL, ";")); // ID Pays
        c.id_pays = atoi(fld);

        strcpy(fld, strtok(NULL, ";")); // ID Duché
        c.id_duche = atoi(fld);

        strcpy(fld, strtok(NULL, ";")); // Nom Ville
        strncpy(c.nom_ville, fld, strlen(fld));

        strcpy(fld, strtok(NULL, ";")); // Sexe
        c.sex = fld[0];

        strcpy(fld, strtok(NULL, ";")); // Nom Citoyen
        strncpy(c.nom_cit, fld, strlen(fld));

        strcpy(fld, strtok(NULL, ";")); // Prénom Citoyen
        strncpy(c.pre_cit, fld, strlen(fld));

        strcpy(fld, strtok(NULL, ";")); // Fonction Citoyen
        strncpy(c.fct_cit, fld, strlen(fld));

        // Extraction de la date de naissance au format xx/xx/xxxx
        strcpy(fld, strtok(NULL, ";")); // Date de naissance au format xx/xx/xxxx
        strncpy(c.date_cit, fld, strlen(fld)-1);

        fwrite(&c, sizeof(citoyen), 1, db->fp_db);
        i++; // Incrémente le compteur
    }

    db->hdr.nbr_cit = i;

    printf("Done : %d", db->hdr.nbr_cit);

	fclose(fpi);

    fprintf(db->fp_lg, "Citoyens imported\n");

    // Réécriture de l'en-tête de la base de données pour enregistrer le nombre de villes
    fseek(db->fp_db, 0, SEEK_SET);  // Repositionner le curseur au début du fichier de la base
    fwrite(&db->hdr, sizeof(hder), 1, db->fp_db);  // Réécrire l'en-tête mis à jour

	return ;
}

/****************************************************************************************
* Generation d'un backup de la table ville au format .csv
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Export_Citoyens(dbb *db)
{
    int i=0;
    citoyen c;
	FILE *fpi;

    fpi = fopen("DB_Export\\DB_Citoyens.csv", "w");

    printf("\nCitoyens : exporting ... ");

    fprintf(fpi, "Id;Id_Pays;Id_Duc;ville;Sex;Nom;Prenom;Fonction;Dt_Nais\n");

    fseek(db->fp_db, db->hdr.off_cit, SEEK_SET);

    for (i=1; i<=(db->hdr.nbr_cit); i++)
    {
        memset(&c, 0, sizeof(c));
        fread(&c, sizeof(c), 1, db->fp_db);

        fprintf(fpi, "%d;%d;%d;%s;%c;%s;%s;%s;%s\n", c.id_cit,c.id_pays,c.id_duche,c.nom_ville,c.sex,c.nom_cit,c.pre_cit,c.fct_cit,c.date_cit);
    }

    printf("Done : %d", db->hdr.nbr_cit);

	fclose(fpi);

    fprintf(db->fp_lg, "Citoyens exported\n");

	return ;
}
/****************************************************************************************
* Chargement du fichier original DB_Duche.csv dans la database
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Import_Duches(dbb *db)
{
    int i=0;
    char buf[BUF_LEN], fld[BUF_LEN];
	duch d;
	FILE *fpi;

    fpi = fopen("DB_Import\\DB_Duche.csv", "r");

    printf("\nDuches  : importing ... ");

    fgets(buf, 200, fpi);   // Suprimme entete

    fseek(db->fp_db, db->hdr.off_duc, SEEK_SET);

    while (fgets(buf, 200, fpi) != NULL)
    {
        memset(&d, 0, sizeof(duch));
        strcpy(d.tp_rec, "DUCHE");

        strcpy(fld, strtok(buf,";"));
        d.id_duche = atoi(fld);
        strcpy(fld, strtok(NULL,";"));
        d.id_pays = atoi(fld);
        strcpy(fld, strtok(NULL,";"));
        strncpy(d.nm_duche, fld, strlen(fld)-1);

        i++;

        fwrite(&d, sizeof(duch), 1, db->fp_db);
    }

    db->hdr.nbr_duc = i;

    printf("Done : %d", db->hdr.nbr_duc);

	fclose(fpi);

    fprintf(db->fp_lg, "Duches imported\n");

        // Réécriture de l'en-tête de la base de données pour enregistrer le nombre de duche
    fseek(db->fp_db, 0, SEEK_SET);  // Repositionner le curseur au début du fichier de la base
    fwrite(&db->hdr, sizeof(hder), 1, db->fp_db);  // Réécrire l'en-tête mis à jour

	return ;
}

/****************************************************************************************
* Generation d'un backup de la table duche au format .csv
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Export_Duches(dbb *db)
{
    int i=0;
    duch d;
	FILE *fpi;

    fpi = fopen("DB_Export\\DB_Duche.csv", "w");

    printf("\nDuches   : exporting ... ");

    fprintf(fpi, "Id;Id_Pays;Duche\n");

    fseek(db->fp_db, db->hdr.off_duc, SEEK_SET);

    for (i=1; i<=db->hdr.nbr_duc; i++)
    {
        memset(&d, 0, sizeof(duch));
        fread(&d, sizeof(duch), 1, db->fp_db);

        fprintf(fpi, "%d;%d;%s\n", d.id_duche, d.id_pays, d.nm_duche);
    }

    printf("Done : %d", db->hdr.nbr_duc);

	fclose(fpi);

    fprintf(db->fp_lg, "Duches exported\n");

	return ;
}
/****************************************************************************************
* Chargement du fichier original DB_Noblesse.csv dans la database
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Import_Noblesses(dbb *db)
{
    int i = 0; // Compteur
    char buf[BUF_LEN], fld[BUF_LEN]; // Buffer ligne et champ
    noblesse nob;
    FILE *fpi; // Pointeur vers CSV

    fpi = fopen("DB_Import\\DB_Noblesse.csv", "r");
        // Vérification de l'ouverture du fichier
    if (fpi == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier DB_Noblesse.csv\n");
        fprintf(db->fp_lg, "Erreur : ouverture du fichier DB_Noblesse.csv échouée\n");
        return;
    }

    printf("\nNoblesse: importing ... ");

    fgets(buf, 200, fpi);   // Suprimme entete
    fseek(db->fp_db, db->hdr.off_nob, SEEK_SET);

    while (fgets(buf, 200, fpi) != NULL)
    {
        memset(&nob, 0, sizeof(nob));
        strcpy(nob.tp_rec, "NOBLESSE");

        strcpy(fld, strtok(buf, ";")); // ID Noblesse
        nob.id_nob = atoi(fld);

        strcpy(fld, strtok(NULL, ";")); // Rang
        nob.rang_nob = atoi(fld);

        strcpy(fld, strtok(NULL, ";")); // ID suzerin
        nob.id_suze = atoi(fld);

        strcpy(fld, strtok(NULL, ";")); // ID blason
        nob.id_bla = atoi(fld);

        strcpy(fld, strtok(NULL, ";")); // id pays
        nob.id_pays = atoi(fld);

        strcpy(fld, strtok(NULL, ";")); // ID duche
        nob.id_duche = atoi(fld);

        strcpy(fld, strtok(NULL, ";")); // titre
        strncpy(nob.titre_nob, fld, strlen(fld));

        strcpy(fld, strtok(NULL, ";")); // region
        strncpy(nob.region_nob, fld, strlen(fld));

        strcpy(fld, strtok(NULL, ";")); // zone
        strncpy(nob.zone_nob, fld, strlen(fld)-1);

        fwrite(&nob, sizeof(nob), 1, db->fp_db);
        i++;
    }

    db->hdr.nbr_nob = i;

    printf("Done : %d", db->hdr.nbr_nob);

	fclose(fpi);

    fprintf(db->fp_lg, "Noblesse imported\n");

    // Réécriture de l'en-tête de la base de données pour enregistrer le nombre de villes
    fseek(db->fp_db, 0, SEEK_SET);  // Repositionner le curseur au début du fichier de la base
    fwrite(&db->hdr, sizeof(hder), 1, db->fp_db);  // Réécrire l'en-tête mis à jour

	return ;
}

/****************************************************************************************
* Generation d'un backup de la table ville au format .csv
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Export_Noblesses(dbb *db)
{
    int i=0;
    noblesse nob;
	FILE *fpi;

    fpi = fopen("DB_Export\\DB_Noblesse.csv", "w");
        if (fpi == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier DB_Noblesse.csv\n");
        fprintf(db->fp_lg, "Erreur : ouverture du fichier DB_Noblesse.csv échouée\n");
        return;
    }

    printf("\nNoblesse : exporting ... ");

    fprintf(fpi, "Id;Rang;Id_Suz;Id_Bla;Id_Pays;Id_Duche;Titre;Region;Zone\n");

    fseek(db->fp_db, db->hdr.off_nob, SEEK_SET);

    for (i=1; i<=(db->hdr.nbr_nob); i++)
    {
        memset(&nob, 0, sizeof(nob));
        fread(&nob, sizeof(nob), 1, db->fp_db);

        fprintf(fpi, "%d;%d;%d;%d;%d;%d;%s;%s;%s\n", nob.id_nob,nob.rang_nob,nob.id_suze,nob.id_bla,nob.id_pays,nob.id_duche,nob.titre_nob,nob.region_nob,nob.zone_nob);
    }

    printf("Done : %d", db->hdr.nbr_nob);

	fclose(fpi);

    fprintf(db->fp_lg, "Noblesse exported\n");

	return ;
}

/****************************************************************************************
* Chargement du fichier original DB_Pays.csv dans la database
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Import_Pays(dbb *db)
{
    int i = 0; // compteur pour suivre le nombre de pays importés
    char buf[BUF_LEN], fld[BUF_LEN];
	pays p;  // structure pour stocker les données des pays
	FILE *fpi;  // pointeur vers le fichier CSV

    // Ouvrir le fichier CSV contenant les pays à importer
    fpi = fopen("DB_Import\\DB_Pays.csv", "r");

    printf("\nPays    : importing ... ");

    // Lire et ignorer la première ligne du fichier CSV (l'entête, souvent des titres de colonnes)
    fgets(buf, 200, fpi);   // Supprime l'entête

    // Positionner le curseur dans le fichier de base de données à l'emplacement réservé aux pays
    fseek(db->fp_db, db->hdr.off_pay, SEEK_SET);

    // Boucle pour lire chaque ligne du fichier CSV et importer les données dans la base
    while (fgets(buf, 200, fpi) != NULL)
    {
        // Réinitialiser la structure pays
        memset(&p, 0, sizeof(pays));
        strcpy(p.tp_rec, "PAYS");

        // Lire le champ 'id_pays' et le convertir en entier
        strcpy(fld, strtok(buf, ";"));
        p.id_pays = atoi(fld);


        strcpy(fld, strtok(NULL, ";"));
        // Lire le champ 'nm_pays' (nom du pays)
        strncpy(p.nm_pays, fld, strlen(fld) - 1);  // Retirer le dernier caractère (\n)

        // Incrémenter le compteur de pays
        i++;

        // Écrire les données du pays dans le fichier de base de données à la position courante
        fwrite(&p, sizeof(pays), 1, db->fp_db);
    }

    // Mettre à jour le champ 'nbr_pay' dans l'en-tête avec le nombre de pays importés
    db->hdr.nbr_pay = i;

    printf("Done : %d", db->hdr.nbr_pay);

    // Fermer le fichier CSV
    fclose(fpi);

    // Log de l'importation
    fprintf(db->fp_lg, "Pays imported\n");

    // Réécriture de l'en-tête de la base de données pour enregistrer le nombre de pays
    fseek(db->fp_db, 0, SEEK_SET);  // Repositionner le curseur au début du fichier de la base
    fwrite(&db->hdr, sizeof(hder), 1, db->fp_db);  // Réécrire l'en-tête mis à jour

    return;
}

/****************************************************************************************
* Generation d'un backup de la table pays au format .csv
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Export_Pays(dbb *db)
{
    int i=0;
	pays p;
	FILE *fpi;

    fpi = fopen("DB_Export\\DB_Pays.csv", "w");

    printf("\nPays     : exporting ... ");

    fprintf(fpi, "Id;Pays\n");

    fseek(db->fp_db, db->hdr.off_pay, SEEK_SET);

    for (i=1; i<=db->hdr.nbr_pay; i++)
    {
        memset(&p, 0, sizeof(pays));
        fread(&p, sizeof(pays), 1, db->fp_db);

        fprintf(fpi, "%d;%s\n", p.id_pays, p.nm_pays);
    }

    printf("Done : %d", db->hdr.nbr_pay);

	fclose(fpi);

    fprintf(db->fp_lg, "Pays exported\n");

	return ;
}


/****************************************************************************************
* Load static buffers pays
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Load_Buffers_pays(dbb *db)
{
    int i=0;
	pays p;

    printf("\nBuffers  : loading ... \n\n");

    fseek(db->fp_db, db->hdr.off_pay, SEEK_SET);

    for (i=1; i<=db->hdr.nbr_pay; i++)
    {
        memset(&p, 0, sizeof(pays));
        fread(&p, sizeof(pays), 1, db->fp_db);

        db->pay[i] = p;
    }

	return;
}
/****************************************************************************************
* Load static buffers duche
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Load_Buffers_duch(dbb *db)
{
    int i=0;
	duch d;

    printf("\nBuffers  : loading ... \n\n");

    fseek(db->fp_db, db->hdr.off_duc, SEEK_SET);

    for (i=1; i<=db->hdr.nbr_duc; i++)
    {
        memset(&d, 0, sizeof(duch));
        fread(&d, sizeof(duch), 1, db->fp_db);

        db->duc[i] = d;
    }

	return;
}
/****************************************************************************************
* Lister a l'ecran la table pays a partir du fichier DB
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Lister_Pays_DB(dbb *db)
{
    int i=0;
	pays p;
    printf("Nombre de pays dans la base de données : %d\n", db->hdr.nbr_pay);

    printf("%4s  %-20s\n\n", "Id", "Pays   - DB");

    fseek(db->fp_db, db->hdr.off_pay, SEEK_SET);

    for (i=0; i<db->hdr.nbr_pay; i++)
    {
        memset(&p, 0, sizeof(pays));
        fread(&p, sizeof(pays), 1, db->fp_db);

        printf("%4d  %-20s\n", p.id_pays, p.nm_pays);
    }

    printf("\n");

	return ;
}

/****************************************************************************************
* Lister a l'ecran la table pays a partir du buffer charge
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Lister_Pays_BUF(dbb *db)
{
    int i=0;

    printf("%4s  %-20s\n\n", "Id", "Pays   - BUF");

    for (i=1; i<=db->hdr.nbr_pay; i++)
    {
        printf("%4d  %-20s\n", db->pay[i].id_pays, db->pay[i].nm_pays);
    }

    printf("\n");

	return ;
}
/****************************************************************************************
* Lister à l'écran la table des duchés en deux colonnes à partir du buffer charge
*   db : pointeur vers la base de données blasons
****************************************************************************************/
void Lister_duch_BUF(dbb *db) {
    int i;

    printf("%4s  %-20s    %4s  %-20s\n", "Id", "Duche - BUF", "Id", "Duche - BUF");
    printf("-------------------------------------------------------------\n");

    for (i = 1; i <= db->hdr.nbr_duc; i += 2) { // Avancer par paires
        // Colonne 1
        printf("%4d  %-20s", db->duc[i].id_duche, db->duc[i].nm_duche);
        // Colonne 2 : affichée seulement si elle existe
        if (i + 1 <= db->hdr.nbr_duc) {
            printf("    %4d  %-20s", db->duc[i + 1].id_duche, db->duc[i + 1].nm_duche);
        }
        printf("\n");
    }
    printf("\n");
    return;
}

/****************************************************************************************
* Chargement du fichier original DB_Noblesse.csv dans la database
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Import_Richesses(dbb *db)
{
    int i = 0; // Compteur
    char buf[BUF_LEN], fld[BUF_LEN]; // Buffer ligne et champ
    richesse rich;
    FILE *fpi; // Pointeur vers CSV

    fpi = fopen("DB_Import\\DB_Richesses.csv", "r");
        // Vérification de l'ouverture du fichier
    if (fpi == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier DB_Noblesse.csv\n");
        fprintf(db->fp_lg, "Erreur : ouverture du fichier DB_Noblesse.csv échouée\n");
        return;
    }

    printf("\nRichesse: importing ... ");

    fgets(buf, 200, fpi);   // Suprimme entete
    fseek(db->fp_db, db->hdr.off_rich, SEEK_SET);

    while (fgets(buf, 200, fpi) != NULL)
    {
        memset(&rich, 0, sizeof(rich));
        strcpy(rich.tp_rec, "RICHESSES");

        strcpy(fld, strtok(buf, ";")); // ID ville
        rich.id_ville = atoi(fld);

        strcpy(fld, strtok(NULL, ";")); // annee
        rich.annee = atoi(fld);

        strcpy(fld, strtok(NULL, ";")); // population
        rich.popu = atoi(fld);

        strcpy(fld, strtok(NULL, ";")); // produit
        rich.prod = atoi(fld)-1;

        fwrite(&rich, sizeof(rich), 1, db->fp_db);
        i++;
    }

    db->hdr.nbr_richesse = i;

    printf("Done : %d", db->hdr.nbr_richesse);

	fclose(fpi);

    fprintf(db->fp_lg, "Richesses imported\n");

    // Réécriture de l'en-tête de la base de données pour enregistrer le nombre de villes
    fseek(db->fp_db, 0, SEEK_SET);  // Repositionner le curseur au début du fichier de la base
    fwrite(&db->hdr, sizeof(hder), 1, db->fp_db);  // Réécrire l'en-tête mis à jour

	return ;
}

/****************************************************************************************
* Generation d'un backup de la table richesses au format .csv
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Export_Richesses(dbb *db)
{
    int i=0;
    richesse rich;
	FILE *fpi;

    fpi = fopen("DB_Export\\DB_Richesses.csv", "w");
        if (fpi == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier DB_Richesses.csv\n");
        fprintf(db->fp_lg, "Erreur : ouverture du fichier DB_Richesses.csv échouée\n");
        return;
    }

    printf("\nRichesses: exporting ... ");

    fprintf(fpi, "Id_Ville;Annee;Population;Produit\n");

    fseek(db->fp_db, db->hdr.off_rich, SEEK_SET);

    for (i=1; i<=(db->hdr.nbr_richesse); i++)
    {
        memset(&rich, 0, sizeof(rich));
        fread(&rich, sizeof(rich), 1, db->fp_db);

        fprintf(fpi, "%d;%d;%d;%d\n", rich.id_ville,rich.annee,rich.popu,rich.prod);
    }

    printf("Done : %d", db->hdr.nbr_richesse);

	fclose(fpi);

    fprintf(db->fp_lg, "Richesses exported\n");

	return ;
}

/****************************************************************************************
* Chargement du fichier original DB_Ville.csv dans la database
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Import_Villes(dbb *db)
{
    int i=0;
    char buf[BUF_LEN], fld[BUF_LEN];
	ville v;
	FILE *fpi;

    fpi = fopen("DB_Import\\DB_ville.csv", "r");

    printf("\nVilles  : importing ... ");

    fgets(buf, 200, fpi);   // Suprimme entete

    fseek(db->fp_db, db->hdr.off_ville, SEEK_SET);

    while (fgets(buf, 200, fpi) != NULL)
    {
        memset(&v, 0, sizeof(ville));
        strcpy(v.tp_rec, "VILLE");

        strcpy(fld, strtok(buf,";"));
        v.id_ville = atoi(fld);

        strcpy(fld, strtok(NULL,";"));
        v.id_duch = atoi(fld);

        strcpy(fld, strtok(NULL,";"));
        strncpy(v.nom_ville, fld, strlen(fld));

        strcpy(fld, strtok(NULL,";"));
        v.nr = atoi(fld);

        strcpy(fld, strtok(NULL,";"));
        strncpy(v.cap, fld, strlen(fld)-1);

        i++;

        fwrite(&v, sizeof(ville), 1, db->fp_db);
    }

    db->hdr.nbr_ville = i;

    printf("Done : %d", db->hdr.nbr_ville);

	fclose(fpi);

    fprintf(db->fp_lg, "Ville imported\n");

    // Réécriture de l'en-tête de la base de données pour enregistrer le nombre de villes
    fseek(db->fp_db, 0, SEEK_SET);  // Repositionner le curseur au début du fichier de la base
    fwrite(&db->hdr, sizeof(hder), 1, db->fp_db);  // Réécrire l'en-tête mis à jour

	return ;
}

/****************************************************************************************
* Generation d'un backup de la table ville au format .csv
*   db : pointeur de base de donnees blasons
****************************************************************************************/
void Export_Villes(dbb *db)
{
    int i=0;
    ville v;
	FILE *fpi;

    fpi = fopen("DB_Export\\DB_Ville.csv", "w");

    printf("\nVilles   : exporting ... ");

    fprintf(fpi, "Id;Id_Duc;ville;Nr;Capital\n");

    fseek(db->fp_db, db->hdr.off_ville, SEEK_SET);

    for (i=1; i<=db->hdr.nbr_ville; i++)
    {
        memset(&v, 0, sizeof(ville));
        fread(&v, sizeof(ville), 1, db->fp_db);

        fprintf(fpi, "%d;%d;%s;%d;%s\n", v.id_ville, v.id_duch, v.nom_ville, v.nr, v.cap);
    }

    printf("Done : %d", db->hdr.nbr_ville);

	fclose(fpi);

    fprintf(db->fp_lg, "Villes exported\n");

	return ;
}




