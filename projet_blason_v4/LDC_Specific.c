/****************************************************************************************
* Listes doublement chainees circulaires : Version generique
*
* Version basee sur demo Open Classroom
*
* Fonctions specifiques a l'application
*
* Programmation procedurale
****************************************************************************************/
#include "DB_Main.h"
/****************************************************************************************
* Impression pays
****************************************************************************************/
void Print_Pays(void *pt)
{
    pays *c=NULL;

    c = (pays*)pt;

    printf("Pays : ID = %d    Nom = %-12s \n", c->id_pays, c->nm_pays);

    return;
}
/****************************************************************************************
* Print_Citoyen
****************************************************************************************/
void Print_Citoyen(void *pt)
{
    citoyen *c=NULL;

    c = (citoyen*)pt;

    printf("Citoyen ID : %d - Nom : %-12s - Prenom %s \n", c->id_cit, c->nom_cit, c->pre_cit);

    return;
}
/****************************************************************************************
* Impression duche
****************************************************************************************/
void Print_duche(void *pt)
{
    duch *d=NULL;

    d = (duch*)pt;

    printf("Duche ID = %3d   Pays ID = %3d    Nom =  %-12s \n", d->id_duche, d->id_pays, d->nm_duche );

    return;
}
/****************************************************************************************
* Impression ville
****************************************************************************************/
void Print_ville(void *pt)
{
    ville *c=NULL;

    c = (ville*)pt;

    printf("Ville : ID = %d  ID Duche = %d Nom Ville %-12s Nr = %d \n", c->id_ville, c->id_duch, c->nom_ville, c->nr);

    return;
}
/****************************************************************************************
* Impression Record
****************************************************************************************/
void Print_record(void *pt)
{
    record *rec = NULL;

    // Conversion du pointeur générique en pointeur vers record
    rec = (record *)pt;

    // Afficher les détails de la structure record
//    printf("Charge ID : %3d | Citoyen ID : %3d | Noblesse ID : %3d | Offset Citoyen : %3d\n",
//           rec->cha.id_nob, rec->cit.id_cit, rec->nob.id_nob, rec->off_cit);


    printf("Off : %d | ID ch : %d | ID cit : %d |  Année entrée : %4d | Année sortie : %4d\n\n",rec->off_cit, rec->cha.id_nob, rec->cha.id_indi , rec->cha.yr_in, rec->cha.yr_out);
    printf("Nom : %s | Titre : %s \n\n",rec->cit.nom_cit, rec->nob.titre_nob);
    printf("ID Duche: %d | Nom Duche : %s \n\n",rec->duc.id_duche, rec->duc.nm_duche);
   // printf("  Nom citoyen  : %-12s | Titre noblesse : %-12s\n",rec->cit.nom_cit, rec->nob.titre_nob);

    return;
}

/****************************************************************************************
* Premier plus grand pays : ordre alpha sur nom
****************************************************************************************/
int PGd_Pays(void *r1, void *r2)
{
    pays *j1, *j2;

    j1 = (pays*)r1;
    j2 = (pays*)r2;

    return strcmp(j1->nm_pays, j2->nm_pays)<0?1:0;
}

