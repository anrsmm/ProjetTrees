#include <stdio.h>
#include <stdlib.h>
#include "grille.h"
#include "voisinage.h"

int main(int argc, char *argv[]) {
    Grille *g;
    Position p;
    Position voisins[4];
    int nb, i;

    if (argc < 4) {
        printf("Usage: %s <fichier> <ligne> <colonne>\n", argv[0]);
        return 1;
    }

    g = lire_grille_fichier(argv[1]);
    if (g == NULL) {
        printf("Erreur lecture fichier\n");
        return 1;
    }



    p.ligne = atoi(argv[2]);
    p.colonne = atoi(argv[3]);

    afficher_grille(g);

    // =========================
    // test N4
    // =========================
    printf("\nTest N4 voisins de (%d,%d)\n", p.ligne, p.colonne);

    nb = n4_voisins(g, p, voisins);

    printf("Nombre de voisins : %d\n", nb);
    for (i = 0; i < nb; i++) {
        printf("voisin %d : (%d,%d)\n", i, voisins[i].ligne, voisins[i].colonne);
    }

//     =========================
    //   test cases vides adj à un arbre
    // =========================
    if (case_est_tree(g, p)) {
        printf("\nTest cases vides voisines de larbre (%d,%d)\n", p.ligne, p.colonne);

        nb = est_caseVide_Adj(g, p, voisins);

        printf("Nombre cases vides voisines : %d\n", nb);
        for (i = 0; i < nb; i++) {
            printf("case vide %d : (%d,%d)\n", i, voisins[i].ligne, voisins[i].colonne);
        }
    } else {
        printf("\n(%d,%d) n'est pas un arbre -> pas test cases_vides_adj \n", p.ligne, p.colonne);
    }

    // =========================
    // test arbres voisins
    // =========================
    printf("\nTest arbres voisins de (%d,%d)\n", p.ligne, p.colonne);

    nb = adj_trees(g, p, voisins);

    printf("Nombre d'arbres voisins : %d\n", nb);
    for (i = 0; i < nb; i++) {
        printf("arbre %d : (%d,%d)\n", i, voisins[i].ligne, voisins[i].colonne);
    }

    free_grille(g);
    return 0;
}
