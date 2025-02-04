/****************************************************************************************
* Listes doublement chainees circulaires : Version generique
*
* Version basee sur demo Open Classroom
*
* Test de la librairie (3 types de records : Cty, Job, Pers)
*
* Programmation procedurale 2024 - E.Bosly
****************************************************************************************/
#include "LDC_Main_4.h"

#define NR 5

ccty cty[NR]={
    { 1, "France", "FR" },
    { 2, "Italie", "IT" },
    { 3, "Espagne", "SP" },
    { 4, "Suisse", "CH" },
    { 5, "Portugal", "PT" }
};

cjob job[NR]={
    { 1, "Manager", "MG" },
    { 2, "Director", "DI" },
    { 3, "Secretary", "SE" },
    { 4, "Clerck", "CL" },
    { 5, "Lawyer", "LW" }
};

/****************************************************************************************
* Demonstration de la librairie LDC
****************************************************************************************/
int main(void)
{
    int i;

    ccty c;
    cjob j;

    Gen_LDC *rac_cty, *rac_job, *rac_per, *pt;

    // Creation de la liste
    rac_cty = creeListe();
    rac_job = creeListe();
    rac_per = creeListe();

    // Chargement de la liste cty
    for (i=0; i<NR; i++)
    {
        c = cty[i];
        ajouterEnTete(rac_cty, &c, sizeof(c));
    }

    printDir(rac_cty, Print_Cty);
    printInv(rac_cty, Print_Cty);

    // Chargement de la liste job
    for (i=0; i<NR; i++)
    {
        j = job[i];
        ajouterEnQueue(rac_job, &j, sizeof(j));
    }

    printDir(rac_job, Print_Job);
    printInv(rac_job, Print_Job);

    // Reimpression apres modification

    viderListe(rac_cty);

    printDir(rac_job, Print_Job);
    printDir(rac_cty, Print_Cty);

    supprimerPremierElement(rac_job);
    supprimerDernierElement(rac_job);

    printDir(rac_job, Print_Job);
    printDir(rac_cty, Print_Cty);

    // Chargement trie des listes

    viderListe(rac_job);

    for (i=0; i<NR; i++)
    {
        j = job[i];
        pt = chercherPGd(rac_job, &j, PGd_Job);
        ajouterAvant(pt, &j, sizeof(j));
    }

    for (i=0; i<NR; i++)
    {
        c = cty[i];
        pt = chercherPGd(rac_cty, &c, PGd_Cty);
        ajouterAvant(pt, &c, sizeof(c));
    }

    printDir(rac_job, Print_Job);
    printInv(rac_job, Print_Job);

    printDir(rac_cty, Print_Cty);
    printInv(rac_cty, Print_Cty);

    // Chargement de Nr person depuis fichier

    Load_Person(rac_per, 15);

    printDir(rac_per, Print_Pers);
    printInv(rac_per, Print_Pers);

    // Suppression de la liste
    supprimerListe(&rac_cty);
    supprimerListe(&rac_job);
    supprimerListe(&rac_per);

    return 0;
}
