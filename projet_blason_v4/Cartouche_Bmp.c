/****************************************************************************************
* Generation d'images .bmp 24 bits
*
* Gestion des cartouches
*
* Programmation procedurale 2024 - A.Umina - image blason version 0
****************************************************************************************/
#include "DB_Main.h"

/****************************************************************************************
* Lire un blason dans un fichier
****************************************************************************************/
void Lire_Blason(blason * bla, int position)
{
    FILE * fp;
    fp = fopen(".\\Mask\\Mask_1.bmp", "rb");
   // fp = fopen(".\\DB_Import\\DB_Blason_Image.dat", "rb");
    if (fp == NULL)
    {
        printf("Erreur : Impossible d'ouvrir le fichier \n");
        return;
    }

    fseek(fp, position * sizeof(blason), SEEK_SET);
    fread(bla, sizeof(blason), 1, fp);
    if (feof(fp))
    {
        printf("Erreur : feof \n");
    }
    fclose(fp);
}
/****************************************************************************************
* Lire un masque de blason dans un fichier BMP (avec vérification et débogage)
****************************************************************************************/
void Lire_Mask(blason * bla , int position)
{

    FILE * fp;
    fp = fopen(".\\Mask\\Mask.bmp", "rb");
   // fp = fopen(".\\DB_Import\\DB_Blason_Image.dat", "rb");
    if (fp == NULL)
    {
        printf("Erreur : Impossible d'ouvrir le fichier \n");
        return;
    }

    fseek(fp, position * sizeof(blason), SEEK_SET);
    fread(bla, sizeof(blason), 1, fp);
    if (feof(fp))
    {
        printf("Erreur : feof \n");
    }
    fclose(fp);
}



/****************************************************************************************
* Lire un blason dans la db
****************************************************************************************/
void Lire_Blason_db(dbb *db, blason * blaz, int position)
{
    if (db == NULL)
    {
        printf("Erreur : Impossible d'ouvrir la DB\n");
        return;
    }

    // Repositionner le curseur à la bonne position pour lire le blason
    fseek(db->fp_db, db->hdr.off_blaz + sizeof(blason) * position, SEEK_SET);

    // Lire le blason à la position donnée
    if (fread(blaz, sizeof(blason), 1, db->fp_db) != 1)
    {
        printf("Erreur : impossible de lire le blason à la position %d\n", position);
    }

    // Optionnel : Vérifier si on est à la fin du fichier (peut-être utile pour éviter des erreurs)
    if (feof(db->fp_db))
    {
        printf("Erreur : fin de fichier atteinte avant la lecture complète du blason\n");
    }
}


/****************************************************************************************
* Ecrire l'image blason sur disque
*   bla    : image blason
*   nm_tag : extension du nom de fichier
****************************************************************************************/
void Ecrire_Blason(blason * bla, char * nm_tag)
{
    char nm_fil[80];
    FILE * fp;

    Nom_Image(nm_fil, "Blason", nm_tag);

    fp=fopen(nm_fil, "w");

    fwrite(bla, sizeof(blason), 1, fp);

    fclose(fp);

    return;
}

/****************************************************************************************
* Initialiser l'image blason a une valeur constante
*   bla : image blason
*   col : pixel de la couleur voulue
****************************************************************************************/
void Initialiser_Blason(blason * bla, pixel * col)
{
    int x, y;

    memset(bla, 0, sizeof(blason));

    bla->header.typ[0]='B';
    bla->header.typ[1]='M';

    bla->header.largeur = BLA_LARG;
    bla->header.hauteur = BLA_HAUT;

    bla->header.bitpix = BIT_PIXEL;
    bla->header.offset = HEADER_SIZE;
    bla->header.lg_head= HEADER_SIZE - 14;
    bla->header.taille = bla->header.offset + ((3 * bla->header.largeur + bla->header.largeur%4) * bla->header.hauteur);

    bla->header.applic_id[0] = 'T';
    bla->header.applic_id[1] = 'O';
    bla->header.applic_id[2] = 'N';
    bla->header.applic_id[3] = 'Y';

    bla->header.planes = 1;

    for (x=0; x<BLA_LARG; x++)
    {
        for (y=0; y<BLA_HAUT; y++)
        {
            bla->img[y][x] = *col;
        }
    }

    return;
}
/****************************************************************************************
* Ecrire un blason sur une image
* Parametres :
*   - img  : Image de destination dans laquelle du blason .
*   - bla  : Structure blason .
*   - x    : origine x dans l image.
*   - y    : origine y dans l image.
****************************************************************************************/
void Ecrire_Blason_img(image * img, blason * bla, int posX, int posY)
{
    int largeur_Blason = bla->header.largeur;
    int hauteur_Blason = bla->header.hauteur;

    // Parcourir tous les pixels du blason
    for (int y = 0; y < hauteur_Blason; y++)
    {
        for (int x = 0; x < largeur_Blason; x++)
        {
            Set_Pixel(img, posX + x, posY + y, &bla->img[y][x]);
        }
    }
}

