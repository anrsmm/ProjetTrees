#include <stdio.h>
#include <stdlib.h>

#include "grille.h"
#include "cnf.h"
#include "sat.h"
#include "contraintesCard.h"

int main(int argc, char *argv[]) {
    Grille *g;
    CNFformule f;
    int i, j;
    int ligne;

    if (argc < 3) {
        printf("Usage: %s <fichier> <ligne>\n", argv[0]);
        return 1;
    }

    g = lire_grille_fichier(argv[1]);
    if (g == NULL) {
        printf("Erreur lecture fichier\n");
        return 1;
    }

    ligne = atoi(argv[2]);

    f = creer_cnfFormule();
    if (f.clauses == NULL) {
        printf("Erreur creation formule CNF\n");
        free_grille(g);
        return 1;
    }

    afficher_grille(g);

    printf("\nTest contrainte_ligne sur la ligne %d\n", ligne);
    printf("Quota = %d\n", g->ligne_nbr[ligne]);

    contrainte_ligne(g, &f, ligne);

    printf("Nombre de clauses = %d\n", f.num);

    for (i = 0; i < f.num; i++) {
        printf("Clause %d : ", i);
        for (j = 0; j < f.clauses[i].taille; j++) {
            printf("%d ", f.clauses[i].litt[j]);
        }
        printf("\n");
    }

    free_cnfFormule(&f);
    free_grille(g);

    return 0;
}
