/****************************************************************************************
* Dossier Blasons : Creation de la Database
*
* Definition des structures : DB Blason
*
* Programmation procedurale 2024 - A.Umina - Version 0
****************************************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "Util_Bitmap.h"
#include "menu.h"
#include "Rapport.h"

#define NBR_CIT   1200000
#define NBR_NOB   6000
#define NBR_CHARG 600000
#define NBR_PAY   10
#define NBR_DUC   50
#define NBR_VILLE 6000
#define NBR_RICH  6639605
#define NBR_BLAZ  8000
#define BLA_LARG 40
#define BLA_HAUT 60
#define HEADER_SIZE 54
#define BIT_PIXEL   24
#pragma pack(2)     // Indispensable pour l'ecriture correcte du header
#define BUF_LEN 200
#define BLA_LARG 40
#define BLA_HAUT 60
#define CART_ROI_LARG 220
#define CART_ROI_HAUT 170
#define CART_DUC_LARG 220
#define CART_DUC_HAUT 100
#define CART_EMP_LARG 220
#define CART_EMP_HAUT 170
#define ROIS_PAR_PAGE 30

typedef unsigned int uint;



/***************************************************************************************
* Header de la database
****************************************************************************************/
typedef struct Header
{
    uint db_size;       // Taille de la database
    char db_name[24];   // Nom de la database
    uint rac_ind_charge_arb;//racine de l arbre

    uint sz_cit;        // Taille bloc citoyens
    uint sz_ind_cit;    // bloc index citoyens
    uint sz_ind_id_cit;
    uint sz_nob;        // Taille bloc noblesse
    uint sz_charge;     // Taille bloc charges
    uint sz_ind_charge; // Taille bloc blaz
    uint sz_ind_charge_arb;
    uint sz_pay;        // Taille bloc pays
    uint sz_duc;        // Taille bloc duche

    uint sz_ville;      // Taille bloc ville
    uint sz_rich;       // Taille bloc richesse
    uint sz_blaz;       // Taille bloc blaz
    uint off_cit;       // Position bloc cit
    uint off_ind_cit;   // Position bloc index cit
    uint off_ind_cit_id;
    uint off_nob;
    uint off_charge;
    uint off_ind_charge;

    uint off_ind_cha_arb;
    uint off_pay;       // Position bloc pays
    uint off_duc;       // Position bloc duche
    uint off_ville;     // Position bloc ville
    uint off_rich;     // Position bloc richesse
    uint off_blaz;     // Position bloc richesse
    uint nbr_cit;       // Nbr de cit
    uint nbr_ind_cit;   // Nbr d index de cit
    uint nbr_ind_cit_id;
    uint nbr_nob;
    uint nbr_charge;
    uint nbr_ind_charge;
    uint nbr_ind_cha_arb;
    uint nbr_pay;       // Nbr of pays
    uint nbr_duc;       // Nbr of duche
    uint nbr_ville;     // Nbr of ville
    uint nbr_richesse;  // Nbr of richesse

    uint nbr_blaz;      // Nbr of blaz
    char filler[16];     // Filler
} hder;

/****************************************************************************************
* Structure table Citoyen
****************************************************************************************/
typedef struct Citoyen
{
    char tp_rec[8];     // Type de record Cit
    int  id_cit;
    int  id_pays;
    int  id_duche;
    char fill_1[12];    // Champ de remplissage pour l'alignement 32o

    char nom_ville[31];
    char sex;

    char nom_cit[32];
    char pre_cit[32];
    char fct_cit[32];

    char date_cit[12];
    char fill_2[20];     // Nouveau remplissage pour l'alignement à 32 octets
} citoyen;

/****************************************************************************************
* Structure indexe  Citoyen.nom
****************************************************************************************/
typedef struct Index_Nom_Cit {
    char tp_rec[8];       // Type de record Cit
    uint ptr_offset;      // Offset dans le fichier vers la structure Citoyen
    char fill[20];        // Remplissage pour alignement à 32 octets
    char nom_cit[64];     // Nom du citoyen (copié de la structure citoyen)

} index_nom_cit;
/****************************************************************************************
* Structure indexe  Citoyen.id
****************************************************************************************/
typedef struct Index_id_Cit {
    char tp_rec[8];       // Type de record Cit
    uint ptr_offset;      // Offset dans le fichier vers la structure citoyen
    int id_cit;           // id du citoyen (copié de la structure charge)
    char fill[16];        // Remplissage pour alignement à 32 octets
} index_id_cit;