/****************************************************************************************
* Ecrire un blason masqué sur une image
* Parametres :
*   - img  : Image de destination dans laquelle du blason .
*   - bla  : Structure blason .
*   - x    : origine x dans l image.
*   - y    : origine y dans l image.
****************************************************************************************/
void Ecrire_Blason_M_img(image *img, blason *bla, blason *mask, int posX, int posY)
{
    int largeur_Blason = bla->header.largeur;
    int hauteur_Blason = bla->header.hauteur;

    // Parcourir tous les pixels du blason
    for (int y = 0; y < hauteur_Blason; y++)
    {
        for (int x = 0; x < largeur_Blason; x++) {
            // Vérifier si le pixel du masque est noir
            if (mask->img[y][x].B == 0 && mask->img[y][x].G == 0 && mask->img[y][x].R == 0) {
                // Ne pas copier ce pixel
                continue;
            }

            // Copier le pixel dans l'image si le masque n'est pas noir
            Set_Pixel(img, posX + x, posY + y, &bla->img[y][x]);
        }
    }
}


/****************************************************************************************
 * Ecrire un blason sur le cartouche image
 * Paramètres :
 *   - img  : Image de destination dans laquelle le blason sera écrit.
 *   - bla  : Structure blason.
 *   - posX : Position x du blason dans l'image.
 *   - posY : Position y du blason dans l'image.
 *   - type : Type de cartouche (CART_ROI, CART_DUC, ou CART_EMP).
 ****************************************************************************************/
void Ecrire_Blason_cart_roi(cart_roi *cart, blason *bla, int posX, int posY)
{
    int largeur_Blason = bla->header.largeur;
    int hauteur_Blason = bla->header.hauteur;

    // Vérification des limites de l'image pour le blason
    if (posX < 0 || posX + largeur_Blason > cart->header.largeur ||
            posY < 0 || posY + hauteur_Blason > cart->header.hauteur)
    {
        printf("Erreur : Le blason dépasse les limites de l'image du cartouche.\n");
        return;
    }

    // Parcourir tous les pixels du blason
    for (int y = 0; y < hauteur_Blason; y++)
    {
        for (int x = 0; x < largeur_Blason; x++)
        {
            Set_Pixel_Cartouche_roi(cart, posX + x, posY + y, &bla->img[y][x]);
        }

    }
}
/****************************************************************************************
 * Ecrire un blason duc sur le cartouche image
 * Paramètres :
 *   - img  : Image de destination dans laquelle le blason sera écrit.
 *   - bla  : Structure blason.
 *   - posX : Position x du blason dans l'image.
 *   - posY : Position y du blason dans l'image.
 *   - type : Type de cartouche (CART_ROI, CART_DUC, ou CART_EMP).
 ****************************************************************************************/
void Ecrire_Blason_cart_duc(cart_duc *cart, blason *bla, int posX, int posY)
{
    int largeur_Blason = bla->header.largeur;
    int hauteur_Blason = bla->header.hauteur;

    // Vérification des limites de l'image pour le blason
    if (posX < 0 || posX + largeur_Blason > cart->header.largeur ||
            posY < 0 || posY + hauteur_Blason > cart->header.hauteur)
    {
        printf("Erreur : Le blason dépasse les limites de l'image du cartouche.\n");
        return;
    }

    // Parcourir tous les pixels du blason
    for (int y = 0; y < hauteur_Blason; y++)
    {
        for (int x = 0; x < largeur_Blason; x++)
        {
            Set_Pixel_Cartouche_duc(cart, posX + x, posY + y, &bla->img[y][x]);
        }

    }
}

/****************************************************************************************
 * Initialiser un cartouche rectangulaire à une valeur constante.
 * Paramètres :
 *   - cart     : structure cartouche qui contiendra un blason, un nom et un titre.
 *   - bla      : image blason.
 *   - col      : pixel du cartouche.
 *   - bord_col : couleur de contours.
 ****************************************************************************************/
