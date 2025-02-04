
/****************************************************************************************
* Generation d'images .bmp 24 bits
*
* Implementation des fonctions systemes via tableau a 2 dimensions :
*   lecture/ecriture sur disque,
*   allocation dynamique, affichage
*
* Programmation procedurale 2024 - A.Umina - image blason version 0
****************************************************************************************/
#include "Util_Bitmap.h"
#include <time.h>
#include <sys/stat.h>   // Pour struct stat
#include <sys/types.h>

/****************************************************************************************
* Definition du tableau palette de couleurs
****************************************************************************************/
pixel color[60] =
{
    {   0,   0,   0 },  // 000 Noir
    { 255, 255, 255 },  // 001 Blanc
    { 255,   0,   0 },  // 002 Bleu
    {   0, 255,   0 },  // 003 Vert
    {   0,   0, 255 },  // 004 Rouge
    {   0, 255, 255 },  // 005 Jaune
    { 255,   0, 255 },  // 006 Magenta
    { 255, 255,   0 },  // 007 Cyan
    {  32,  32,  32 },  // 008 Gris_x
    {  64,  64,  64 },  // 009
    {  96,  96,  96 },  // 010
    { 128, 128, 128 },  // 011
    { 160, 160, 160 },  // 012
    { 192, 192, 192 },  // 013
    { 224, 224, 224 },  // 014
    { 255,  32,  32 },  // 015 Bleu_x
    { 255,  64,  64 },  // 016
    { 255,  96,  96 },  // 017
    { 255, 128, 128 },  // 018
    { 255, 160, 160 },  // 019
    { 255, 192, 192 },  // 020
    { 255, 224, 224 },  // 021
    {  32, 255,  32 },  // 022 Vert_x
    {  64, 255,  64 },  // 023
    {  96, 255,  96 },  // 024
    { 128, 255, 128 },  // 025
    { 160, 255, 160 },  // 026
    { 192, 255, 192 },  // 027
    { 224, 255, 224 },  // 028
    {  32,  32, 255 },  // 029 Rouge_x
    {  64,  64, 255 },  // 030
    {  96,  96, 255 },  // 031
    { 128, 128, 255 },  // 032
    { 160, 160, 255 },  // 033
    { 192, 192, 255 },  // 034
    { 224, 224, 255 },  // 035
    {  32, 255, 255 },  // 036 Jaune_x
    {  64, 255, 255 },  // 037
    {  96, 255, 255 },  // 038
    { 128, 255, 255 },  // 039
    { 160, 255, 255 },  // 040
    { 192, 255, 255 },  // 041
    { 224, 255, 255 },  // 042
    { 255,  32, 255 },  // 043 Magenta_x
    { 255,  64, 255 },  // 044
    { 255,  96, 255 },  // 045
    { 255, 128, 255 },  // 046
    { 255, 160, 255 },  // 047
    { 255, 192, 255 },  // 048
    { 255, 224, 255 },  // 049
    { 255, 255,  32 },  // 050 Cyan_x
    { 255, 255,  64 },  // 051
    { 255, 255,  96 },  // 052
    { 255, 255, 128 },  // 053
    { 255, 255, 160 },  // 054
    { 255, 255, 192 },  // 055
    { 255, 255, 224 },  // 056
    {   0,   0,   0 },  // 057
    {   0,   0,   0 },  // 058
    {   0,   0,   0 }   // 059
};


/****************************************************************************************
* Donner une couleur de base sur 8 niveaux possibles
*   col     : couleur definie dans enum couleur
****************************************************************************************/
pixel * Get_Col(int col)
{
    return &color[col];
}

/****************************************************************************************
* Afficher les donnees du fichier BMP 24bits original
*   img     : structure image
****************************************************************************************/
void Afficher_Header(image *img)
{
    printf("\n--------------------------------------------------------\n");
    printf("%-12s: %s  \n", "Image", img->nom_base);
    printf("%-12s: %d  \n","Largeur", img->header.largeur);
    printf("%-12s: %d  \n","Hauteur", img->header.hauteur);
    printf("%-12s: %d Kbytes \n","Taille", img->header.taille/1024);
    printf("\n");

    return;
}



