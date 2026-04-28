// Fichier cartes.h
// Contient uniquement les structures et les prototypes (Cours 01 et 08)

typedef struct {
    int valeur;   
    int type;     
} Carte;

typedef struct {
    char nom[50];
    int score;
    int a_brule; 
} Joueur;

Carte* initialiser_paquet(int *taille);
void melanger_paquet(Carte *paquet, int taille);
Carte piocher_carte(Carte *paquet, int *taille);
void afficher_carte(Carte c);