void Initialiser_Cartouche_roi2(cart_roi *cart, blason *bla, pixel *col, pixel *bord_col)
{
    int x, y;
    int posX, posY;
    // Réinitialisation de la structure
    memset(cart, 0, sizeof(cart_roi));
    // Initialisation de l'en-tête du cartouche
    cart->type = CART_ROI;
    cart->ancre_x = CART_ROI_LARG/2;
    cart->ancre_y = CART_ROI_HAUT/2;
    cart->header.typ[0] = 'B';
    cart->header.typ[1] = 'M';
    // Déterminer les dimensions en fonction du type de cartouche
    cart->header.largeur = CART_ROI_LARG;
    cart->header.hauteur = CART_ROI_HAUT;

    cart->header.bitpix = BIT_PIXEL;
    cart->header.offset = HEADER_SIZE;
    cart->header.lg_head = HEADER_SIZE - 14;
    cart->header.taille = cart->header.offset + ((3 * cart->header.largeur + cart->header.largeur % 4) * cart->header.hauteur);
    cart->header.applic_id[0] = 'T';
    cart->header.applic_id[1] = 'O';
    cart->header.applic_id[2] = 'N';
    cart->header.applic_id[3] = 'Y';
    cart->header.planes = 1;
    // Remplissage de pixels de la couleur passée en paramètre
    for (y = 0; y < CART_ROI_HAUT; y++)
    {
        for (x = 0; x < CART_ROI_LARG; x++)
        {
            cart->img_roi[y][x] = *col;
        }
    }
    // Dessiner le bord de 2 pixels
    for (x = 0; x < CART_ROI_LARG; x++)
    {
        for (y = 0; y < 2; y++)   // Haut
        {
            cart->img_roi[y][x] = *bord_col;
        }
        for (y = CART_ROI_HAUT - 2; y < CART_ROI_HAUT; y++)   // Bas
        {
            cart->img_roi[y][x] = *bord_col;
        }
    }
    for (y = 0; y < CART_ROI_HAUT; y++)
    {
        for (x = 0; x < 2; x++)   // Gauche
        {
            cart->img_roi[y][x] = *bord_col;
        }
        for (x = CART_ROI_LARG - 2; x < CART_ROI_LARG; x++)   // Droite
        {
            cart->img_roi[y][x] = *bord_col;
        }
    }
    // Initialisation du blason
    posX = CART_ROI_LARG / 2 - bla->header.largeur / 2; // Centre horizontalement
    posY = CART_ROI_HAUT / 4 - 20 ; // Un quart de hauteur pour le blason
    Ecrire_Blason_cart_roi(cart, bla, posX, posY); // Écrire le blason
    // Écrire le nom dans le cartouche
    Ecrire_Chaine_Cartouche_roi(cart, "Titre ROI ", POL_M, POL_0, 10, 130, NOIR, 9);
    Ecrire_Chaine_Cartouche_roi(cart, "Nom du bonhomme", POL_M, POL_0, 10, 110, NOIR, 9);
}
/****************************************************************************************
 * Initialiser un cartouche rectangulaire à une valeur constante.
 * Paramètres :
 *   - cart     : structure cartouche qui contiendra un blason, un nom et un titre.
 *   - bla      : image blason.
 *   - col      : pixel du cartouche.
 *   - bord_col : couleur de contours.
 ****************************************************************************************/