/****************************************************************************************
* Premier plus grand citoyen : ordre alpha sur nom
****************************************************************************************/
int PGd_Citoyen(void *r1, void *r2)
{
    citoyen *c1, *c2;
    //int comp;
    c1 = (citoyen*)r1;
    c2 = (citoyen*)r2;

    if (strcmp(c1->nom_cit, c2->nom_cit) == 0)
    {
        return strcmp(c1->pre_cit, c2->pre_cit) < 0 ? 1 : 0;
    }
    return strcmp(c1->nom_cit, c2->nom_cit)<0?1:0;
}
/****************************************************************************************
* Premier plus grand duche : ordre alpha sur duche
****************************************************************************************/
int PGd_Duche(void *r1, void *r2)
{
    duch *d1, *d2;
    d1 = (duch *)r1;
    d2 = (duch *)r2;

    return strcmp(d1->nm_duche, d2->nm_duche) < 0 ? 1 : 0;
}
/****************************************************************************************
* Charger un nombre de citoyens nr  depuis la DB vers une liste DC
****************************************************************************************/
void Load_Citoyen(dbb *db, Gen_LDC *rac_per, int nr)
{
    int i=0;
    citoyen rec ;
    Gen_LDC *pt;

    fseek(db->fp_db, db->hdr.off_cit, SEEK_SET);

    while (i< db->hdr.nbr_cit && i < nr )
    {
        memset(&rec, 0, sizeof(rec));

        fread(&rec, sizeof(citoyen), 1, db->fp_db);


        pt = chercherPGd(rac_per, &rec, PGd_Citoyen);

        ajouterAvant(pt, &rec, sizeof(rec));

        i++;
    }

    printf("\nNombre de citoyens charges : %d\n\n", i);

    return;
}

void Test_LDC1(dbb *db)
{
    int i;
    pays p;
    duch d;
    int NRP = db->hdr.nbr_pay;
    int NRD = db->hdr.nbr_duc;
    Gen_LDC *rac_pays, *rac_duch;
    // Creation de la liste
    rac_pays = creeListe();
    rac_duch =  creeListe();
    // Chargement de la liste pays
    for (i=1; i<=NRP; i++)
    {
        p = db->pay[i];
        ajouterEnTete(rac_pays, &p, sizeof(p));
    }
    printf("\nListe des Pays  directe et inverse(ID et Nom):\n");
    printDir(rac_pays, Print_Pays);
    printInv(rac_pays, Print_Pays);
    //Chargement de la liste duche
    for (i=1; i<=NRD; i++)
    {
        d = db->duc[i];
        ajouterEnQueue(rac_duch, &d, sizeof(d));
    }
    printf("\nListe des duches  directe et inverse(ID et Nom):\n");
    printDir(rac_duch, Print_duche);
    printInv(rac_duch, Print_duche);
    supprimerListe(&rac_pays);
    supprimerListe(&rac_duch);
}
void Test_LDC2(dbb *db)
{
    int i;
    pays p;
    duch d;
    int NRP = db->hdr.nbr_pay;
    int NRD = db->hdr.nbr_duc;
    Gen_LDC *rac_pays, *rac_duch, *pt;
    // Creation de la liste
    rac_pays = creeListe();
    rac_duch =  creeListe();
    // Chargement de la liste pays
    for (i=1; i<=NRP; i++)
    {
        p = db->pay[i];
        pt = chercherPGd(rac_pays, &p, PGd_Pays);
        ajouterEnTete(rac_pays, &p, sizeof(p));
    }
    printf("\nListe des Pays  directe et inverse(ID et Nom):\n");
    printDir(rac_pays, Print_Pays);
    printInv(rac_pays, Print_Pays);
    //Chargement de la liste duche
    for (i=1; i<=NRD; i++)
    {
        d = db->duc[i];
        pt = chercherPGd(rac_duch, &d, PGd_Duche);
        ajouterEnQueue(rac_duch, &d, sizeof(d));
    }
    printf("\nListe des duches  directe et inverse(ID et Nom):\n");
    printDir(rac_duch, Print_duche);
    printInv(rac_duch, Print_duche);
    supprimerListe(&rac_pays);
    supprimerListe(&rac_duch);
 }
void Test_LDC3(dbb *db)
{
    Gen_LDC *rac_cit;
    rac_cit = creeListe();
    Load_Citoyen(db,rac_cit, 20);
    printDir(rac_cit, Print_Citoyen);
    supprimerListe(&rac_cit);
}
void Test_LDC4(dbb *db)
{
    Gen_LDC *rac_cit;
    rac_cit = creeListe();
    Load_Citoyen(db,rac_cit, 20);
    supprimerPremierElement(rac_cit);
    supprimerDernierElement(rac_cit);
    printDir(rac_cit, Print_Citoyen);
    supprimerListe(&rac_cit);
}