/****************************************************************************************
* Structure table Noblesse
****************************************************************************************/
typedef struct Noblesse
{
    char tp_rec[8];     // Type de record Noblesse
    int  id_nob;
    int  rang_nob;
    int  id_suze;
    int  id_bla;
    int  id_pays;
    int  id_duche;

    char titre_nob[32];
    char region_nob[32];
    char zone_nob[32];

} noblesse;

/****************************************************************************************
* Structure table Charge
****************************************************************************************/
typedef struct Charge
{
    char tp_rec[8];     // Type de record charge
    int  id_nob;
    int  id_indi;
    int  yr_in;
    int  yr_out;
    char fill_1[8];    // filler pour 32
} charge;

/****************************************************************************************
* Structure indexe  charge.id_citoyen
****************************************************************************************/
typedef struct Index_id_Cit_Cha {
    char tp_rec[8];       // Type de record Cit
    uint ptr_offset;      // Offset dans le fichier vers la structure Charge
    int id_cit;           // id du citoyen (copié de la structure charge)
    char fill[16];        // Remplissage pour alignement à 32 octets
} index_id_cit_cha;

/****************************************************************************************
* Structure indexe  charge.id_cit_Arbre
****************************************************************************************/
typedef struct Index_id_Cit_Arb {
    char tp_rec[8];           // Type de record index
    uint id_pers_offset;      // Offset id dans la table charge
    uint ng_offset;           // Offset noeud gauche
    uint nd_offset;           // Offset noeud droite
    char fill[8];            // Remplissage pour alignement à 32 octets
    int id_cit_arb;           // id du citoyen (copié de la structure charge)
} index_id_cit_arb;

/***************************************************************************************
* Structure table pays
****************************************************************************************/
typedef struct Pays
{
    char tp_rec[8];     // categorie de record PAYS
    int  id_pays;       // 4oct
    char fill_1[20];    // filler pour 32
    char nm_pays[32];
} pays;

/***************************************************************************************
* Structure table duche
****************************************************************************************/
typedef struct Duche
{
    char tp_rec[8];     // Type de record DUCHE
    int  id_duche;
    int  id_pays;
    char fill_1[16];
    char nm_duche[32];
} duch;

/***************************************************************************************
* Structure table ville
****************************************************************************************/
typedef struct Ville
{
    char tp_rec[8];     // Type de record ville
    int  id_ville;
    int  id_duch;
    char fill_1[16];
    char nom_ville[24];
    int  nr;
    char cap[4];
} ville;

/***************************************************************************************
* Structure table Richesses
****************************************************************************************/
typedef struct Richesse
{
    char tp_rec[8];     // Type de record ville
    int  id_ville;
    int  annee;
    int  popu;
    int  prod;
    char fill_1[8];
} richesse;

/***************************************************************************************
* Structure temporaire pour le tri des index
****************************************************************************************/
typedef struct	Tri
{
    int  indice;
    char nom[64];
    uint obj_offset; // offset de l objet pointé dans la base
    uint pt_ng;
    uint pt_nd;
} tab_tri;

/****************************************************************************************
* Record
****************************************************************************************/
typedef struct Record {
    charge cha;
    citoyen cit;
    noblesse nob;
    int off_cit;
    duch duc;
} record;
/***************************************************************************************
* Structure base de donnees
****************************************************************************************/
typedef struct	db_blason
{
    hder hdr;           // Header
	FILE *fp_db;        // Data file
	FILE *fp_lg;        // Log file
    pays pay[NBR_PAY];  // Buffer Pays
    duch duc[NBR_DUC];  // Buffer duches
    int status;
    tab_tri *tri;
    record *reco;
} dbb;


/***************************************************************************************
* Structure des petits blasons
****************************************************************************************/
typedef struct Image_Stat               // Image statique pour dessiner blason
{
    head_bmp header;                    // Header
    pixel img[BLA_HAUT][BLA_LARG];      // Image
} blason;

