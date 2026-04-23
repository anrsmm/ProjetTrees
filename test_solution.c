#include <stdio.h>
#include <stdlib.h>

#include "grille.h"
#include "cnf.h"
#include "sat.h"
#include "contraintes.h"
#include "contraintesCard.h"
#include "dimacs.h"

int main(int argc, char *argv[]) {
    Grille *g;
    SATmap *m;
    CNFformule f;
    int *val;
    int ok;
    int nb_vars_tentes;

    if (argc < 3) {
        printf("Usage: %s <grille.txt> <resultat.txt>\n", argv[0]);
        return 1;
    }

    g = lire_grille_fichier(argv[1]);
    if (g == NULL) {
        printf("Erreur lecture grille\n");
        return 1;
    }

    m = creer_mapSAT(g);
    if (m == NULL) {
        printf("Erreur creation mapping SAT\n");
        free_grille(g);
        return 1;
    }

    f = creer_cnfFormule();
    if (f.clauses == NULL) {
        printf("Erreur creation formule CNF\n");
        free_mapSAT(m);
        free_grille(g);
        return 1;
    }

    construire_cnf_complete(g, m, &f);

    nb_vars_tentes = g->Hauteur * g->Largeur;

    val = calloc(nb_vars_tentes + 1, sizeof(int));
    if (val == NULL) {
        free_cnfFormule(&f);
        free_mapSAT(m);
        free_grille(g);
        return 1;
    }

    ok = lire_solution(argv[2], val, nb_vars_tentes);
    if (!ok) {
        printf("Erreur lecture solution SAT\n");
        free(val);
        free_cnfFormule(&f);
        free_mapSAT(m);
        free_grille(g);
        return 1;
    }

    printf("Grille initiale :\n");
    afficher_grille(g);

    printf("\nGrille solution :\n");
    afficher_solution(g, val);

    free(val);
    free_cnfFormule(&f);
    free_mapSAT(m);
    free_grille(g);

    return 0;
}
