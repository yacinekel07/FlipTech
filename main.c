#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cartes.h" 

int main() {
    srand(time(NULL));
    printf("=== BIENVENUE DANS FLIP 7 ===\n\n");

    int nb_joueurs = 0;
    while (nb_joueurs < 2) {
        printf("Combien de joueurs etes-vous ? (Minimum 2) : ");
        scanf("%d", &nb_joueurs);
    }

    // Allocation dynamique (Tableaux dynamiques de structures)
    Joueur *liste_joueurs = malloc(nb_joueurs * sizeof(Joueur));
    if (liste_joueurs == NULL) {
        printf("Erreur d'allocation memoire pour les joueurs.\n");
        exit(1);
    }

    int p;
    for (p = 0; p < nb_joueurs; p++) {
        printf("Prenom du joueur %d : ", p + 1);
        scanf("%s", liste_joueurs[p].nom);         
        liste_joueurs[p].score_total = 0;        
    }

    int taille_du_paquet = 0;
    Carte *le_paquet = initialiser_paquet(&taille_du_paquet);
    melanger_paquet(le_paquet, taille_du_paquet);
    
    int stats_cartes[13];
    int i;
    for (i = 0; i < 13; i++) {
        stats_cartes[i] = 0; 
    }

    int partie_en_cours = 1;
    int manche = 1;

    // --- BOUCLE DE LA PARTIE ---
    while (partie_en_cours == 1) {
        printf("\n\n=================================\n");
        printf("         DEBUT MANCHE %d\n", manche);
        printf("=================================\n");
        printf("Cartes restantes dans la pioche : %d\n", taille_du_paquet);

        int joueurs_en_lice = nb_joueurs;
        for (p = 0; p < nb_joueurs; p++) {
            liste_joueurs[p].en_jeu = 1;
            liste_joueurs[p].score_manche = 0;
            liste_joueurs[p].nb_cartes = 0;
            liste_joueurs[p].bonus_plus = 0;
            liste_joueurs[p].a_bonus_x2 = 0;
        }

        // --- BOUCLE DE LA MANCHE ---
        int manche_en_cours = 1;
        while (manche_en_cours == 1 && joueurs_en_lice > 0 && taille_du_paquet > 0) {
            
            for (p = 0; p < nb_joueurs; p++) {
                
                // On remplace le "continue" et le "break" par des conditions if
                if (manche_en_cours == 1 && liste_joueurs[p].en_jeu == 1) {
                    
                    if (taille_du_paquet == 0) {
                        printf("\nLA PIOCHE EST VIDE ! Fin immediate de la manche.\n");
                        manche_en_cours = 0; // Remplace le break
                    } 
                    else {
                        printf("\n------------------------------------------------\n");
                        printf("Tour de %s (Score temporaire: %d)\n", liste_joueurs[p].nom, liste_joueurs[p].score_manche);
                        
                        int choix = 0;
                        while (choix != 1 && choix != 2) {
                            printf("1. Piocher | 2. S'arreter | 3. Voir Statistiques : ");
                            scanf("%d", &choix);

                            if (choix == 3) {
                                afficher_statistiques(stats_cartes);
                                choix = 0;
                            }
                        }

                        if (choix == 2) {
                            printf("%s s'arrete et securise ses points.\n", liste_joueurs[p].nom);
                            liste_joueurs[p].en_jeu = 0;
                            joueurs_en_lice--;
                        } 
                        else if (choix == 1) {
                            Carte ma_carte = piocher_carte(le_paquet, &taille_du_paquet);
                            afficher_carte(ma_carte);

                            if (ma_carte.type == 1) { 
                                liste_joueurs[p].bonus_plus = liste_joueurs[p].bonus_plus + ma_carte.valeur;
                                printf("Bonus +%d ajoute !\n", ma_carte.valeur);
                            } 
                            else if (ma_carte.type == 2) { 
                                liste_joueurs[p].a_bonus_x2 = 1;
                                printf("Bonus multiplicateur x2 obtenu !\n");
                            } 
                            else { 
                                stats_cartes[ma_carte.valeur]++; 
                                int a_perdu = 0;
                                int c;
                                
                                for (c = 0; c < liste_joueurs[p].nb_cartes; c++) {
                                    if (liste_joueurs[p].cartes_main[c] == ma_carte.valeur) {
                                        a_perdu = 1;
                                    }
                                }

                                if (a_perdu == 1) {
                                    printf("BOUUM ! %s a deja un %d et perd tout !\n", liste_joueurs[p].nom, ma_carte.valeur);
                                    liste_joueurs[p].score_manche = 0;
                                    liste_joueurs[p].bonus_plus = 0;
                                    liste_joueurs[p].a_bonus_x2 = 0;
                                    liste_joueurs[p].en_jeu = 0;
                                    joueurs_en_lice--;
                                } else {
                                    liste_joueurs[p].cartes_main[liste_joueurs[p].nb_cartes] = ma_carte.valeur;
                                    liste_joueurs[p].nb_cartes++;
                                    liste_joueurs[p].score_manche = liste_joueurs[p].score_manche + ma_carte.valeur;
                                    printf("Score facial actuel : %d\n", liste_joueurs[p].score_manche);

                                    // Condition FLIP 7
                                    if (liste_joueurs[p].nb_cartes == 7) {
                                        printf("\n!!! FLIP 7 !!! %s a 7 cartes differentes !\n", liste_joueurs[p].nom);
                                        liste_joueurs[p].score_manche = liste_joueurs[p].score_manche + 15;
                                        manche_en_cours = 0; // Remplace le break
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } 

        printf("\n--- FIN DE LA MANCHE %d ---\n", manche);
        for (p = 0; p < nb_joueurs; p++) {
            if (liste_joueurs[p].score_manche > 0 || liste_joueurs[p].bonus_plus > 0) {
                if (liste_joueurs[p].a_bonus_x2 == 1) {
                    liste_joueurs[p].score_manche = liste_joueurs[p].score_manche * 2;
                }
                liste_joueurs[p].score_manche = liste_joueurs[p].score_manche + liste_joueurs[p].bonus_plus;
            }
            liste_joueurs[p].score_total = liste_joueurs[p].score_total + liste_joueurs[p].score_manche;
            printf("%s gagne %d points (Total: %d)\n", liste_joueurs[p].nom, liste_joueurs[p].score_manche, liste_joueurs[p].score_total);
            
            if (liste_joueurs[p].score_total >= 200 || taille_du_paquet == 0) {
                partie_en_cours = 0;
            }
        }
        manche++;
    } 

    // --- FIN DE PARTIE & FICHIERS ---
    printf("\n\n======= FIN DE LA PARTIE =======\n");
    int score_max = -1;
    char nom_gagnant[50];

    for (p = 0; p < nb_joueurs; p++) {
        if (liste_joueurs[p].score_total > score_max) {
            score_max = liste_joueurs[p].score_total;
            
            // On utilise la boucle while pour copier le string (au lieu de strcpy)
            int k = 0;
            while(liste_joueurs[p].nom[k] != '\0') {
                nom_gagnant[k] = liste_joueurs[p].nom[k];
                k++;
            }
            nom_gagnant[k] = '\0';
        }
    }
    printf("LE GAGNANT EST %s AVEC %d POINTS !\n", nom_gagnant, score_max);

    int save;
    printf("\nVoulez-vous sauvegarder les scores ? (1 = Oui, 0 = Non) : ");
    scanf("%d", &save);
    
    if (save == 1) {
        char nom_fichier[100];
        printf("Nom du fichier (ex: scores.txt) : ");
        scanf("%s", nom_fichier);
        
        FILE *fichier = fopen(nom_fichier, "w");
        if (fichier != NULL) {
            fprintf(fichier, "--- RESULTATS FLIP 7 ---\n");
            for (p = 0; p < nb_joueurs; p++) {
                fprintf(fichier, "%s : %d points\n", liste_joueurs[p].nom, liste_joueurs[p].score_total);
            }
            fclose(fichier);
            printf("Scores sauvegardes avec succes dans %s !\n", nom_fichier);
        } else {
            printf("Erreur lors de la creation du fichier.\n");
        }
    }

    free(le_paquet);
    free(liste_joueurs);

    return 0;
FJERDHRYYHYHYHYHYYYYYYYYYYYYYYYYYYY
