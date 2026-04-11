#ifndef SAT_H
#define SAT_H

#include "grille.h"

// type A_t,c
typedef struct {
	Position tree;
	Position case_vide;
	int dimacs_id;
} AssociationVar


//correspondace SAT
typedef struct {
	AssociationVar *assoc_vars;
	int assoc_var_num;
	int total_var_num;
} SATmap;


//toutes cases mais on ne stocke pas tous les T, retourn identifiant DIMACS
int tente_var(Grille *g, Position p);

//elle parcours le tableau assoc_vars et renvoie dimacs_id pour le bon couple
//int assoc_var_id( Position tree, Position case, AssociationVar *assoc_vars, int assoc_var_num);

#endif

