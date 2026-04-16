#include <stdio.h>
#include <stdlib.h>
#include "cnf.h"

int main(void) {
    Clause c;
    CNFformule f;
    int i, j;

    // =========================
    // TEST creer_cnfFormule
    // =========================
    f = creer_cnfFormule();

    if (f.clauses == NULL) {
        printf("Erreur creation formule CNF\n");
        return 1;
    }

    printf("=== Test creer_cnfFormule ===\n");
    printf("num = %d\n", f.num);
    printf("max = %d\n", f.max);

    // =========================
    // TEST ajout_litt_clause
    // =========================
    c = creer_clause();

    if (c.litt == NULL) {
        printf("Erreur creation clause\n");
        free_cnfFormule(&f);
        return 1;
    }

    printf("\n=== Test ajout_litt_clause ===\n");

    ajout_LittClause(&c, -14);
    ajout_LittClause(&c, 8);
    ajout_LittClause(&c, 3);
    ajout_LittClause(&c, -2);
    ajout_LittClause(&c, 20);

    printf("taille = %d\n", c.taille);
    printf("max = %d\n", c.max);

    for (i = 0; i < c.taille; i++) {
        printf("litt[%d] = %d\n", i, c.litt[i]);
    }

    // =========================
    // TEST ajout_clause_formule
    // =========================
    ajout_clauseFormule(&f, c);

    printf("\n=== Test ajout_clause_formule ===\n");
    printf("num = %d\n", f.num);
    printf("max = %d\n", f.max);

    for (i = 0; i < f.num; i++) {
        printf("Clause %d : ", i);
        for (j = 0; j < f.clauses[i].taille; j++) {
            printf("%d ", f.clauses[i].litt[j]);
        }
        printf("\n");
    }

    // =========================
    // TEST free_cnfFormule
    // =========================
    free_cnfFormule(&f);

    printf("\n=== Test free_cnfFormule ===\n");
    printf("num = %d\n", f.num);
    printf("max = %d\n", f.max);
    printf("clauses = %p\n", (void *)f.clauses);

    return 0;
}