/****************************************************************************************
* Composition du nom de fichier complet
* Exemple : si le nom de base est Test et l'extension _V1 : .\Test\Test_V1.bmp
*   nom     : nom compose
*   nom_bas : nom de base de l'image
*   nom_tag : suffixe du nom d'image
****************************************************************************************/
void Nom_Image(char * nom, char * nom_base, char * nom_tag)
{
    strncpy(nom, PHOTO_DIR,     FIC_NM);   // Repertoire de base, harcode via define ds racine
    strncat(nom, nom_base,      FIC_NM);   // Repertoire image = Nom de base
    strncat(nom, "\\",          FIC_NM);
    strncat(nom, nom_base,      FIC_NM);   // Nom de base de l'image
    strncat(nom, nom_tag,       FIC_NM);   // Extension accrochee au nom de base
    strncat(nom, ".bmp",        FIC_NM);   // Type de fichier
    printf("Fichier : %s\n\n", nom);

    return;
}

/****************************************************************************************
* Lire un fichier BMP 24bits original en memoire
*   nom_bas : nom de base de l'image
*   nom_tag : suffixe du nom d'image
*   retourne le pointeur vers l'image allouee
****************************************************************************************/
image * Lire_Image(char * nom_bas, char * nom_tag)
{
    int bufsize, pt_line, i;
    char nom_fic[FIC_NM] = {0};    // Nom complet du fichier image

    FILE *fp = NULL;                    // Pointeur vers le fichier image .bmp
    image *img = NULL;                  // Pointeur vers la structure image

    /* Allocation de la structure image */
    img = (image *)malloc(sizeof(image));
    if (img==NULL)
    {
        printf("Lire_Image 1 : Impossible d'initialiser avec malloc\n" );
        exit(EXIT_FAILURE);
    }
    /* Ouverture fichier binaire en lecture */
    Nom_Image(nom_fic, nom_bas, nom_tag);
    if ((fp = fopen(nom_fic, "rb")) == NULL)
    {
        printf("\nError opening source file %s\n", nom_fic);
        img=NULL;
        return img;
    }

    /* Premiere lecture de l'entete (54 octets) pour obtenir les caracteristiques image */
    memset(&img->header, 0, sizeof(img->header));
    fseek(fp, 0, SEEK_SET);
    fread(&img->header, sizeof(img->header), 1, fp);

    /* Initialisation des autres caracteristiques de l'image */
    img->nb_pix = img->header.hauteur * img->header.largeur;
    strncpy(img->nom_base, nom_bas, FIC_NM);

    /* Allocation de l'image */
    img->pic = Malloc_Pic(img->header.hauteur, img->header.largeur);

    /* Lecture de l'image dans le tableau de pixel */

    bufsize = (3 * img->header.largeur + img->header.largeur%4) * img->header.hauteur;

    for (i=0; i<img->header.hauteur; i++)
    {
        pt_line = img->header.offset + i * (3 * img->header.largeur + img->header.largeur%4);
        fseek(fp, pt_line, SEEK_SET);
        fread(img->pic[i], 3*img->header.largeur, 1, fp);
    }

    /* Resize header to 54 char */
    img->header.offset = HEADER_SIZE;
    img->header.lg_head = HEADER_SIZE - 14;
    img->header.taille = img->header.offset + bufsize;

    /* Desallocation du buffer et fermeture du fichier : l'image est dans la structure img */
    fclose(fp);

    return img;
}

/****************************************************************************************
* Get pixel value
*   img     : structure image
*   x       : coordonnee x
*   y       : coordonnee y
*   retourne un pointeur vers le pixel x,y
****************************************************************************************/
pixel * Get_Pixel(image * img, int x, int y)
{
    return &(img->pic[y][x]);
}

