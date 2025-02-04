
/****************************************************************************************
* Generation d'images .bmp 24 bits
*
* Gestion des polices de caracteres
*
* Programmation procedurale 2024 - A.Umina - image blason version 0
****************************************************************************************/
#include "DB_Main.h"

// Jeu de 3 polices : Small Medium Big : police[taille][ascii][hauteur_max][largeur_max]
uchar police[3][256][18][10];

// Taille des polices : [hauteur][largeur]
int pdim[3][2]= {{9,5},{11,7},{18,10}};

// Code ascii des caracteres speciaux utilises
int spec[10]= {126,58,45,46,43,33,47,42,39,0};

// Nr de caracteres speciaux utilises
int nspec=9;

/****************************************************************************************
* Charge le tableau des polices de caractères a partir du fichier Police_Triple.bmp
****************************************************************************************/
void Generer_Police()
{
    int i=0, pt=1;

    image *car=NULL;

    car=Lire_Image("Police","_Triple");

    //Afficher_Header(car);

    memset(police, 0, 3*256*18*10);

    for (i='A'; i<='Z'; i++)
    {
        Read_Car_Pol(car, i, POL_S, pt);
        Read_Car_Pol(car, i, POL_M, pt);
        Read_Car_Pol(car, i, POL_B, pt);
        pt=pt+10+2;
    }

    for (i='0'; i<='9'; i++)
    {
        Read_Car_Pol(car, i, POL_S, pt);
        Read_Car_Pol(car, i, POL_M, pt);
        Read_Car_Pol(car, i, POL_B, pt);
        pt=pt+10+2;
    }

    for (i='a'; i<='z'; i++)
    {
        Read_Car_Pol(car, i, POL_S, pt);
        Read_Car_Pol(car, i, POL_M, pt);
        Read_Car_Pol(car, i, POL_B, pt);
        pt=pt+10+2;
    }

    for (i=0; i<nspec; i++)
    {
        Read_Car_Pol(car, spec[i], POL_S, pt);
        Read_Car_Pol(car, spec[i], POL_M, pt);
        Read_Car_Pol(car, spec[i], POL_B, pt);
        pt=pt+10+2;
    }

    Free_Image(car);

    return;
}

/****************************************************************************************
* Charge un caractere dans la cellule correspondante du tableau police depuis l'image
*   car : pointeur vers l'image Police_Triple.bmp
*   c   : code ascii du caractere = indice 2 du tableau police
*   sz  : taille de la police = indice 1 du tableau police
*   pt  : position x courante dans l'image
****************************************************************************************/
void Read_Car_Pol(image *car, int c, int sz, int pt)
{
    int i, j, ht;
    pixel *p;

    if (sz==POL_S) ht=1;        // Position y dans images source a charger
    if (sz==POL_M) ht=13;
    if (sz==POL_B) ht=25;

    for (i=0; i<pdim[sz][0]; i++)
    {
        for (j=0; j<pdim[2][1]; j++)
        {
            p = Get_Pixel(car, j+pt, i+ht);
            police[sz][c][i][j] = p->B;
        }
    }

    return;
}

