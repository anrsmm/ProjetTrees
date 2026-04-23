#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cnf.h"
#include "sat.h"

int ecrire_dimacs(const char *nom_fichier, CNFformule *f, SATmap *m) {
    FILE *out;
    int i, j;

    if (nom_fichier == NULL || f == NULL || m == NULL) {
        return 0;
    }

    out = fopen(nom_fichier, "w");
    if (out == NULL) {
        return 0;
    }

    fprintf(out, "p cnf %d %d\n", m->total_var_num, f->num);

    for (i = 0; i < f->num; i++) {
        for (j = 0; j < f->clauses[i].taille; j++) {
            fprintf(out, "%d ", f->clauses[i].litt[j]);
        }
        fprintf(out, "0\n");
    }

    fclose(out);
    return 1;
}


int lire_solution(const char *nom, int *val, int nb_vars) {
    FILE *f = fopen(nom, "r");
    char buffer[10];
    int x;

    if (nom == NULL || val == NULL) {
        return 0;
    }

    if (f == NULL) {
        return 0;
    }

    if (fscanf(f, "%9s", buffer) != 1) {
        fclose(f);
        return 0;
    }

    if (strcmp(buffer, "SAT") != 0) {
        fclose(f);
        return 0;
    }

    while (fscanf(f, "%d", &x) == 1 && x != 0) {
        int var = abs(x);
        if (var <= nb_vars) {
            val[var] = (x > 0) ? 1 : 0;
        }
    }

    fclose(f);
    return 1;
}

void afficher_solution(Grille *g, int *val) {
    int i, j;

    for (i = 0; i < g->Hauteur; i++) {
        for (j = 0; j < g->Largeur; j++) {
            Position p = {i, j};

            if (case_est_tree(g, p)) {
                printf("T ");
            } else {
                int var = tente_var(g, p);

                if (val[var] == 1) {
                    printf("X ");
                } else {
                    printf(". ");
                }
            }
        }
        printf("\n");
    }
}

