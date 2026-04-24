#ifndef DIMACS_H
#define DIMACS_H

#include "cnf.h"
#include "sat.h"

int ecrire_dimacs(const char *nom_fichier, CNFformule *f, SATmap *m);
int lire_solution(const char *nom, int *val, int nb_vars);
void afficher_solution(Grille *g, int *val);



// AFFICHAGE FINAL
int ecrire_donnees_grilles_js(const char *nom_fichier, Grille *g, int *val);

#endif