/****************************************************************************************
* Set pixel value
*   img     : structure image
*   x       : coordonnee x
*   y       : coordonnee y
*   col     : couleur a mettre dans le pixel x,y
****************************************************************************************/
void Set_Pixel(image * img, int x, int y, pixel * col)
{
    img->pic[y][x] = *col;

    return;
}
/****************************************************************************************
* Point_Dans_Image
*   img     : structure image
*   x       : coordonnee x
*   y       : coordonnee y
****************************************************************************************/
int Point_Dans_Image(image *img, int x, int y)
{
    return (x >= 0 && x < img->header.largeur && y >= 0 && y < img->header.hauteur);
}
/****************************************************************************************
* Creer une image vierge uniformement coloree en memoire
*   nom_bas : nom de base de l'image
*   hauteur : hauteur de l'image en pixels
*   largeur : largeur de l'image en pixels
*   col     : couleur definie dans enum couleur
*   retourne le pointeur vers l'image allouee
****************************************************************************************/
image * Creer_Image(char * nom_bas, uint largeur, uint hauteur, int col)
{
    image *img = NULL;

    img = (image *)malloc(sizeof(image));
    if (img==NULL)
    {
        printf ("Creer_Image 1 : Impossible d'initialiser avec malloc\n" );
        exit(EXIT_FAILURE);
    }

    memset(&img->header, 0, sizeof(img->header));

    strncpy(img->nom_base, nom_bas, FIC_NM);

    img->header.typ[0]='B';
    img->header.typ[1]='M';

    img->header.largeur = largeur;
    img->header.hauteur = hauteur;
    img->nb_pix = largeur * hauteur;

    img->header.bitpix = BIT_PIXEL;
    img->header.offset = HEADER_SIZE;
    img->header.lg_head = HEADER_SIZE - 14;
    img->header.taille = img->header.offset + ((3 * img->header.largeur + img->header.largeur%4) * img->header.hauteur);

    img->header.applic_id[0] = 'T';
    img->header.applic_id[1] = 'O';
    img->header.applic_id[2] = 'N';
    img->header.applic_id[3] = 'Y';

    img->header.planes = 1;

    img->pic = Malloc_Pic(img->header.hauteur, img->header.largeur);
    Initialiser_Image(img, Get_Col(col) );

    return img;
}


/****************************************************************************************
* Ecrire l'image en memoire sur disque (en passant par un buffer)
*   img     : structure image
*   nom_tag : suffixe du nom d'image
****************************************************************************************/
void Ecrire_Image(image * img, char * nom_tag)
{
    int i, pt_line;
    char nom_fic[FIC_NM] = {0};
    char fil[4]= {0};

    FILE *fp = NULL;

    Nom_Image(nom_fic, img->nom_base,nom_tag);
    if ((fp = fopen(nom_fic, "wb")) ==NULL)
    {
        printf("\nError opening source file\n");
        exit(EXIT_FAILURE);
    }

    fwrite(&img->header, sizeof(img->header), 1, fp);

    for (i=0; i<img->header.hauteur; i++)
    {
        pt_line = img->header.offset + i * (3 * img->header.largeur + img->header.largeur%4);
        fseek(fp, pt_line, SEEK_SET);
        fwrite(img->pic[i], 3*img->header.largeur, 1, fp);
        fwrite(fil, img->header.largeur%4, 1, fp);
    }

    fclose(fp);

    return;
}