void Initialiser_Cartouche_roi(cart_roi *cart, blason *bla, blason *mask, pixel *col, pixel *bord_col, char zone[32], char nom[32], int in, int out)
{
    int x, y;
    int posX, posY;
    // Réinitialisation de la structure
    memset(cart, 0, sizeof(cart_roi));
    // Initialisation de l'en-tête du cartouche
    cart->type = CART_ROI;
    cart->ancre_x = CART_ROI_LARG/2;
    cart->ancre_y = CART_ROI_HAUT/2;
    cart->header.typ[0] = 'B';
    cart->header.typ[1] = 'M';
    // Déterminer les dimensions en fonction du type de cartouche
    cart->header.largeur = CART_ROI_LARG;
    cart->header.hauteur = CART_ROI_HAUT;

    cart->header.bitpix = BIT_PIXEL;
    cart->header.offset = HEADER_SIZE;
    cart->header.lg_head = HEADER_SIZE - 14;
    cart->header.taille = cart->header.offset + ((3 * cart->header.largeur + cart->header.largeur % 4) * cart->header.hauteur);
    cart->header.applic_id[0] = 'T';
    cart->header.applic_id[1] = 'O';
    cart->header.applic_id[2] = 'N';
    cart->header.applic_id[3] = 'Y';
    cart->header.planes = 1;
    // Remplissage de pixels de la couleur passée en paramètre
    for (y = 0; y < CART_ROI_HAUT; y++)
    {
        for (x = 0; x < CART_ROI_LARG; x++)
        {
            cart->img_roi[y][x] = *col;
        }
    }
    // Dessiner le bord de 2 pixels
    for (x = 0; x < CART_ROI_LARG; x++)
    {
        for (y = 0; y < 2; y++)   // Haut
        {
            cart->img_roi[y][x] = *bord_col;
        }
        for (y = CART_ROI_HAUT - 2; y < CART_ROI_HAUT; y++)   // Bas
        {
            cart->img_roi[y][x] = *bord_col;
        }
    }
    for (y = 0; y < CART_ROI_HAUT; y++)
    {
        for (x = 0; x < 2; x++)   // Gauche
        {
            cart->img_roi[y][x] = *bord_col;
        }
        for (x = CART_ROI_LARG - 2; x < CART_ROI_LARG; x++)   // Droite
        {
            cart->img_roi[y][x] = *bord_col;
        }
    }
    // Initialisation du blason
    posX = CART_ROI_LARG / 2 - bla->header.largeur / 2; // Centre horizontalement
    posY = CART_ROI_HAUT / 4 - 20 ; // Un quart de hauteur pour le blason
    Ecrire_Blason_cart_roi(cart, bla, posX, posY); // Écrire le blason
 // Concaténer et écrire les chaînes
    char buffer[64];
    // 1. Roi de + zone
    snprintf(buffer, sizeof(buffer), "ROI de %s", zone);
    Ecrire_Chaine_Cartouche_roi(cart, buffer, POL_M, POL_0, 32, CART_ROI_HAUT / 10 * 8 -20, NOIR, 8);
    // 2. Nom
    Ecrire_Chaine_Cartouche_roi(cart, nom, POL_M, POL_0, 85, CART_ROI_HAUT / 10 * 8 - 40, NOIR, 8);
    // 3. DE : + in
    snprintf(buffer, sizeof(buffer), "DE:%d", in);
    Ecrire_Chaine_Cartouche_roi(cart, buffer, POL_M, POL_0, 20, CART_ROI_HAUT / 10 * 8 - 60, NOIR, 8);
    // 4. A : + out
    snprintf(buffer, sizeof(buffer), "A:%d", out);
    Ecrire_Chaine_Cartouche_roi(cart, buffer, POL_M, POL_0, 145, CART_ROI_HAUT / 10 * 8 - 60, NOIR, 8);
}
/****************************************************************************************
 * Initialiser un cartouche duc à une valeur constante.
 * Paramètres :
 *   - cart     : structure cartouche qui contiendra un blason, un nom et un titre.
 *   - bla      : image blason.
 *   - col      : pixel du cartouche.
 *   - bord_col : couleur de contours.
 ****************************************************************************************/
void Initialiser_Cartouche_duc(cart_duc *cart, blason *bla, pixel *col, pixel *bord_col)
{
    int x, y;
    int posX, posY;
    // Réinitialisation de la structure
    memset(cart, 0, sizeof(cart_duc));
    // Initialisation de l'en-tête du cartouche
    cart->type = CART_ROI;
    cart->ancre_x = CART_DUC_LARG/2;
    cart->ancre_x = CART_DUC_HAUT/2;
    cart->header.typ[0] = 'B';
    cart->header.typ[1] = 'M';
    // Déterminer les dimensions en fonction du type de cartouche
    cart->header.largeur = CART_DUC_LARG;
    cart->header.hauteur = CART_DUC_HAUT;

    cart->header.bitpix  = BIT_PIXEL;
    cart->header.offset  = HEADER_SIZE;
    cart->header.lg_head = HEADER_SIZE - 14;
    cart->header.taille  = cart->header.offset + ((3 * cart->header.largeur + cart->header.largeur % 4) * cart->header.hauteur);
    cart->header.applic_id[0] = 'T';
    cart->header.applic_id[1] = 'O';
    cart->header.applic_id[2] = 'N';
    cart->header.applic_id[3] = 'Y';
    cart->header.planes = 1;
    // Remplissage de pixels de la couleur passée en paramètre
    for (y = 0; y < CART_DUC_HAUT; y++)
    {
        for (x = 0; x < CART_DUC_LARG; x++)
        {
            cart->img_duc[y][x] = *col;
        }
    }
    // Dessiner le bord de 2 pixels
    for (x = 0; x < CART_DUC_LARG; x++)
    {
        for (y = 0; y < 2; y++)   // Haut
        {
            cart->img_duc[y][x] = *bord_col;
        }
        for (y = CART_DUC_HAUT - 2; y < CART_DUC_HAUT; y++)   // Bas
        {
            cart->img_duc[y][x] = *bord_col;
        }
    }
    for (y = 0; y < CART_DUC_HAUT; y++)
    {
        for (x = 0; x < 2; x++)   // Gauche
        {
            cart->img_duc[y][x] = *bord_col;
        }
        for (x = CART_DUC_LARG - 2; x < CART_DUC_LARG; x++)   // Droite
        {
            cart->img_duc[y][x] = *bord_col;
        }
    }
    // Initialisation du blason
    posX = CART_DUC_LARG / 3 * 2 ; // Centre horizontalement
    posY = CART_DUC_HAUT / 4 - 10 ; // Un quart de hauteur pour le blason
    Ecrire_Blason_cart_duc(cart, bla, posX, posY); // Écrire le blason
    // Écrire le nom dans le cartouche
    Ecrire_Chaine_Cartouche_duc(cart, "Titre DUC ", POL_M, POL_0, 10, CART_DUC_HAUT/10*8, NOIR, 8);
    Ecrire_Chaine_Cartouche_duc(cart, "Nom du bonhomme", POL_M, POL_0, 10, 20, NOIR, 8);
    //Ecrire_Caractere_Cartouche_duc(cart,'T',2,0,10,10,NOIR);
}

