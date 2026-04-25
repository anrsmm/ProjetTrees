#include <stdio.h>

#include "cnf.h"
#include "3sat.h"

int main(int argc, char *argv[]) {
    CNFformule f;
    CNFformule f3;
    int nb_vars;
    int ok;

    if (argc < 3) {
        printf("Usage: %s <entree.cnf> <sortie_3sat.cnf>\n", argv[0]);
        return 1;
    }

    f = lire_dimacs(argv[1], &nb_vars);

    printf("Lecture DIMACS :\n");
    printf("Variables initiales = %d\n", nb_vars);
    printf("Clauses initiales = %d\n", f.num);

    f3 = transform_Formule3sat(&f, &nb_vars);

    printf("\nApres transformation 3-SAT :\n");
    printf("Variables finales = %d\n", nb_vars);
    printf("Clauses finales = %d\n", f3.num);

    ok = ecrire_dimacs3sat(argv[2], &f3, nb_vars);
    if (!ok) {
        printf("Erreur ecriture fichier 3-SAT\n");
        free_cnfFormule(&f3);
        free_cnfFormule(&f);
        return 1;
    }

    printf("\nFichier 3-SAT ecrit : %s\n", argv[2]);

    free_cnfFormule(&f3);
    free_cnfFormule(&f);

    return 0;
}
