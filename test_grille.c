#include <stdio.h>
#include "grille.h"

int main(void) {
    Grille *g;
    Position p1, p2, p3;

    g = creer_grille(4, 5);
    if (g == NULL) {
        printf("Erreur : creation de la grille impossible.\n");
        return 1;
    }

    printf("Grille creee : %d x %d\n", g->Hauteur, g->Largeur);

    // On place quelques arbres "a la main"
    g->est_tree[0][1] = 1;
    g->est_tree[1][3] = 1;
    g->est_tree[2][0] = 1;
    g->est_tree[3][2] = 1;

    p1.ligne = 0;
    p1.colonne = 1;   // arbre

    p2.ligne = 0;
    p2.colonne = 0;   // vide

    p3.ligne = 10;
    p3.colonne = 10;  // hors grille

    printf("pos_valide(0,1) = %d\n", pos_valide(g, 0, 1));
    printf("pos_valide(10,10) = %d\n", pos_valide(g, 10, 10));

    printf("case_est_tree(p1) = %d\n", case_est_tree(g, p1));
    printf("case_est_vide(p1) = %d\n", case_est_vide(g, p1));

    printf("case_est_tree(p2) = %d\n", case_est_tree(g, p2));
    printf("case_est_vide(p2) = %d\n", case_est_vide(g, p2));

    printf("case_est_tree(p3) = %d\n", case_est_tree(g, p3));
    printf("case_est_vide(p3) = %d\n", case_est_vide(g, p3));

    free_grille(g);
    return 0;
}
