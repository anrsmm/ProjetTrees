#include <stdio.h>
#include <stdlib.h>

#include "grille.h"
#include "sat.h"
#include "cnf.h"
#include "contraintes.h"
#include "dimacs.h"

int main(int argc, char *argv[]) {
    Grille *g;
    SATmap *m;
    CNFformule f;

    if (argc < 3) {
        printf("Usage: %s <grille.txt> <sortie.cnf>\n", argv[0]);
        return 1;
    }

    g = lire_grille_fichier(argv[1]);
    m = creer_mapSAT(g);
    f = creer_cnfFormule();

    construire_cnf_complete(g, m, &f);

    if (!ecrire_dimacs(argv[2], &f, m)) {
        printf("Erreur ecriture DIMACS\n");
    }

    free_cnfFormule(&f);
    free(m);
    free_grille(g);

    return 0;
}
