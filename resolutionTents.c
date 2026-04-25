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
    int *val;

    if (argc < 2) {
        printf("Usage: %s <grille.txt>\n", argv[0]);
        return 1;
    }

    g = lire_grille_fichier(argv[1]);
    m = creer_mapSAT(g);
    f = creer_cnfFormule();

    construire_cnf_complete(g, m, &f);

    ecrire_dimacs("temp.cnf", &f, m);

    system("minisat temp.cnf temp.out");

    val = calloc(m->total_var_num + 1, sizeof(int));

    lire_solution("temp.out", val, m->total_var_num);

    printf("Solution :\n");
    afficher_solution(g, val);

    if (ecrire_donnees_grilles_json("donnees_grilles.json", g, val)) {
        printf("\nFichier cree : donnees_grilles.json\n");
    } else {
        printf("\nErreur : impossible de creer donnees_grilles.json\n");
    }

    free(val);
    free_cnfFormule(&f);
    free_mapSAT(m);
    free_grille(g);

    return 0;
}
