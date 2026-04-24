#include <stdio.h>
#include <stdlib.h>

#include "grille.h"
#include "sat.h"
#include "dimacs.h"

int main(int argc, char *argv[]) {
    Grille *g;
    SATmap *m;
    int *val;

    if (argc < 3) {
        printf("Usage: %s <grille.txt> <resultat.txt>\n", argv[0]);
        return 1;
    }

    g = lire_grille_fichier(argv[1]);
    m = creer_mapSAT(g);

    val = calloc(m->total_var_num + 1, sizeof(int));

    if (!lire_solution(argv[2], val, m->total_var_num)) {
        printf("Erreur lecture solution\n");
        return 1;
    }

    printf("Solution :\n");
    afficher_solution(g, val);

    free(val);
    free(m);
    free_grille(g);

    return 0;
}
