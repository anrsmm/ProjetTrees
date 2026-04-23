#include <stdio.h>
#include <stdlib.h>
#include "cnf.h"
#include "contraintesCard.h"

int main(void) {
    CNFformule f;
    int vars[4] = {1, 2, 3, 4};
    int i, j;

    f = creer_cnfFormule();
    if (f.clauses == NULL) {
        printf("Erreur creation formule CNF\n");
        return 1;
    }

    ajout_au_moins_k(&f, vars, 4, 2);

    printf("Nombre de clauses = %d\n", f.num);

    for (i = 0; i < f.num; i++) {
        printf("Clause %d : ", i);
        for (j = 0; j < f.clauses[i].taille; j++) {
            printf("%d ", f.clauses[i].litt[j]);
        }
        printf("\n");
    }

    free_cnfFormule(&f);
    return 0;
}
