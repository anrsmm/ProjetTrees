#include <stdio.h>
#include <stdlib.h>

#include "grille.h"
#include "sat.h"
#include "dimacs.h"

/*
lecture de la solution SAT et affichage de la grille resolue
 */
int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Usage: %s <grille.txt> <resultat.txt>\n", argv[0]);
        return 1;
    }

    Grille *g;
    SATmap *m;
    g = lire_grille_fichier(argv[1]);//lecture de la grille
    m = creer_mapSAT(g);

    int *val;//solution SAT
    val = calloc(m->total_var_num + 1, sizeof(int));

    if (!lire_solution(argv[2], val, m->total_var_num)) {
        printf("Erreur lecture solution\n");
        return 1;
    }

    printf("Solution :\n");//affic
    afficher_solution(g, val);

    //liberation
    free(val);
    free(m);
    free_grille(g);

    return 0;
}