/****************************************************************************************
 * Fonction pour dessiner un cartouche roi, centré sur les coordonnées spécifiées.
 *
 * parametre : img  Pointeur vers l'image principale dans laquelle le cartouche sera inséré.
 *                  Cette image doit déjà être initialisée et posséder des dimensions suffisantes
 *                  pour accueillir le cartouche aux coordonnées spécifiées.
 *             cart Pointeur vers la structure `cart_roi` représentant le cartouche à dessiner.
 *                  Cette structure doit contenir les pixels du cartouche roi.
 *             x_center  Position en x dans l'image principale correspondant au centre du cartouche.
 *             y_center  Position en y dans l'image principale correspondant au centre du cartouche.
 *
 * retour : aucun
 ***************************************************************************************/
void Ecrire_Cartouche_roi(image *img, cart_roi *cart, int x_center, int y_center)
{
    int x, y;
    int largeur = CART_ROI_LARG;
    int hauteur = CART_ROI_HAUT;
    // Calculer le coin supérieur gauche pour centrer le cartouche
    int x_offset = x_center - largeur / 2;
    int y_offset = y_center - hauteur / 2;

    // Vérification des limites de l'image
    if (x_offset < 0 || x_offset + largeur > img->header.largeur ||
        y_offset < 0 || y_offset + hauteur > img->header.hauteur)
    {
        printf("Erreur : Le cartouche dépasse les limites de l'image.\n");
        return;
    }
    // Mettre à jour les points d'ancrage lors du placement
    cart->ancre_x = x_center;  // Réassigner le point d'ancrage en x
    cart->ancre_y = y_center + CART_ROI_HAUT/2;  // Réassigner le point d'ancrage en y
    // Copier les pixels du cartouche dans l'image principale
    for (x = 0; x < largeur; x++)
    {
        for (y = 0; y < hauteur; y++)
        {
            Set_Pixel(img, x + x_offset, y + y_offset, &cart->img_roi[y][x]);
        }
    }
}

/****************************************************************************************
 * Fonction pour dessiner un cartouche duc, centré sur les coordonnées spécifiées.
 *
 * parametre : img       Pointeur vers l'image principale dans laquelle le cartouche sera inséré.
 *                       Cette image doit déjà être initialisée et posséder des dimensions suffisantes
 *                       pour accueillir le cartouche aux coordonnées spécifiées.
 *             cart      Pointeur vers la structure `cart_duc` représentant le cartouche à dessiner.
 *             x_center  Position en x dans l'image principale correspondant au centre du cartouche.
 *             y_center  Position en y dans l'image principale correspondant au centre du cartouche.
 *
 * retour : aucun
 ***************************************************************************************/
void Ecrire_Cartouche_duc(image *img, cart_duc *cart, int x_center, int y_center)
{
    int x, y;
    int largeur = CART_DUC_LARG;
    int hauteur = CART_DUC_HAUT;

    // Calculer le coin supérieur gauche pour centrer le cartouche
    int x_offset = x_center - largeur / 2;
    int y_offset = y_center - hauteur / 2;

    // Vérification des limites de l'image
    if (x_offset < 0 || x_offset + largeur > img->header.largeur ||
        y_offset < 0 || y_offset + hauteur > img->header.hauteur)
    {
        printf("Erreur : Le cartouche dépasse les limites de l'image.\n");
        return;
    }
        // Mettre à jour les points d'ancrage lors du placement
    cart->ancre_x = x_center;  // Réassigner le point d'ancrage en x
    cart->ancre_y = y_center + CART_DUC_HAUT/2;  // Réassigner le point d'ancrage en y

    // Copier les pixels du cartouche dans l'image principale
    for (x = 0; x < largeur; x++)
    {
        for (y = 0; y < hauteur; y++)
        {
            Set_Pixel(img, x + x_offset, y + y_offset, &cart->img_duc[y][x]);
        }
    }
}


/****************************************************************************************
 * Fonction pour mettre à jour les pixels dans le contexte d'un cartouche roi.
 *
 * parametre
 * img       Tableau de pixels représentant le cartouche dont les pixels doivent être mis à jour.
 *           La taille de ce tableau doit correspondre au type de cartouche utilisé.
 * x         Coordonnée x où le pixel doit être mis à jour.
 * y         Coordonnée y où le pixel doit être mis à jour.
 * src_pixel Pointeur vers le pixel source à copier dans le tableau d'image.
 * type      Type de cartouche (CART_ROI, CART_DUC, ou CART_EMP) pour ajuster les dimensions.
 ***************************************************************************************/
