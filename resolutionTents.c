#include <stdio.h>
#include <stdlib.h>

#include "grille.h"
#include "sat.h"
#include "cnf.h"
#include "contraintes.h"
#include "dimacs.h"

/*
resolution de la grille (grille -> CNF -> minisat) et generation du fichier JSON
 */
int main(int argc, char *argv[]) {
    Grille *g;
    SATmap *m;
    CNFformule f;
    int *val;

    if (argc < 2) {
        printf("Usage: %s <grille.txt>\n", argv[0]);
        return 1;
    }

    // init structures
    g = lire_grille_fichier(argv[1]);
    m = creer_mapSAT(g);
    f = creer_cnfFormule();



    // generation CNF + solveur
//generation de la CNF
    construire_cnf_complete(g, m, &f);
//ecriture de la CNF dans un fichier
    ecrire_dimacs("temp.cnf", &f, m);
//execution du solveur SAT
    system("minisat temp.cnf temp.out");

//lecture de la solution SAT
    val = calloc(m->total_var_num + 1, sizeof(int));//all solution SAT
    lire_solution("temp.out", val, m->total_var_num);//lecture de la solution SAT


    //affichage de la solution
    printf("Solution :\n");
    afficher_solution(g, val);



//ecriture de la solution dans un fichier JSON
    if (ecrire_donnees_grilles_json("donnees_grilles.json", g, val)) {//ecriture de la solution dans un fichier JSON
        printf("\nFichier cree : donnees_grilles.json\n");
    } else {
        printf("\nErreur : impossible de creer donnees_grilles.json\n");
    }
//liberation 
    free(val);
    free_cnfFormule(&f);
    free_mapSAT(m);
    free_grille(g);

    return 0;
}