/****************************************************************************************
* Ecrire l'image en mémoire sur disque dans le dossier Rapport_Graphique
*   img  : structure image
*   date : date du jour en format jj_mm_aaaa
****************************************************************************************/
void Ecrire_Image_Rap_1(image *img, int date)
{
    int i, pt_line;
    char nom_fic[FIC_NM] = {0};       // Nom complet du fichier
    char nom_tag[128] = {0};          // Buffer temporaire pour le tag
    char date_str[16] = {0};          // Buffer pour la date passée en paramètre
    char date_jour[16] = {0};         // Buffer pour la date du jour
    char fil[4] = {0};

    FILE *fp = NULL;
    // Convertir la date passée en paramètre en chaîne
    snprintf(date_str, sizeof(date_str), "%d", date);
    // Obtenir la date du jour
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(date_jour, sizeof(date_jour), "%02d_%02d_%04d",
             tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    // Construire le tag avec les deux dates
    snprintf(nom_tag, sizeof(nom_tag), "_Emp_Roi_Date_%s_%s", date_str, date_jour);
    Nom_Image(nom_fic, img->nom_base, nom_tag);
    if ((fp = fopen(nom_fic, "wb")) ==NULL)
    {
        printf("\nError opening source file----------------\n");
        exit(EXIT_FAILURE);
    }

    fwrite(&img->header, sizeof(img->header), 1, fp);

    for (i=0; i<img->header.hauteur; i++)
    {
        pt_line = img->header.offset + i * (3 * img->header.largeur + img->header.largeur%4);
        fseek(fp, pt_line, SEEK_SET);
        fwrite(img->pic[i], 3*img->header.largeur, 1, fp);
        fwrite(fil, img->header.largeur%4, 1, fp);
    }
    fclose(fp);
    printf("Rapport sauvegarde dans : %s\n", nom_fic);

    // Étape 9 : Afficher l'image sous Windows
    char commande[256] = {0};
    snprintf(commande, sizeof(commande), "start %s", nom_fic);
    system(commande);

    return;
}
/****************************************************************************************
* Ecrire l'image en mémoire sur disque dans le dossier Rapport_Graphique
*   img  : structure image
*   date : date du jour en format jj_mm_aaaa
****************************************************************************************/
void Ecrire_Image_Rap_2(image *img, int date)
{
    int i, pt_line;
    char nom_fic[FIC_NM] = {0};       // Nom complet du fichier
    char nom_tag[128] = {0};          // Buffer temporaire pour le tag
    char date_str[16] = {0};          // Buffer pour la date passée en paramètre
    char date_jour[16] = {0};         // Buffer pour la date du jour
    char fil[4] = {0};

    FILE *fp = NULL;
    // Convertir la date passée en paramètre en chaîne
    snprintf(date_str, sizeof(date_str), "%d", date);
    // Obtenir la date du jour
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(date_jour, sizeof(date_jour), "%02d_%02d_%04d",
             tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    // Construire le tag avec les deux dates
    snprintf(nom_tag, sizeof(nom_tag), "_Emp_Roi_Carte_%s_%s", date_str, date_jour);
    Nom_Image(nom_fic, img->nom_base, nom_tag);
    if ((fp = fopen(nom_fic, "wb")) ==NULL)
    {
        printf("\nError opening source file----------------\n");
        exit(EXIT_FAILURE);
    }

    fwrite(&img->header, sizeof(img->header), 1, fp);

    for (i=0; i<img->header.hauteur; i++)
    {
        pt_line = img->header.offset + i * (3 * img->header.largeur + img->header.largeur%4);
        fseek(fp, pt_line, SEEK_SET);
        fwrite(img->pic[i], 3*img->header.largeur, 1, fp);
        fwrite(fil, img->header.largeur%4, 1, fp);
    }
    fclose(fp);
    printf("Rapport sauvegarde dans : %s\n", nom_fic);

    // Étape 9 : Afficher l'image sous Windows
    char commande[256] = {0};
    snprintf(commande, sizeof(commande), "start %s", nom_fic);
    system(commande);

    return;
}


/****************************************************************************************
* Allocation dynamique d'une image sous forme d'un tableau de pixels a 2 dimensions
*   hauteur : hauteur de l'image en pixels
*   largeur : largeur de l'image en pixels
*   retourne un pointeur vers la matrice image allouee
****************************************************************************************/
pixel ** Malloc_Pic(uint hauteur, uint largeur)
{
    int i;
    pixel **t;

    t = (pixel**)malloc(sizeof(*t) * hauteur);
    if (t==NULL)
    {
        printf ("Malloc_pic 1 : Impossible d'initialiser avec malloc\n" );
        exit(EXIT_FAILURE);
    }

    for (i=0; i<hauteur; i++)
    {
        t[i] = (pixel*)malloc(sizeof(**t) * largeur);
        if (t[i]==NULL)
        {
            printf ("Malloc_pic 2 : Impossible d'initialiser avec malloc\n" );
            exit(EXIT_FAILURE);
        }
    }

    return t;
}

/****************************************************************************************
* Desallocation d'une image
*   img     : structure image a desallouer
****************************************************************************************/
void Free_Image(image * img)
{
    Free_Pic(img->pic, img->header.hauteur);

    free(img);

    return;
}

/****************************************************************************************
* Desallocation d'une matrice image
*   t       : matrice image a desallouer
*   hauteur : hauteur de l'image en pixels
****************************************************************************************/
void Free_Pic(pixel ** t, uint hauteur)
{
    int i;

    for (i=0; i<hauteur; i++) free(t[i]);

    free(t);

    return;
}

/****************************************************************************************
* Initialiser l'image a une valeur constante
*   img : structure image
*   col : vecteur des composantes de la couleur
****************************************************************************************/
void Initialiser_Image(image * img, pixel * col)
{
    int x, y;

    for (x=0; x<img->header.largeur; x++)
    {
        for (y=0; y<img->header.hauteur; y++)
        {
            Set_Pixel(img, x, y, col);
        }
    }

    return;
}
/****************************************************************************************
* Dessiner une ligne de points entre deux points
*   img : pointeur vers l'image dans laquelle dessiner la ligne
*   x1, y1 : coordonnées du point de départ
*   x2, y2 : coordonnées du point d'arrivée
*   col : couleur de la ligne
*   intervalle : espace entre les points
****************************************************************************************/
void Dessiner_Ligne_Points(image *img, int x1, int y1, int x2, int y2, int col, int intervalle)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int count = 0;

    pixel *pix = Get_Col(col);

    // Boucle principale pour dessiner la ligne de points
    while (x1 != x2 || y1 != y2)
    {
        // Vérifie les limites de l'image sans afficher de message
        if (Point_Dans_Image(img, x1, y1))
        {
            // Place un point seulement tous les 'intervalle' pixels
            if (count % intervalle == 0)
            {
                Set_Pixel(img, x1, y1, pix);
            }
        }
        count++;

        // Calcul des prochaines coordonnées selon l'algorithme de Bresenham
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

/****************************************************************************************
* Dessiner un rectangle de points
*   img : pointeur vers l'image dans laquelle dessiner le rectangle
*   x, y : coordonnées du point supérieur gauche du rectangle
*   largeur : largeur du rectangle
*   hauteur : hauteur du rectangle
*   col : couleur du rectangle
*   intervalle : espace entre les points
****************************************************************************************/
void Dessiner_Rectangle_Points(image *img, int x, int y, int largeur, int hauteur, int col, int intervalle)
{
    // Dessine le côté supérieur
    Dessiner_Ligne_Points(img, x, y, x + largeur, y, col, intervalle);

    // Dessine le côté droit
    Dessiner_Ligne_Points(img, x + largeur, y, x + largeur, y + hauteur, col, intervalle);

    // Dessine le côté inférieur
    Dessiner_Ligne_Points(img, x + largeur, y + hauteur, x, y + hauteur, col, intervalle);

    // Dessine le côté gauche
    Dessiner_Ligne_Points(img, x, y + hauteur, x, y, col, intervalle);
}

/****************************************************************************************
* Dessiner une ellipse de points
*   img : pointeur vers l'image dans laquelle dessiner l'ellipse
*   cx, cy : coordonnées du centre de l'ellipse
*   largeur : largeur de l'ellipse (diamètre horizontal)
*   hauteur : hauteur de l'ellipse (diamètre vertical)
*   col : couleur de l'ellipse
****************************************************************************************/
void Dessiner_Ellipse(image *img, int cx, int cy, int largeur, int hauteur, int col)
{
    int a = largeur / 2;    // Demi-largeur (rayon horizontal)
    int b = hauteur / 2;    // Demi-hauteur (rayon vertical)
    int a2 = a * a;
    int b2 = b * b;
    int x = 0;
    int y = b;
    int sigma = 2 * b2 + a2 * (1 - 2 * b);

    pixel *pix = Get_Col(col);

    // Partie supérieure de l'ellipse
    while (b2 * x <= a2 * y)
    {
        Set_Pixel(img, cx + x, cy + y, pix);
        Set_Pixel(img, cx - x, cy + y, pix);
        Set_Pixel(img, cx + x, cy - y, pix);
        Set_Pixel(img, cx - x, cy - y, pix);

        if (sigma >= 0)
        {
            sigma += 4 * a2 * (1 - y);
            y--;
        }
        sigma += b2 * (4 * x + 6);
        x++;
    }

    // Partie inférieure de l'ellipse
    x = a;
    y = 0;
    sigma = 2 * a2 + b2 * (1 - 2 * a);

    while (a2 * y <= b2 * x)
    {
        Set_Pixel(img, cx + x, cy + y, pix);
        Set_Pixel(img, cx - x, cy + y, pix);
        Set_Pixel(img, cx + x, cy - y, pix);
        Set_Pixel(img, cx - x, cy - y, pix);

        if (sigma >= 0)
        {
            sigma += 4 * b2 * (1 - x);
            x--;
        }
        sigma += a2 * (4 * y + 6);
        y++;
    }
}

// Fonction pour copier une image
image *Copier_Image(image *img_originale)
{
    if (img_originale == NULL) return NULL;

    // Allouer la mémoire pour la nouvelle image
    image *img_copie = (image *)malloc(sizeof(image));
    if (img_copie == NULL)
    {
        printf("Erreur d'allocation pour l'image copiee\n");
        exit(EXIT_FAILURE);
    }

    // Copier l'en-tête et les informations de base
    img_copie->header = img_originale->header;
    img_copie->nb_pix = img_originale->nb_pix;
    strncpy(img_copie->nom_base, img_originale->nom_base, FIC_NM);

    // Allouer le tableau de pixels pour la copie
    img_copie->pic = Malloc_Pic(img_originale->header.hauteur, img_originale->header.largeur);
    if (img_copie->pic == NULL)
    {
        printf("Erreur d allocation pour les pixels de l image copiee\n");
        free(img_copie);
        exit(EXIT_FAILURE);
    }

    // Copier les pixels de l'image originale
    for (int i = 0; i < img_originale->header.hauteur; i++)
    {
        memcpy(img_copie->pic[i], img_originale->pic[i], 3 * img_originale->header.largeur);
    }

    return img_copie;
}


/*****************************************************************************************
 * Fonction pour modifier la couleur d'un comté donné par son indice.
 *
 * Cette fonction parcourt l'image d'une carte et modifie la couleur des pixels correspondant
 * à un comté donné. Le comté est identifié par la couleur de ses pixels, et la couleur
 * du comté est changée en fonction de l'indice fourni.
 *
 * param carte           Pointeur vers la structure `image` représentant la carte à modifier.
 * param indice_comte    Indice du comté dont la couleur doit être modifiée.
 * param nouvelle_couleur   Nouvelle couleur à appliquer au comté, spécifiée sous forme d'un entier
 *                          représentant la couleur dans une palette ou un format de couleur spécifique.
 *
 * retour                Cette fonction ne retourne rien (void). Elle modifie directement
 *                       l'image en mémoire.
 ****************************************************************************************/
void Modifier_Comte(image *carte, int indice_comte, int nouvelle_couleur)
{
    for (int x = 0; x < carte->header.hauteur; x++) {
        for (int y = 0; y < carte->header.largeur; y++) {
            // Si le pixel correspond à un comté (R == 0 et G == B = 100 + 3 * indice)
            if (carte->pic[x][y].R == 0 && carte->pic[x][y].G == 100 + 3 * indice_comte && carte->pic[x][y].B == 100 + 3 * indice_comte) {
                // On modifie la couleur du pixel
                Set_Pixel(carte, y, x, Get_Col(nouvelle_couleur));
            }
        }
    }
}
