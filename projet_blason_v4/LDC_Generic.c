/****************************************************************************************
* Listes doublement chainees circulaires : Version generique
*
* Version basee sur demo Open Classroom
*
* Code de la librairie generique
*
* Programmation procedurale 2024
****************************************************************************************/
#include "DB_Main.h"

/****************************************************************************************
* Creation d'une liste generique
****************************************************************************************/
Gen_LDC* creeListe (void)
{
    Gen_LDC* racine = malloc ( sizeof *racine );
    if ( racine != NULL )  /* si la racine a été correctement allouée */
    {
        /* pour l'instant, la liste est vide,
           donc 'prec' et 'suiv' pointent vers la racine elle-même */
        racine->prec = racine;
        racine->suiv = racine;
    }
    return racine;
}

/****************************************************************************************
* Parcourir la liste en sens direct : canevas pour implementer des fonctions
****************************************************************************************/
void parcourDir(Gen_LDC* liste)
{
    int i=1;
    Gen_LDC* it;

    for ( it = liste->suiv; it != liste; it = it->suiv )
        printf("%4d %08X \n", i++, it->rec);
}

/****************************************************************************************
* Impression en sens direct de la liste : base sur le canevas parcourDir
****************************************************************************************/
void printDir(Gen_LDC* liste, void (* pF) (void*) )
{
    int i=1;
    Gen_LDC* it;
    for ( it = liste->suiv; it != liste; it = it->suiv )
    {
        printf("%4d : ", i++);
        pF(it->rec);
    }
    puts("---------------------------------------------------------------");
}

/****************************************************************************************
* Parcourir la liste en sens inverse : canevas pour implementer des fonctions
****************************************************************************************/
void parcourInv(Gen_LDC* liste)
{
    int i=1;
    Gen_LDC* rit;
    for ( rit = liste->prec; rit != liste; rit = rit->prec )
        printf("%4d %08X \n", i++, rit->rec);
}

/****************************************************************************************
* Impression en sens inverse de la liste : base sur le canevas parcourInv
****************************************************************************************/
void printInv(Gen_LDC* liste, void (* pF) (void*) )
{
    int i=1;
    Gen_LDC* rit;
    for ( rit = liste->prec; rit != liste; rit = rit->prec )
    {
        printf("%4d : ", i++);
        pF(rit->rec);
    }
    puts("---------------------------------------------------------------");
}

/****************************************************************************************
* Vider la liste sans pour autant la detruire
****************************************************************************************/
void viderListe (Gen_LDC* liste)
{
    Gen_LDC *it, *next;

    for ( it = liste->suiv; it != liste; it = next )
    {
        next = it->suiv;  /* on enregistre le pointeur sur l'élément suivant avant de supprimer l'élément courant */
        free(it->rec);    /* on supprime le record data */
        free(it);         /* on supprime l'élément courant */
    }

    liste->prec = liste; // Correction EB
    liste->suiv = liste;
}

/****************************************************************************************
* Detruire la liste, apres l'avoir videe
****************************************************************************************/
void supprimerListe (Gen_LDC** liste)
{
    viderListe( *liste );  /* on vide d'abord la liste */
    free( *liste ), *liste = NULL;
}

/****************************************************************************************
* Ajouter avant element le record donne
****************************************************************************************/
void ajouterAvant (Gen_LDC* element, void *rec, int size_rec)
{
    Gen_LDC* nouvel_element = malloc ( sizeof *nouvel_element );
    if ( nouvel_element != NULL )
    {
        nouvel_element->rec = malloc ( size_rec );
        memcpy(nouvel_element->rec, rec, size_rec);
        /* on définit les pointeurs du nouvel élément */
        nouvel_element->prec = element->prec;
        nouvel_element->suiv = element;
        /* on modifie les éléments de la liste */
        element->prec->suiv = nouvel_element;
        element->prec = nouvel_element;
    }
}

/****************************************************************************************
* Ajouter apres element le record donne
****************************************************************************************/
void ajouterApres (Gen_LDC* element, void *rec, int size_rec)
{
    Gen_LDC* nouvel_element = malloc ( sizeof *nouvel_element );
    if ( nouvel_element != NULL )
    {
        nouvel_element->rec = malloc ( size_rec );
        memcpy(nouvel_element->rec, rec, size_rec);
        /* on définit les pointeurs du nouvel élément */
        nouvel_element->prec = element;
        nouvel_element->suiv = element->suiv;
        /* on modifie les éléments de la liste */
        element->suiv->prec = nouvel_element;
        element->suiv = nouvel_element;
    }
}

/****************************************************************************************
* Ajouter le record donne en tete de liste
****************************************************************************************/
void ajouterEnTete (Gen_LDC* racine, void *rec, int size_rec)
{
    ajouterApres (racine, rec, size_rec);
}

/****************************************************************************************
* Ajouter le record donne en fin de liste
****************************************************************************************/
void ajouterEnQueue (Gen_LDC* racine, void *rec, int size_rec)
{
    ajouterAvant (racine, rec, size_rec);
}

/****************************************************************************************
* Supprimer element de la liste
****************************************************************************************/
void supprimerElement (Gen_LDC* element)
{
    element->prec->suiv = element->suiv;
    element->suiv->prec = element->prec;
    /* on libère la mémoire allouée */
    free(element->rec);
    free(element);
}

/****************************************************************************************
* Supprimer le premier element de la liste
****************************************************************************************/
void supprimerPremierElement (Gen_LDC* racine)
{
    if (racine->suiv != racine)
        supprimerElement (racine->suiv);
}

/****************************************************************************************
* Supprimer le dernier element de la liste
****************************************************************************************/
void supprimerDernierElement (Gen_LDC* racine)
{
    if (racine->prec != racine)
        supprimerElement (racine->prec);
}

/****************************************************************************************
* Retourner le premier element de la liste
****************************************************************************************/
Gen_LDC* premierElement (Gen_LDC* racine)
{
    if (racine->suiv != racine)  /* on vérifie que l'élément existe bien */
        return racine->suiv;
    else                         /* sinon on retourne NULL */
        return NULL;
}

/****************************************************************************************
* Retourner le dernier element de la liste
****************************************************************************************/
Gen_LDC* dernierElement (Gen_LDC* racine)
{
    if (racine->prec != racine)  /* on vérifie que l'élément existe bien */
        return racine->prec;
    else                         /* sinon on retourne NULL */
        return NULL;
}

/****************************************************************************************
* Calculer le nombre d'element dans la liste
****************************************************************************************/
size_t nombreElements (Gen_LDC* liste)
{
    size_t n = 0;
    Gen_LDC* it;

    for ( it = liste->suiv; it != liste; it = it->suiv )
        n++;

    return n;
}

/****************************************************************************************
* Recherche le premier plus grand dans la liste : pF donne "r1 < r2"
****************************************************************************************/
Gen_LDC * chercherPGd(Gen_LDC* liste, void *rec, int (* pF) (void *r1, void *r2))
{
    Gen_LDC* it;

    for ( it = liste->suiv; it != liste && pF(it->rec, rec); it = it->suiv );

    return it;
}