void Set_Pixel_Cartouche_roi(cart_roi *cart, int x, int y, pixel *src_pixel)
{
    int largeur, hauteur;
    largeur = CART_ROI_LARG;
    hauteur = CART_ROI_HAUT;
    // Vérifier les limites pour éviter les débordements
    if (x >= 0 && x < largeur && y >= 0 && y < hauteur)
    {
        cart->img_roi[y][x] = *src_pixel;

    }
    else
    {
        printf("Erreur : Coordonnées en dehors des limites de l'image du cartouche.\n");
    }
}

/****************************************************************************************
 * Fonction pour mettre à jour les pixels dans le contexte d'un cartouche duc.
 *
 * parametre
 * img       Tableau de pixels représentant le cartouche dont les pixels doivent être mis à jour.
 *           La taille de ce tableau doit correspondre au type de cartouche utilisé.
 * x         Coordonnée x où le pixel doit être mis à jour.
 * y         Coordonnée y où le pixel doit être mis à jour.
 * src_pixel Pointeur vers le pixel source à copier dans le tableau d'image.
 ***************************************************************************************/
void Set_Pixel_Cartouche_duc(cart_duc *cart, int x, int y, pixel *src_pixel)
{
    int largeur, hauteur;
    largeur = CART_DUC_LARG;
    hauteur = CART_DUC_HAUT;
    // Vérifier les limites pour éviter les débordements
    if (x >= 0 && x < largeur && y >= 0 && y < hauteur)
    {
        cart->img_duc[y][x] = *src_pixel;
    }
    else
    {
        printf("Erreur : Coordonnées en dehors des limites de l'image du cartouche.\n");
    }
}

/****************************************************************************************
 * Initialiser un cartouche empereur à une valeur constante en forme elliptique.
 * Paramètres :
 *   - cart     : structure cartouche qui contiendra un blason, un nom et un titre.
 *   - bla      : image blason.
 *   - mask     : masque pour le blason.
 *   - col      : pixel de couleur du cartouche.
 *   - bord_col : couleur de la bordure.
 *   - zone     : zone du cartouche.
 *   - nom      : nom de l'empereur.
 *   - in       : valeur de l'entrée pour le cartouche.
 *   - out      : valeur de la sortie pour le cartouche.
 ****************************************************************************************/
void Initialiser_Cartouche_emp(cart_emp *cart, blason *bla, blason *mask, pixel *col, pixel *bord_col, char zone[32], char nom[32], int in, int out)
{
    int x, y;
    int posX, posY;
    memset(cart, 0, sizeof(cart_emp));
    cart->type = CART_EMP;
    cart->ancre_x = CART_EMP_LARG / 2;
    cart->ancre_y = CART_EMP_HAUT / 2; // Correction ici (était cart->ancre_x)
    cart->header.typ[0] = 'B';
    cart->header.typ[1] = 'M';
    // Dimensions pour un cartouche empereur elliptique
    cart->header.largeur = CART_EMP_LARG;
    cart->header.hauteur = CART_EMP_HAUT;
    cart->header.bitpix = BIT_PIXEL;
    cart->header.offset = HEADER_SIZE;
    cart->header.lg_head = HEADER_SIZE - 14;
    cart->header.taille = cart->header.offset + ((3 * cart->header.largeur + cart->header.largeur % 4) * cart->header.hauteur);
    cart->header.applic_id[0] = 'T';
    cart->header.applic_id[1] = 'O';
    cart->header.applic_id[2] = 'N';
    cart->header.applic_id[3] = 'Y';
    cart->header.planes = 1;
    // Dessiner les pixels de fond et la bordure en forme elliptique
    int centerX = CART_EMP_LARG / 2;
    int centerY = CART_EMP_HAUT / 2;
    int a = centerX - 2; // Rayon horizontal (en excluant la bordure)
    int b = centerY - 2; // Rayon vertical (en excluant la bordure)
    for (y = 0; y < CART_EMP_HAUT; y++)
    {
        for (x = 0; x < CART_EMP_LARG; x++)
        {
            // Équation de l'ellipse : (x - centerX)^2 / a^2 + (y - centerY)^2 / b^2 <= 1
            if (((x - centerX) * (x - centerX)) * b * b + ((y - centerY) * (y - centerY)) * a * a <= a * a * b * b)
            {
                cart->img_emp[y][x] = *col; // Remplissage de couleur
            }
        }
    }
    // Bordure elliptique de 2 pixels
    for (y = 0; y < CART_EMP_HAUT; y++)
    {
        for (x = 0; x < CART_EMP_LARG; x++)
        {
            if (((x - centerX) * (x - centerX)) * b * b + ((y - centerY) * (y - centerY)) * a * a <= a * a * b * b &&
                ((x - centerX) * (x - centerX)) * (b - 2) * (b - 2) + ((y - centerY) * (y - centerY)) * (a - 2) * (a - 2) > (a - 2) * (a - 2) * (b - 2) * (b - 2))
            {
                cart->img_emp[y][x] = *bord_col;
            }
        }
    }
    // Positionner le blason au centre
    posX = CART_EMP_LARG / 2 - bla->header.largeur / 2; // Centre horizontalement
    posY = CART_EMP_HAUT / 4 - 20;
    Ecrire_Blason_cart_emp(cart, bla, mask, posX, posY);

    // Concaténer et écrire les chaînes
    char buffer[64];
    // 1. EMPEREUR de + zone
    snprintf(buffer, sizeof(buffer), "EMPEREUR de %s", zone);
    Ecrire_Chaine_Cartouche_emp(cart, buffer, POL_M, POL_0, 32, CART_EMP_HAUT / 10 * 8 -20, NOIR, 8);
    // 2. Nom
    Ecrire_Chaine_Cartouche_emp(cart, nom, POL_M, POL_0, 85, CART_EMP_HAUT / 10 * 8 - 40, NOIR, 8);
    // 3. DE : + in
    snprintf(buffer, sizeof(buffer), "DE:%d", in);
    Ecrire_Chaine_Cartouche_emp(cart, buffer, POL_M, POL_0, 20, CART_EMP_HAUT / 10 * 8 - 60, NOIR, 8);
    // 4. A : + out
    snprintf(buffer, sizeof(buffer), "A:%d", out);
    Ecrire_Chaine_Cartouche_emp(cart, buffer, POL_M, POL_0, 145, CART_EMP_HAUT / 10 * 8 - 60, NOIR, 8);
}


