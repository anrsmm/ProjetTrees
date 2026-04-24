#ifndef TROIS_SAT_H
#define TROIS_SAT_H
#include "cnf.h"

//fonction DIMACS ->LECT MEMOIRE
CNFformule lire_dimacs(const char *nmfichier, int *nb_var);
int ecrire_dimacs3sat(const char *nm_fichier, CNFformule *f, int nb_var);

void transform_Clause3sat(Clause *cl, CNFformule *f3, int *next_var);
CNFformule transform_Formule3sat(CNFformule *f, int *total_vars);

#endif
