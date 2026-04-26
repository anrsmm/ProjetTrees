#include <stdio.h>
#include <stdlib.h>

#include "grille.h"
#include "cnf.h"
#include "sat.h"
#include "contraintes.h"
#include "contraintesCard.h"
#include "dimacs.h"

//test export DIMACS de la formule CNF.
int main(int argc, char *argv[]) {
    Grille *g;
    SATmap *m;
    CNFformule f;
    int ok;

    if (argc < 3) {
        printf("Usage: %s <fichier_entree> <fichier_sortie.cnf>\n", argv[0]);
        return 1;
    }

    g = lire_grille_fichier(argv[1]);
    if (g == NULL) {
        printf("Erreur lecture fichier\n");
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

    ok = ecrire_dimacs(argv[2], &f, m);

    if (!ok) {
        printf("Erreur ecriture fichier DIMACS\n");
    } else {
        printf("Fichier DIMACS ecrit : %s\n", argv[2]);
        printf("Nombre de variables = %d\n", m->total_var_num);
        printf("Nombre de clauses = %d\n", f.num);
    }

    free_cnfFormule(&f);
    free_mapSAT(m);
    free_grille(g);

    return 0;
}
