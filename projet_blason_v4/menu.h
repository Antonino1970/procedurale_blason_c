/****************************************************************************************
* menu
*
*
* Programmation procedurale 2024 - A.Umina - image blason version 3
****************************************************************************************/
#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**********************************************************************************
* enumeration menu base de donnees imperiale
       0.  Fin

       1.  Ouvrir DB
       2.  Cree BD vide
       3.  Informations systeme

**********************************************************************************/
enum menu_principale
{
        FIN,
        OUVRIR_DB,
        CREE_DB,
        INFO_SYST
};

/**********************************************************************************
* enumeration menu de la DB ouverte
       0.  Fermer DB

       1.  Importer Fichier dans DB
       2.  Exporter BD dans fichier
       3.  Recherche sequentielle dans DB
       4.  Recherche indexee dans DB
       5.  Produire Rapports
       6.  Informations systeme

**********************************************************************************/
enum menu_DB
{
        FERMER,
        IMPORT,
        EXPORT,
        CREE_INDEX,
        LISTER,
        LIST_DC,
        RECH_IND,
        RAPPORT,
        INFO_DB,

};

/**********************************************************************************
* enumeration recherche sequentielle
       0.  Retour

       1.  Lister Pays
       2.  Lister Duches
       3.  Lister Villes
       4.  Lister Noblesse
       5.  Lister Charges
       6.  Lister Citoyens

**********************************************************************************/
enum menu_Rech_Seq
{
        RETOUR,
        PAYS,
        DUCHER,
        NOBLESSE,
        CHARGES,
        CITOYENS
};

/**********************************************************************************
* enumeration recherche indexee
       0.  Retour

       1.  Lister Roi Sequentiel
       2.  Lister Dichotomique
       3.  Recherche charge index arbre

**********************************************************************************/
enum menu_Rech_Ind
{
        RETOUR2,
        ROI_SEQ,
        CIT_IND,
        CHA_ARB
};



#endif // MENU_H_INCLUDED
