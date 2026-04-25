#include <stdio.h>

#include "3sat.h"
#include "cnf.h"

int main(int argc, char *argv[]) {
    CNFformule f;
    int nb_vars;
    int i, j;

    if (argc < 2) {
        printf("Usage: %s <fichier.cnf>\n", argv[0]);
        return 1;
    }

    f = lire_dimacs(argv[1], &nb_vars);

    printf("Nombre de variables = %d\n", nb_vars);
    printf("Nombre de clauses lues = %d\n", f.num);

    for (i = 0; i < f.num; i++) {
        printf("Clause %d : ", i);
        for (j = 0; j < f.clauses[i].taille; j++) {
            printf("%d ", f.clauses[i].litt[j]);
        }
        printf("0\n");
    }

    free_cnfFormule(&f);

    return 0;
}
