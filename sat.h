#ifndef SAT_H
#define SAT_H

#include "grille.h"
#include "voisinage.h"

// type A_t,c
typedef struct {
	Position tree;
	Position case_vide;
	int dimacs_id;
} AssociationVar;


//correspondace SAT
typedef struct {
	AssociationVar *assoc_vars;
	int assoc_var_num;
	int total_var_num;
} SATmap;


//toutes cases mais on ne stocke pas tous les T, retourn identifiant DIMACS
int tente_var(Grille *g, Position p);

int compter_varsAssoc(Grille *g);// donne le nombre total de variables d assoc A_{t,c} de la grille
//creation structure pour
SATmap *creer_mapSAT(Grille *g);

void free_mapSAT(SATmap *m);

//pour retrouver id DIMACS d un A_{t,c}
int assoc_var(SATmap *m, Position tree, Position case_vide);

#endif

