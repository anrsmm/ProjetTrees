#include <stdio.h>

#include "cnf.h"
#include "3sat.h"

int main(int argc, char *argv[]) {
    CNFformule f;
    CNFformule f3;
    int nb_vars;

    if (argc < 3) {
        printf("Usage: %s <input.cnf> <output.cnf>\n", argv[0]);
        return 1;
    }

    f = lire_dimacs(argv[1], &nb_vars);

    f3 = transform_Formule3sat(&f, &nb_vars);

    ecrire_dimacs3sat(argv[2], &f3, nb_vars);

    free_cnfFormule(&f3);
    free_cnfFormule(&f);

    return 0;
}
