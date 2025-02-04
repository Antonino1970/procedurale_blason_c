/****************************************************************************************
* Dossier Blasons :
*
* Programme principal
*
* Programmation procedurale 2024 - A.Umina - Version 3
****************************************************************************************/
#include "DB_Main.h"
#include "menu.h"

/****************************************************************************************
* Programme principal
****************************************************************************************/

int main(void)
{

    dbb db = {0};
    menu(&db);
    Close_DB(&db);
    return 0;
}



void test_image(dbb *db)
{
    printf("\n--- Test 5 -----------------------------------------------------\n\n");
    blason bla1, bla2 ,bla3;
    cart_roi roi;
    cart_duc duc;
    cart_emp emp;
    blason mask;

    // Créer l'image principale
    image *img = Creer_Image("cartouche", 1600, 1200, BLEU_6);
    if (img == NULL) {
        printf("Erreur lors de la création de l'image.\n");
        return;
    }

    // Écrire une chaîne de texte dans l'image
    Ecrire_Chaine(img, "toni", 1, 0, 500, 500, NOIR, 10);

    // Initialiser les blasons et le masque, et les écrire dans l'image
    Lire_Blason_db(db, &bla1, 0);
    Lire_Blason_db(db, &bla2, 1);
    Lire_Blason_db(db, &bla3, 2);
    Lire_Mask(&mask, 0); // Charger le masque pour le blason
    Ecrire_Blason(&mask, "T2"); // Appliquer un texte dans le masque

    // Dessiner des lignes de repère dans l'image principale
    Dessiner_Ligne_Points(img, 0, 600, 1600, 600, NOIR, 5);
    Dessiner_Ligne_Points(img, 800, 0, 800, 1200, NOIR, 5);

    // Écrire le blason avec masque dans l'image
    Ecrire_Blason_M_img(img, &bla1, &mask, 100, 100);
    Ecrire_Blason_img(img, &mask, 300, 100);

    // Initialiser les cartouches avec les blasons, les couleurs de fond spécifiques, et un masque pour l'empereur
    Initialiser_Cartouche_duc(&duc, &bla2, Get_Col(VERT), Get_Col(VERT_5));
    Initialiser_Cartouche_roi2(&roi, &bla3, Get_Col(JAUNE), Get_Col(BLEU_5));
    //Initialiser_Cartouche_emp(emp, bla1, mask, Get_Col(ROUGE), Get_Col(ROUGE_5));

    // Écrire les cartouches dans l'image principale
    Ecrire_Cartouche_roi(img, &roi, 200, 800);
    Ecrire_Cartouche_duc(img, &duc, 200, 600);
    Ecrire_Cartouche_emp(img, &emp, 800, 1100);

    // Définir les points d'ancrage pour dessiner les lignes de liaison entre les cartouches
    int pt1_x = roi.ancre_x;
    int pt1_y = roi.ancre_y;
    int pt2_x = emp.ancre_x;
    int pt2_y = emp.ancre_y;
    Dessiner_Ligne_Points(img, pt1_x, pt1_y, pt2_x, pt2_y, NOIR, 5);

    pt1_x = roi.ancre_x;
    pt1_y = roi.ancre_y - CART_ROI_HAUT;
    pt2_x = duc.ancre_x;
    pt2_y = duc.ancre_y;
    Dessiner_Ligne_Points(img, pt1_x, pt1_y, pt2_x, pt2_y, NOIR, 2);

    // Écrire à nouveau le cartouche empereur pour le positionnement
    Ecrire_Cartouche_emp(img, &emp, 800, 1100);

    // Sauvegarder l'image avec les cartouches insérés
    Ecrire_Image(img, "_cart_1");

    // Libérer la mémoire de l'image principale
    Free_Image(img);
}