/****************************************************************************************
 * Fonction pour dessiner un cartouche empereur, centré sur les coordonnées spécifiées.
 *
 * parametre : img       Pointeur vers l'image principale dans laquelle le cartouche sera inséré.
 *                       Cette image doit déjà être initialisée et posséder des dimensions suffisantes
 *                       pour accueillir le cartouche aux coordonnées spécifiées.
 *             cart      Pointeur vers la structure `cart_emp` représentant le cartouche à dessiner.
 *             x_center  Position en x dans l'image principale correspondant au centre du cartouche.
 *             y_center  Position en y dans l'image principale correspondant au centre du cartouche.
 *
 * retour : aucun
 ***************************************************************************************/
void Ecrire_Cartouche_emp(image *img, cart_emp *cart, int x_center, int y_center)
{
    int x, y;
    int centerX = CART_EMP_LARG / 2;
    int centerY = CART_EMP_HAUT / 2;
    int a = centerX - 2; // Rayon horizontal de l'ellipse
    int b = centerY - 2; // Rayon vertical de l'ellipse

    // Calculer le coin supérieur gauche pour centrer le cartouche
    int x_offset = x_center - centerX;
    int y_offset = y_center - centerY;

    // Vérification des limites de l'image
    if (x_offset < 0 || x_offset + CART_EMP_LARG > img->header.largeur ||
        y_offset < 0 || y_offset + CART_EMP_HAUT > img->header.hauteur)
    {
        printf("Erreur : Le cartouche dépasse les limites de l'image.\n");
        return;
    }
    // Mettre à jour les points d'ancrage lors du placement
    cart->ancre_x = x_center;  // Réassigner le point d'ancrage en x
    cart->ancre_y = y_center;  // Réassigner le point d'ancrage en y


    // Copier les pixels du cartouche dans l'image principale en respectant la forme elliptique
    for (x = 0; x < CART_EMP_LARG; x++)
    {
        for (y = 0; y < CART_EMP_HAUT; y++)
        {
            // Vérifier si le point (x, y) se trouve dans l'ellipse centrée
            if (((x - centerX) * (x - centerX)) * b * b + ((y - centerY) * (y - centerY)) * a * a <= a * a * b * b)
            {
                Set_Pixel(img, x + x_offset, y + y_offset, &cart->img_emp[y][x]);
            }
        }
    }
}

/****************************************************************************************
 * Ecrire un blason empereur sur le cartouche image de forme elliptique en appliquant un masque.
 * Paramètres :
 *   - cart : Structure cartouche empereur dans laquelle le blason sera écrit.
 *   - bla  : Structure blason contenant l'image et le masque du blason.
 *   - posX : Position x du blason dans l'image du cartouche.
 *   - posY : Position y du blason dans l'image du cartouche.
 ****************************************************************************************/
