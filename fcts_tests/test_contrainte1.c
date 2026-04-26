#include <stdio.h>
#include <stdlib.h>

#include "grille.h"
#include "cnf.h"
#include "sat.h"
#include "contraintes.h"
#include "voisinage.h"

/*
test contrainte1 (arbre => non tente).
 */
int main(int argc, char *argv[]) {
    Grille *g;
    CNFformule f;
    int i, j;

    if (argc < 2) {
        printf("Usage: %s <fichier>\n", argv[0]);
        return 1;
    }

    g = lire_grille_fichier(argv[1]);
    if (g == NULL) {
        printf("Erreur lecture fichier\n");
        return 1;
    }

    f = creer_cnfFormule();
    if (f.clauses == NULL) {
        printf("Erreur creation CNF\n");
        free_grille(g);
        return 1;
    }

    afficher_grille(g);

    contrainte1(g, &f);

    printf("\nNombre de clauses = %d\n", f.num);

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