/****************************************************************************************
* Ecrire un caractere a l'adresse x,y
*   img : pointeur vers l'image dans laquelle on insert le caractere
*   c   : code ascii du caractere
*   sz  : taille de la police : voir enum pol_size
*   ori : orientation de la police : voir enum pol_ori
*   x,y : coordonnées d'insertion dans l'image
*   col : couleur du caractere : voir enum couleur
****************************************************************************************/
void Ecrire_Caractere(image *img, int c, int sz, int ori, int x, int y, int col)
{
    int i, j, di, dj;
    pixel *pix;

    // Récupérer la couleur sous forme de structure pixel
    pix = Get_Col(col);

    // Vérifie que le code ASCII est dans la plage des caractères disponibles dans la police
    if (c < 0 || c >= 128)
    {
        printf("Code ASCII non valide: %d\n", c);
        return;
    }

    for (i = 0; i < pdim[sz][0]; i++)
    {
        for (j = 0; j < pdim[sz][1]; j++)
        {
            switch (ori)
            {
            case POL_0:
                di = x + j;
                dj = y + i;
                break;
            case POL_90:
                di = x - i;
                dj = y + j;
                break;
            case POL_180:
                di = x - j;
                dj = y - i;
                break;
            case POL_270:
                di = x + i;
                dj = y - j;
                break;
            default:
                continue; // Ignore si l'orientation est incorrecte
            }

            // Vérifiez si les coordonnées (di, dj) sont dans les limites de l'image
            if (di >= 0 && di < img->header.largeur && dj >= 0 && dj < img->header.hauteur)
            {
                if (police[sz][c][i][j] == 255) Set_Pixel(img, di, dj, pix);

            }
        }
    }


    return;
}
/****************************************************************************************
* Ecrire un caractere dans le cartouche a l'adresse x, y
*   cart : pointeur vers le cartouche dans lequel on insère le caractere
*   asc    : code ascii du caractere
*   sz   : taille de la police : voir enum pol_size
*   ori  : orientation de la police : voir enum pol_ori
*   x, y : coordonnées d'insertion dans le cartouche
*   col  : couleur du caractere : voir enum couleur
****************************************************************************************/
void Ecrire_Caractere_Cartouche_roi(cart_roi *cart, int asc, int sz, int ori, int x, int y, int col)
{
    int i, j, di, dj;
    pixel *pix;
    // Récupérer la couleur sous forme de structure pixel
    pix = Get_Col(col);
    // Vérifie que le code ASCII est dans la plage des caractères disponibles dans la police
    if (asc < 0 || asc >= 128)
    {
        printf("Code ASCII non valide: %d\n", asc);
        return;
    }
    for (i = 0; i < pdim[sz][0]; i++)
    {
        for (j = 0; j < pdim[sz][1]; j++)
        {
            switch (ori)
            {
            case POL_0:
                di = x + j;
                dj = y + i;
                break;
            case POL_90:
                di = x - i;
                dj = y + j;
                break;
            case POL_180:
                di = x - j;
                dj = y - i;
                break;
            case POL_270:
                di = x + i;
                dj = y - j;
                break;
            default:
                continue; // Ignore si l'orientation est incorrecte
            }

            // Vérifiez si les coordonnées (di, dj) sont dans les limites du cartouche
            if (di >= 0 && di < CART_ROI_LARG && dj >= 0 && dj < CART_ROI_HAUT)
            {
                if (police[sz][asc][i][j] == 255) {
                    cart->img_roi[dj][di] = *pix; // Accès direct au tableau statique de `cart_rect`
                }
            }
        }
    }
}
/****************************************************************************************
 * Écrire un caractère dans le cartouche à l'adresse x, y.
 * Paramètres :
 *   - cart : pointeur vers le cartouche dans lequel on insère le caractère.
 *   - c    : code ASCII du caractère.
 *   - sz   : taille de la police : voir enum pol_size.
 *   - ori  : orientation de la police : voir enum pol_ori.
 *   - x, y : coordonnées d'insertion dans le cartouche.
 *   - col  : couleur du caractère : voir enum couleur.
 ****************************************************************************************/
void Ecrire_Caractere_Cartouche_duc(cart_duc *cart, int c, int sz, int ori, int x, int y, int col)
{
    int i, j, di, dj;
    pixel *pix = Get_Col(col);

    // Vérifie que le code ASCII est dans la plage des caractères disponibles dans la police
    if (c < 0 || c >= 128)
    {
        printf("Code ASCII non valide: %d\n", c);
        return;
    }

    for (i = 0; i < pdim[sz][0]; i++)
    {
        for (j = 0; j < pdim[sz][1]; j++)
        {
            // Déterminer les coordonnées selon l'orientation
            switch (ori)
            {
            case POL_0:
                di = x + j;
                dj = y + i;
                break;
            case POL_90:
                di = x - i;
                dj = y + j;
                break;
            case POL_180:
                di = x - j;
                dj = y - i;
                break;
            case POL_270:
                di = x + i;
                dj = y - j;
                break;
            default:
                continue; // Ignore si l'orientation est incorrecte
            }

            // Vérifiez si les coordonnées (di, dj) sont dans les limites du cartouche
            if (di >= 0 && di < CART_DUC_LARG && dj >= 0 && dj < CART_DUC_HAUT)
            {
                if (police[sz][c][i][j] == 255)
                {
                    cart->img_duc[dj][di] = *pix;
                }
            }
        }
    }
}