/***************************************************************************************
* enumeration du type de cartouche
****************************************************************************************/
typedef enum {
    CART_ROI,
    CART_DUC,
    CART_EMP
} cartouche_type;

/***************************************************************************************
* Structure des cartouches
****************************************************************************************/
typedef struct cartouche_roi{
    head_bmp header;
    cartouche_type type;  // Le type de cartouche
    char titre[FIC_NM];
    char nom[FIC_NM];
    pixel img_roi[CART_ROI_HAUT][CART_ROI_LARG];
    int ancre_x;
    int ancre_y;
} cart_roi;

typedef struct cartouche_duc{
    head_bmp header;
    cartouche_type type;  // Le type de cartouche
    char titre[FIC_NM];
    char nom[FIC_NM];
    pixel img_duc[CART_DUC_HAUT][CART_DUC_LARG];
    int ancre_x;
    int ancre_y;
} cart_duc;

typedef struct cartouche_empreur{
    head_bmp header;
    cartouche_type type;
    char titre[FIC_NM];
    char nom[FIC_NM];
    pixel img_emp[CART_EMP_HAUT][CART_EMP_LARG];
    int ancre_x;
    int ancre_y;
} cart_emp;


/****************************************************************************************
* Polices de caracteres graphiques
****************************************************************************************/
#define POL_SPACE 2 // Nombre de pixels entre 2 caracteres

enum pol_size
{
    POL_S,  // Small
    POL_M,  // Medium
    POL_B   // Big
};

enum pol_ori
{
    POL_0,
    POL_90,
    POL_180,
    POL_270
};
/****************************************************************************************
* Liste doublement chainee generique : allocation dynamique des donnees
****************************************************************************************/
typedef struct LDC_Gen
{
    void *rec;               /* Pointeur vers structure quelconque */
    struct LDC_Gen* prec;    /* pointeur sur l'élément précédent */
    struct LDC_Gen* suiv;    /* pointeur sur l'élément suivant */
} Gen_LDC;
/****************************************************************************************
* Prototypes DB
****************************************************************************************/
void Create_DB(dbb *db);
void Open_DB(dbb *db);
void Close_DB(dbb *db);
void Update_DB_Header(dbb *db);

void Import_Citoyens(dbb *db);
void Export_Citoyens(dbb *db);

void Import_Noblesses(dbb *db);
void Export_Noblesses(dbb *db);

void Import_Charges(dbb *db);
void Export_Charges(dbb *db);

void Import_Pays(dbb *db);
void Export_Pays(dbb *db);

void Import_Duches(dbb *db);
void Export_Duches(dbb *db);

void Import_Villes(dbb *db);
void Export_Villes(dbb *db);

void Import_Richesses(dbb *db);
void Export_Richesses(dbb *db);

void Import_Blason(dbb *db);
void Export_Blason(dbb *db);

void Load_Buffers_pays(dbb *db);
void Lister_Pays_DB(dbb *db);
void Lister_Pays_BUF(dbb *db);
void Load_Buffers_duch(dbb *db);
void Lister_duch_BUF(dbb *db);


/****************************************************************************************
* Prototypes image
****************************************************************************************/
void Filtrer_Noir_Blanc(image * img);
void Damier(image * img);

void Lire_Mask(blason * bla , int position);
void Lire_Blason(blason * bla, int position);
void Lire_Blason_db(dbb *db, blason * blaz, int position);
void Initialiser_Blason(blason * bla, pixel * col);
void Ecrire_Blason(blason * bla, char * nm_tag);
void Ecrire_Blason_img(image * img, blason * bla, int x, int y);
void Ecrire_Blason_cart_duc(cart_duc *cart, blason *bla, int posX, int posY);
void Ecrire_Blason_cart_roi(cart_roi *cart, blason *bla, int posX, int posY);
void Ecrire_Blason_cart_emp(cart_emp *cart, blason *bla, blason *mask, int posX, int posY);
void Ecrire_Blason_M_img(image *img, blason *bla, blason *mask, int posX, int posY);



