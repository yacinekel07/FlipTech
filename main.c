#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 
#include "cartes.h"

int main() {
    srand(time(NULL));
    printf("=== BIENVENUE DANS FLIPTECH ===\n\n");

    // --- ON UTILISE L'ALLOCATION DYNAMIQUE ---
    int taille_du_paquet = 0;
    // On reçoit le pointeur créé par le malloc dans la cuisine
    Carte *le_paquet = initialiser_paquet(&taille_du_paquet); 
    
    melanger_paquet(le_paquet, taille_du_paquet);   
    printf("(Le croupier a melange un paquet de %d cartes)\n\n", taille_du_paquet);

    int mode_jeu = 0;
    while (mode_jeu != 1 && mode_jeu != 2) {
        printf("Choisissez votre mode de jeu :\n");
        printf("1. Joueur vs Ordinateur (IA)\n");
        printf("2. Multijoueur local (2 a 4 joueurs)\n");
        printf("Votre choix : ");
        scanf("%d", &mode_jeu);
    }

    Joueur liste_joueurs[4]; 
    int nb_joueurs = 0;
    int p;

    if (mode_jeu == 1) {
        nb_joueurs = 2; 
        printf("\nTon prenom : ");
        scanf("%s", liste_joueurs[0].nom);
        liste_joueurs[0].score = 0;
        
        strcpy(liste_joueurs[1].nom, "L'Ordinateur (IA)");
        liste_joueurs[1].score = 0;
    } 
    else {
        while (nb_joueurs < 2 || nb_joueurs > 4) {
            printf("\nCombien de joueurs etes-vous ? (2 a 4) : ");
            scanf("%d", &nb_joueurs);
        }
        for (p = 0; p < nb_joueurs; p++) {
            printf("Prenom du joueur %d : ", p + 1);
            scanf("%s", liste_joueurs[p].nom);         
            liste_joueurs[p].score = 0;        
        }
    }

    printf("\n>>> QUE LA PARTIE COMMENCE ! <<<\n");

    for (p = 0; p < nb_joueurs; p++) {
        
        printf("\n------------------------------------------------\n");
        printf("C'est a %s de jouer !\n", liste_joueurs[p].nom);
        printf("------------------------------------------------\n");

        int choix = 1;
        int score_tour = 0;
        int cartes_en_main[15]; 
        int nb_cartes = 0;      
        int vies_en_plus = 0; 
        int tours_forces = 0; 

        int est_IA = 0;
        if (mode_jeu == 1 && p == 1) {
            est_IA = 1; 
        }

        while (choix == 1 && taille_du_paquet > 0) {
            
            Carte ma_carte = piocher_carte(le_paquet, &taille_du_paquet);
            
            printf("\n%s pioche :\n", liste_joueurs[p].nom);
            afficher_carte(ma_carte);

            if (ma_carte.type == 1) { 
                printf("\n>>> CARTE STOP ! Fin du tour, mais les points sont conserves ! <<<\n");
                choix = 0; 
                tours_forces = 0;
            }
            else if (ma_carte.type == 2) { 
                printf("SUPER ! %s gagne une vie supplementaire !\n", liste_joueurs[p].nom);
                vies_en_plus++;
            }
            else if (ma_carte.type == 3) { 
                printf("AIE ! %s est oblige(e) de piocher 3 cartes de suite !\n", liste_joueurs[p].nom);
                tours_forces = tours_forces + 3; 
            }
            else { 
                int a_perdu = 0; 
                int i;
                for (i = 0; i < nb_cartes; i++) {
                    if (cartes_en_main[i] == ma_carte.valeur) {
                        a_perdu = 1; 
                    }
                }

                if (a_perdu == 1) {
                    if (vies_en_plus > 0) {
                        printf("OUF ! La 2nde chance sauve du %d !\n", ma_carte.valeur);
                        vies_en_plus--;
                    } else {
                        printf("OH NON ! Deja un %d. %s a brule !\n", ma_carte.valeur, liste_joueurs[p].nom);
                        score_tour = 0; 
                        choix = 0;   
                        tours_forces = 0; 
                    }   
                } 
                else {
                    cartes_en_main[nb_cartes] = ma_carte.valeur;
                    nb_cartes++; 
                    score_tour += ma_carte.valeur;
                    printf("Score actuel de %s : %d\n", liste_joueurs[p].nom, score_tour);
                }
            }

            if (choix == 1) {
                if (tours_forces > 0) {
                    printf("\n(Tirage(s) force(s) restant(s) : %d...)\n", tours_forces);
                    tours_forces--; 
                } 
                else {
                    if (est_IA == 1) {
                        if (score_tour >= 15) {
                            printf("L'Ordinateur decide de S'ARRETER.\n");
                            choix = 0;
                        } else {
                            printf("L'Ordinateur decide de CONTINUER.\n");
                            choix = 1;
                        }
                    } 
                    else {
                        printf("Veux-tu piocher une autre carte ? (1 = OUI, 0 = NON) : ");
                        scanf("%d", &choix); 
                    }
                }
            }
        } 

        liste_joueurs[p].score = score_tour;
        printf("\nFin du tour de %s. Score final : %d points.\n", liste_joueurs[p].nom, liste_joueurs[p].score);
    } 

    printf("\n\n======= FIN DE LA PARTIE =======\n");
    int score_max = -1;
    char nom_gagnant[50];

    for (p = 0; p < nb_joueurs; p++) {
        printf("%s a fait %d points.\n", liste_joueurs[p].nom, liste_joueurs[p].score);
        if (liste_joueurs[p].score > score_max) {
            score_max = liste_joueurs[p].score;
            int i = 0;
            while(liste_joueurs[p].nom[i] != '\0') {
                nom_gagnant[i] = liste_joueurs[p].nom[i];
                i++;
            }
            nom_gagnant[i] = '\0';
        }
    }
    printf("\nLE GAGNANT EST %s AVEC %d POINTS ! BRAVO !\n", nom_gagnant, score_max);

    // --- TRÈS IMPORTANT : ON LIBÈRE LA MÉMOIRE AVANT DE QUITTER ---
    free(le_paquet); 
    // --------------------------------------------------------------

    return 0;
}