/****************************************************************************************
* Ecrire une chaîne de caractères dans l'image avec orientation individuelle
*   img       : structure image
*   txt       : chaîne de caractères à écrire
*   police    : taille de la police (e.g., POL_S, POL_M, ...)
*   angle     : angle de rotation pour chaque caractère (e.g., POL_0, POL_90, ...)
*   x         : position x de départ pour le premier caractère
*   y         : position y de départ pour le premier caractère
*   couleur   : couleur du texte
*   pas       : espacement entre les caractères, ajusté selon l'angle
****************************************************************************************/
void Ecrire_Chaine(image *img, char *txt, int police, int angle, int x, int y, int couleur, int pas)
{
    int i;
    int dx = 0, dy = 0; // Déplacements en fonction de l'angle

    // Calcul des déplacements horizontaux et verticaux en fonction de l'angle
    switch (angle)
    {
    case POL_0:
        dx = pas;
        dy = 0;
        break;         // Horizontale droite
    case POL_90:
        dx = 0;
        dy = pas;
        break;         // Verticale descendante
    case POL_180:
        dx = -pas;
        dy = 0;
        break;        // Horizontale gauche
    case POL_270:
        dx = 0;
        dy = -pas;
        break;        // Verticale montante
    default:
        dx = pas;
        dy = 0;
        break;         // Par défaut horizontale droite
    }

    for (i = 0; txt[i] != '\0'; i++)
    {
        // Écrire chaque caractère en appliquant l'angle choisi
        Ecrire_Caractere(img, txt[i], police, angle, x + i * dx, y + i * dy, couleur);
    }
}
/****************************************************************************************
 * Écrire une chaîne de caractères dans le cartouche avec orientation individuelle.
 * Paramètres :
 *   - cart    : structure cart_rect_gen (cartouche dans lequel écrire).
 *   - txt     : chaîne de caractères à écrire.
 *   - police   : taille de la police (e.g., POL_S, POL_M, ...).
 *   - angle   : angle de rotation pour chaque caractère (e.g., POL_0, POL_90, ...).
 *   - x       : position x de départ pour le premier caractère.
 *   - y       : position y de départ pour le premier caractère.
 *   - couleur  : couleur du texte.
 *   - pas     : espacement entre les caractères, ajusté selon l'angle.
 ****************************************************************************************/
void Ecrire_Chaine_Cartouche_roi(cart_roi *cart, char *txt, int police, int angle, int x, int y, int couleur, int pas)
{
    int i;
    int dx = 0, dy = 0; // Déplacements en fonction de l'angle

    // Calcul des déplacements horizontaux et verticaux en fonction de l'angle
    switch (angle)
    {
    case POL_0:
        dx = pas;
        dy = 0;
        break;         // Horizontale droite
    case POL_90:
        dx = 0;
        dy = pas;
        break;         // Verticale descendante
    case POL_180:
        dx = -pas;
        dy = 0;
        break;        // Horizontale gauche
    case POL_270:
        dx = 0;
        dy = -pas;
        break;        // Verticale montante
    default:
        dx = pas;
        dy = 0;
        break;         // Par défaut horizontale droite
    }

    for (i = 0; txt[i] != '\0'; i++)
    {
        // Écrire chaque caractère en appliquant l'angle choisi
        Ecrire_Caractere_Cartouche_roi(cart, txt[i], police, angle, x + i * dx, y + i * dy, couleur);
    }
}

/****************************************************************************************
    * Écrire une chaîne de caractères dans le cartouche avec orientation individuelle.
    * Paramètres :
    *   - cart    : structure cart_rect_gen (cartouche dans lequel écrire).
    *   - txt     : chaîne de caractères à écrire.
    *   - police   : taille de la police (e.g., POL_S, POL_M, ...).
    *   - angle   : angle de rotation pour chaque caractère (e.g., POL_0, POL_90, ...).
    *   - x       : position x de départ pour le premier caractère.
    *   - y       : position y de départ pour le premier caractère.
    *   - couleur  : couleur du texte.
    *   - pas     : espacement entre les caractères, ajusté selon l'angle.
    ****************************************************************************************/
void Ecrire_Chaine_Cartouche_duc(cart_duc *cart, char *txt, int police, int angle, int x, int y, int couleur, int pas)
{
    int i;
    int dx = 0, dy = 0; // Déplacements en fonction de l'angle
    // Calcul des déplacements horizontaux et verticaux en fonction de l'angle
    switch (angle)
    {
    case POL_0:
        dx = pas;
        dy = 0;
        break;         // Horizontale droite
    case POL_90:
        dx = 0;
        dy = pas;
        break;         // Verticale descendante
    case POL_180:
        dx = -pas;
        dy = 0;
        break;        // Horizontale gauche
    case POL_270:
        dx = 0;
        dy = -pas;
        break;        // Verticale montante
    default:
        dx = pas;
        dy = 0;
        break;         // Par défaut horizontale droite
    }

    for (i = 0; txt[i] != '\0'; i++)
    {
        Ecrire_Caractere_Cartouche_duc(cart, txt[i], police, angle, x + i * dx, y + i * dy, couleur);
    }
}