void Ecrire_Cartouche_roi(image *img, cart_roi *cart, int x_center, int y_center);
void Initialiser_Cartouche_roi(cart_roi *cart, blason *bla, blason *mask, pixel *col, pixel *bord_col, char zone[32], char nom[32], int in, int out);
void Initialiser_Cartouche_roi2(cart_roi *cart, blason *bla, pixel *col, pixel *bord_col);
void Set_Pixel_Cartouche_roi(cart_roi *cart, int x, int y, pixel *src_pixel);
void Ecrire_Caractere_Cartouche_roi(cart_roi *cart, int c, int sz, int ori, int x, int y, int col);
void Ecrire_Chaine_Cartouche_roi(cart_roi *cart, char *txt, int police, int angle, int x, int y, int couleur, int pas);

void Initialiser_Cartouche_duc(cart_duc *cart, blason *bla, pixel *col, pixel *bord_col);
void Ecrire_Cartouche_duc(image *img, cart_duc *cart, int x_offset, int y_offset);
void Set_Pixel_Cartouche_duc(cart_duc *cart, int x, int y, pixel *src_pixel);
void Ecrire_Caractere_Cartouche_duc(cart_duc *cart, int c, int sz, int ori, int x, int y, int col);
void Ecrire_Chaine_Cartouche_duc(cart_duc *cart, char *txt, int police, int angle, int x, int y, int couleur, int pas);


void Initialiser_Cartouche_emp(cart_emp *cart, blason *bla, blason *mask, pixel *col, pixel *bord_col, char zone[32], char nom[32], int in, int out);
void Ecrire_Cartouche_emp2(image *img, cart_emp *cart, int x_offset, int y_offset);
void Ecrire_Cartouche_emp(image *img, cart_emp *cart, int x_center, int y_center);
void Set_Pixel_Cartouche_emp(cart_emp *cart, int x, int y, pixel *src_pixel);
void Ecrire_Caractere_Cartouche_emp(cart_emp *cart, int c, int sz, int ori, int x, int y, int col);
void Ecrire_Chaine_Cartouche_emp(cart_emp *cart, char *txt, int police, int angle, int x, int y, int couleur, int pas);
void test_image(dbb *db);
void Modifier_Comte(image *carte, int indice_comte, int nouvelle_couleur);
void Positionner_Image(image *source, image *inseree, int pos_x, int pos_y);
image* Colorer_Carte(dbb *db);

/****************************************************************************************
* Prototypes index
****************************************************************************************/
void Creer_index(dbb *db);
void Creer_index_nom(dbb *db);
void Creer_index_cit_id(dbb *db);
void Creer_index_id(dbb *db);
void Creer_index_id_arbre(dbb *db);

void Ecrire_Index_Cit(dbb *db);
void Ecrire_Index_Cit_id(dbb *db);
void Ecrire_Index_Id(dbb *db);
void Ecrire_Index_Id_Arbre(dbb *db);

int Comparer_Nom(const void *a, const void *b);
citoyen read_cit_off(dbb *db, int pos);

void Afficher_Index_Cit(dbb *db);
void Afficher_Index_Cit_id(dbb *db);
void Afficher_Index_Charge(dbb *db);
void Afficher_Index_Cha_arb(dbb *db);

 uint build_tree_charge(dbb *db, uint r, int n);

/****************************************************************************************
* Prototypes recherche
****************************************************************************************/
void tab_tri_alloc(dbb *db, int nbr);
void tab_record_Dyn(dbb *db, int nbr);
void tab_tri_free(dbb *db);
void tab_record_free(dbb *db);
void tri_bulle_nom(dbb *db, int nbr);
void tri_rapide_nom(dbb *db, int left, int right);
void tri_rapide_id(dbb *db, int left, int right);

uint Rechercher_Charge_Arbre_Id(dbb *db, int id);
void Afficher_Charge_Par_Offset(dbb *db, uint off_charge);

int Recuperer_Rois_Rang(dbb *db);
int Recuperer_Rois_par_Zone(dbb *db);
int Recuperer_Id_Rois_Charge(dbb *db, int id_nob);
int Recuperer_Id_Marquis_Charge(dbb *db, int id_nob);
void Recuperer_off_cit_roi(dbb *db, int nbr_record);
citoyen Recuperer_data_cit_roi(dbb *db, int pos);
citoyen Recuperer_Citoyen_Par_Offset(dbb *db, int offset);
void Remplir_data_TabRois(dbb *db, int nbr_record);
int Recherche_Dichotomique_DB(dbb *db, uint offset_base, uint taille, int id_recherche);
int Recherche_Dichotomique_Nom(dbb *db, uint offset_base, uint taille, const char *nom_recherche);
void Recherche_Par_Prefix(dbb *db);

