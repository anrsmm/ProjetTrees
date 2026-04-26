#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grille.h"
#include "sat.h"
#include "cnf.h"
#include "contraintes.h"
#include "contraintesCard_IA.h"
#include "dimacs.h"

/* Version IA de resolutionTents.c

Changements principaux :
- verifier si MiniSat renvoie SAT avant de lire la solution ;
- eviter d'afficher une solution si le probleme est UNSAT ;
- allouer assez de memoire pour les variables auxiliaires
  creees par contraintesCard_IA.c   */




static int solution_est_sat(const char *nom_fichier)
{
    FILE *f;
    char mot[256];

    f = fopen(nom_fichier, "r");
    if (f == NULL) {
        return 0;
    }

    if (fscanf(f, "%255s", mot) != 1) {
        fclose(f);
        return 0;
    }

    fclose(f);

    if (strcmp(mot, "SAT") == 0 || strcmp(mot, "SATISFIABLE") == 0) {
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    Grille *g;
    SATmap *m;
    CNFformule f;
    int *val;
    int nb_variables;

    if (argc < 2) {
        printf("Usage: %s <grille.txt>\n", argv[0]);
        return 1;
    }

    /* Lecture de la grille */
    g = lire_grille_fichier(argv[1]);
    if (g == NULL) {
        printf("Erreur : impossible de lire la grille\n");
        return 1;
    }

    /* Creation de la map SAT */
    m = creer_mapSAT(g);
    if (m == NULL) {
        printf("Erreur : impossible de creer la map SAT\n");
        free_grille(g);
        return 1;
    }

    /* Creation de la formule CNF */
    f = creer_cnfFormule();

    /* Construction de toutes les contraintes */
    construire_cnf_complete(g, m, &f);

    /* Ecriture du fichier DIMACS */
    ecrire_dimacs("temp.cnf", &f, m);

    /* Appel de MiniSat */
    system("minisat temp.cnf temp.out");

    /* Si MiniSat renvoie UNSAT, on ne lit pas de solution.
    Cela evite d'afficher une fausse grille ou de provoquer une erreur. */
    if (!solution_est_sat("temp.out")) {
        printf("Pas de solution SAT.\n");

        free_cnfFormule(&f);
        free_mapSAT(m);
        free_grille(g);

        return 0;
    }

    /*  On recupere le plus grand numero de variable utilise.
    Avec la version IA, il y a des variables auxiliaires en plus  */
    nb_variables = nombre_variables_cardinalite();




    /*     On compare le nombre de variables donne par le compteur sequentiel
           avec celui deja connu dans la map SAT du projet.

           Le but est de garder la plus grande valeur, afin d'allouer un tableau
           assez grand pour stocker toutes les variables de la solution MiniSat
           (variables normales + variables auxiliaires)   */

    if (nb_variables < m->total_var_num) {
        nb_variables = m->total_var_num;
    }



    /* Allocation du tableau de valuation.
    Les variables DIMACS commencent a 1, donc on ajoute 1 */
    val = calloc(nb_variables + 1, sizeof(int));
    if (val == NULL) {
        printf("Erreur allocation memoire\n");

        free_cnfFormule(&f);
        free_mapSAT(m);
        free_grille(g);

        return 1;
    }

    /* Lecture de la solution MiniSat */
    lire_solution("temp.out", val, nb_variables);

    /* Affichage terminal */
    printf("Solution :\n");
    afficher_solution(g, val);

    /* Creation du fichier pour l'affichage Python */
    if (ecrire_donnees_grilles_json("donnees_grilles.json", g, val)) {
        printf("\nFichier cree : donnees_grilles.json\n");
    } else {
        printf("\nErreur : impossible de creer donnees_grilles.json\n");
    }

    /* Liberation memoire */
    free(val);
    free_cnfFormule(&f);
    free_mapSAT(m);
    free_grille(g);

    return 0;
}