void Ecrire_Blason_cart_emp(cart_emp *cart, blason *bla, blason *mask, int posX, int posY)
{
    int largeur_Blason = bla->header.largeur;
    int hauteur_Blason = bla->header.hauteur;
    int centerX = CART_EMP_LARG / 2;
    int centerY = CART_EMP_HAUT / 2;
    int a = centerX - 2; // Rayon horizontal pour l'ellipse du cartouche empereur
    int b = centerY - 2; // Rayon vertical pour l'ellipse du cartouche empereur

    // Vérification des limites du cartouche pour le blason
    if (posX < 0 || posX + largeur_Blason > cart->header.largeur ||
        posY < 0 || posY + hauteur_Blason > cart->header.hauteur)
    {
        printf("Erreur : Le blason dépasse les limites de l'image du cartouche.\n");
        return;
    }

    // Parcourir tous les pixels du blason
    for (int y = 0; y < hauteur_Blason; y++)
    {
        for (int x = 0; x < largeur_Blason; x++)
        {
            int cartX = posX + x;
            int cartY = posY + y;

            // Vérifier si le point (cartX, cartY) se trouve dans l'ellipse du cartouche empereur
            if (((cartX - centerX) * (cartX - centerX)) * b * b + ((cartY - centerY) * (cartY - centerY)) * a * a <= a * a * b * b)
            {
//                // Appliquer le masque : ne copier que si le masque n'est pas noir
//                if (!(mask->img[y][x].B == 0 && mask->img[y][x].G == 0 && mask->img[y][x].R == 0))
//                {
//                    Set_Pixel_Cartouche_emp(cart, cartX, cartY, &bla->img[y][x]);
//                }
                            // Vérifier si le pixel du masque est noir
                if (mask->img[y][x].B == 0 && mask->img[y][x].G == 0 && mask->img[y][x].R == 0) {
                // Ne pas copier ce pixel
                continue;
                }
                 Set_Pixel_Cartouche_emp(cart, cartX, cartY, &bla->img[y][x]);
            }
        }
    }
}

/**
 * Fonction pour mettre à jour les pixels dans le contexte d'un cartouche empereur.
 *
 * parametre
 * cart      : structure cart_emp représentant le cartouche dont les pixels doivent être mis à jour.
 * x         : Coordonnée x où le pixel doit être mis à jour.
 * y         : Coordonnée y où le pixel doit être mis à jour.
 * src_pixel : Pointeur vers le pixel source à copier dans le tableau d'image.
 */
void Set_Pixel_Cartouche_emp(cart_emp *cart, int x, int y, pixel *src_pixel)
{
    int largeur, hauteur;
    largeur = CART_EMP_LARG;
    hauteur = CART_EMP_HAUT;

    // Vérifier les limites pour éviter les débordements
    if (x >= 0 && x < largeur && y >= 0 && y < hauteur)
    {
        cart->img_emp[y][x] = *src_pixel;
    }
    else
    {
        printf("Erreur : Coordonnées en dehors des limites de l'image du cartouche.\n");
    }
}

/*****************************************************************************************
 * Fonction pour positionner une image sur une image source à une position donnée.
 *
 * Cette fonction copie les pixels de l'image à insérer dans l'image source, en partant
 * des coordonnées spécifiées. Les pixels dépassant les bords de l'image source sont ignorés.
 *
 * param source          Pointeur vers l'image source (l'image de fond).
 * param inseree         Pointeur vers l'image à insérer (l'image à superposer).
 * param pos_x           Coordonnée x où l'image insérée commence dans l'image source.
 * param pos_y           Coordonnée y où l'image insérée commence dans l'image source.
 *
 * retour                Cette fonction ne retourne rien. Elle modifie directement l'image source.
 ****************************************************************************************/
void Positionner_Image(image *source, image *inseree, int pos_x, int pos_y)
{
    // Vérifier que les deux images existent
    if (!source || !inseree) {
        printf("Erreur : L'une des images est NULL.\n");
        return;
    }

    // Parcourir chaque pixel de l'image à insérer
    for (int i = 0; i < inseree->header.hauteur; i++) {
        for (int j = 0; j < inseree->header.largeur; j++) {
            // Calculer la position du pixel dans l'image source
            int dest_x = pos_x + j;
            int dest_y = pos_y + i;

            // Vérifier que le pixel se trouve à l'intérieur des limites de l'image source
            if (dest_x >= 0 && dest_x < source->header.largeur &&
                dest_y >= 0 && dest_y < source->header.hauteur) {
                // Copier le pixel dans l'image source
                source->pic[dest_y][dest_x] = inseree->pic[i][j];
                //Set_Pixel(source, dest_x , dest_y , inseree->pic);
            }
        }
    }
}