void Recherche_Noms_Commencant_Par(dbb *db, const char *prefix);
void Afficher_Rois_Pagination(dbb *db, int nbr_record);
void Recherche_roi_pays(dbb *db);
void Recherche_Id_Cha_Arb(dbb *db);
void Recherche_Off_Cit_Par_Id(dbb *db);
void Recherche_Cit_Par_off(dbb *db);


/****************************************************************************************
* Prototypes menu
****************************************************************************************/
void menu(dbb *db);
void menu_rech_ind(dbb *db);
void Menu_Lister(dbb *db);
void menu_rapport(dbb *db);
void menu_List_DC(dbb *db);
/****************************************************************************************

************************************************************************************
* Prototypes specifiques
****************************************************************************************/
void Print_Pays(void *pt);
void Print_Citoyen(void *pt);
int PGd_Pays(void *r1, void *r2);
int PGd_Citoyen(void *r1, void *r2);
void Load_Citoyen(dbb *db, Gen_LDC *rac_per, int nr);
void Print_record(void *pt);

/****************************************************************************************
* Prototypes generiques
****************************************************************************************/
Gen_LDC* creeListe (void);
void parcourDir(Gen_LDC* liste);
void parcourInv(Gen_LDC* liste);
void viderListe (Gen_LDC* liste);
void supprimerListe (Gen_LDC** liste);
void ajouterAvant (Gen_LDC* element, void *rec, int size_rec);
void ajouterApres (Gen_LDC* element, void *rec, int size_rec);
void ajouterEnTete (Gen_LDC* racine, void *rec, int size_rec);
void ajouterEnQueue (Gen_LDC* racine, void *rec, int size_rec);
void supprimerElement (Gen_LDC* element);
void supprimerPremierElement (Gen_LDC* racine);
void supprimerDernierElement (Gen_LDC* racine);
Gen_LDC* premierElement (Gen_LDC* racine);
Gen_LDC* dernierElement (Gen_LDC* racine);
size_t nombreElements (Gen_LDC* liste);
Gen_LDC* chercherPGd(Gen_LDC* liste, void *rec, int (* pF) (void *r1, void *r2));
void printDir(Gen_LDC* liste, void (* pF) (void*) );
void printInv(Gen_LDC* liste, void (* pF) (void*) );
void Test_LDC1(dbb *db);
void Test_LDC2(dbb *db);
void Test_LDC3(dbb *db);
void Test_LDC4(dbb *db);
/************************************************************************************
* rapport
****************************************************************************************/
void Afficher_Rois_Pagination_Periode(dbb *db, int nbr_record);
void Dessiner_Emp_Cart(dbb *db,char tab1[32],char tab2[32], int in, int out ,int date);
void Creer_Rap_Marquiq_Periode(dbb *db);
int Recuperer_Marquis_par_Zone(dbb *db);
void Remplir_nob_TabRois(dbb *db, int nbr_record);
void Creer_Rap_Roi_Periode(dbb *db);
int Recuperer_Charge_date_ldc(dbb *db, int date, Gen_LDC *ldc);
void Recuperer_Off_Ldc_dico(dbb *db, Gen_LDC *liste);
void Recuperer_data_Cit_Ldc(dbb *db, Gen_LDC *liste);
void Recuperer_Nob_Ldc_seq(dbb *db, Gen_LDC *liste);
void Recuperer_Duc_Ldc_seq(dbb *db, Gen_LDC *liste);
void Init_Image_rap1(dbb *db, Gen_LDC *liste,int date);
void Init_Image_rap2(dbb *db, Gen_LDC *liste,int date,image *carte);
void Creer_Map_date2(dbb *db);
void Creer_Map_date(dbb *db);
void Ecrire_Image_Rap_2(image *img, int date);
void Ecrire_Image_Rap_1(image *img, int date);