/****************************************************************************************
 * Écrire un caractère dans le cartouche empereur de forme elliptique.
 * Paramètres :
 *   - cart : pointeur vers le cartouche empereur dans lequel on insère le caractère.
 *   - c    : code ASCII du caractère.
 *   - sz   : taille de la police : voir enum pol_size.
 *   - ori  : orientation de la police : voir enum pol_ori.
 *   - x, y : coordonnées d'insertion dans le cartouche.
 *   - col  : couleur du caractère : voir enum couleur.
 ****************************************************************************************/
void Ecrire_Caractere_Cartouche_emp(cart_emp *cart, int c, int sz, int ori, int x, int y, int col)
{
    int i, j, di, dj;
    pixel *pix = Get_Col(col);
    int centerX = CART_EMP_LARG / 2;
    int centerY = CART_EMP_HAUT / 2;
    int a = centerX - 2; // Rayon horizontal pour l'ellipse
    int b = centerY - 2; // Rayon vertical pour l'ellipse

    // Vérifie que le code ASCII est dans la plage des caractères disponibles dans la police
    if (c < 0 || c >= 128)
    {
        printf("Code ASCII non valide: %d\n", c);
        return;
    }

    for (i = 0; i < pdim[sz][0]; i++)
    {
        for (j = 0; j < pdim[sz][1]; j++)
        {
            // Déterminer les coordonnées selon l'orientation
            switch (ori)
            {
            case POL_0:
                di = x + j;
                dj = y + i;
                break;
            case POL_90:
                di = x - i;
                dj = y + j;
                break;
            case POL_180:
                di = x - j;
                dj = y - i;
                break;
            case POL_270:
                di = x + i;
                dj = y - j;
                break;
            default:
                continue; // Ignore si l'orientation est incorrecte
            }

            // Vérifiez si les coordonnées (di, dj) sont dans les limites du cartouche et de l'ellipse
            if (di >= 0 && di < CART_EMP_LARG && dj >= 0 && dj < CART_EMP_HAUT)
            {
                // Vérification de la condition pour rester dans l'ellipse
                if (((di - centerX) * (di - centerX)) * b * b + ((dj - centerY) * (dj - centerY)) * a * a <= a * a * b * b)
                {
                    // Appliquer la couleur du pixel si le point fait partie du caractère
                    if (police[sz][c][i][j] == 255)
                    {
                        cart->img_emp[dj][di] = *pix;
                    }
                }
            }
        }
    }
}


/****************************************************************************************
 * Écrire une chaîne de caractères dans le cartouche empereur de forme elliptique
 * avec orientation individuelle.
 * Paramètres :
 *   - cart    : structure cart_emp (cartouche empereur dans lequel écrire).
 *   - txt     : chaîne de caractères à écrire.
 *   - police  : taille de la police (e.g., POL_S, POL_M, ...).
 *   - angle   : angle de rotation pour chaque caractère (e.g., POL_0, POL_90, ...).
 *   - x       : position x de départ pour le premier caractère.
 *   - y       : position y de départ pour le premier caractère.
 *   - couleur : couleur du texte.
 *   - pas     : espacement entre les caractères, ajusté selon l'angle.
 ****************************************************************************************/
void Ecrire_Chaine_Cartouche_emp(cart_emp *cart, char *txt, int police, int angle, int x, int y, int couleur, int pas)
{
    int i;
    int dx = 0, dy = 0; // Déplacements en fonction de l'angle
    int centerX = CART_EMP_LARG / 2;
    int centerY = CART_EMP_HAUT / 2;
    int a = centerX - 2; // Rayon horizontal pour l'ellipse
    int b = centerY - 2; // Rayon vertical pour l'ellipse

    // Calcul des déplacements horizontaux et verticaux en fonction de l'angle
    switch (angle)
    {
    case POL_0:
        dx = pas;
        dy = 0;
        break;         // Horizontale droite
    case POL_90:
        dx = 0;
        dy = pas;
        break;         // Verticale descendante
    case POL_180:
        dx = -pas;
        dy = 0;
        break;        // Horizontale gauche
    case POL_270:
        dx = 0;
        dy = -pas;
        break;        // Verticale montante
    default:
        dx = pas;
        dy = 0;
        break;         // Par défaut horizontale droite
    }

    for (i = 0; txt[i] != '\0'; i++)
    {
        int char_x = x + i * dx;
        int char_y = y + i * dy;

        // Vérification si les coordonnées sont dans les limites elliptiques du cartouche empereur
        if (((char_x - centerX) * (char_x - centerX)) * b * b + ((char_y - centerY) * (char_y - centerY)) * a * a <= a * a * b * b)
        {
            Ecrire_Caractere_Cartouche_emp(cart, txt[i], police, angle, char_x, char_y, couleur);
        }
        else
        {
            printf("Position du caractère en dehors des limites elliptiques du cartouche empereur.\n");
        }
    }
}

