#include <stdlib.h>


#include "contraintes.h"
#include "sat.h"
#include "grille.h"
#include "cnf.h"


/*

CONTRAINTE 1 :
Une case contenant un arbre ne peut pas contenur de tente.

*/

//si une case est un arbre alors la variable tente doit être fausse
void contrainte1(Grille *g, CNFformule *f){
	if (g==NULL || f == NULL){
		return;
	}

	for (int i = 0; i < g->tree_nbr; i++){
		//Position arbre = g->est_tree[i]; -> il faut parcourir les arbres
// est_tree repond a question - est ce qu il y a un arbre en (i,j) ? et 
// trees sert a parcourir - quels sont tous les arbres?

		Position arbre = g->trees[i];
		int id = tente_var(g, arbre);

		Clause clause = creer_clause();
		if (clause.litt == NULL){
			return;
		}

		ajout_LittClause(&clause, -id);
		ajout_clauseFormule(f, clause);
	}
}

void contrainte2(Grille *g, SATmap *m, CNFformule *f){
	int idA;
	int idT;
	if (g==NULL || f==NULL || m==NULL){
		return;
	}
	Position c;
	Clause clause;
	for (int i=0; i<m->assoc_var_num; i++){
		idA= m->assoc_vars[i].dimacs_id;
		c =m->assoc_vars[i].case_vide;
		idT= tente_var(g,c);

		clause =creer_clause();
		if (clause.litt ==NULL){
			return;
		}

		ajout_LittClause(&clause, -idA);
		ajout_LittClause(&clause, idT);
		ajout_clauseFormule(f, clause);
	}

}


void contrainte3(Grille *g, SATmap *m, CNFformule *f){
	if (g==NULL || m ==NULL || f==NULL){
		return;
	}
	Position c;
	int idT;
	int nb_arbres;
	Position arbres_voisins[4];
	Clause clause;
	int idA;

	for (int i=0; i<g->empty_nbr; i++){
		c =g->empty_cases[i];
		idT=tente_var(g,c);
		nb_arbres = adj_trees(g,c, arbres_voisins);

		clause = creer_clause();
		if (clause.litt ==NULL){
			return;
		}
		ajout_LittClause(&clause, -idT);

		for (int j=0; j<nb_arbres; j++){
			idA=assoc_var(m, arbres_voisins[j], c);
			ajout_LittClause(&clause, idA);
		}
		ajout_clauseFormule(f,clause);
	}
}

void contrainte4(Grille *g, SATmap *m, CNFformule *f){
	if (g==NULL || m==NULL || f==NULL){
		return;
	}
	Position tree;
	int nb_voisins_vides;
	Position voisins[4];
	Clause clause;
	int idA;
	for (int i=0; i< g->tree_nbr; i++){
		tree = g->trees[i];
		nb_voisins_vides =est_caseVide_Adj(g, tree, voisins);

		clause = creer_clause();
		if (clause.litt ==NULL){
			return;
		}

		for (int j=0; j<nb_voisins_vides; j++){
			idA=assoc_var(m, tree, voisins[j]);
			ajout_LittClause(&clause, idA);
		}
		ajout_clauseFormule(f, clause);
	}

}


void contrainte5(Grille *g, SATmap *m, CNFformule *f){
	if (g==NULL || m==NULL ||f==NULL){
		return;
	}

	Position tree;
	int nb_voisins_vides;
	Position voisins[4];
	int idA1;
	int idA2;
	Clause clause;

	for (int i=0; i<g->tree_nbr; i++){
		tree= g->trees[i];
		nb_voisins_vides = est_caseVide_Adj(g, tree, voisins);
	
		for (int j=0; j<nb_voisins_vides; j++){
			for (int k=j+1; k<nb_voisins_vides; k++){
				idA1=assoc_var(m, tree, voisins[j]);
				idA2=assoc_var(m, tree, voisins[k]);

				clause= creer_clause();
				if (clause.litt==NULL){
					return;
				}
				ajout_LittClause(&clause, -idA1);
				ajout_LittClause(&clause, -idA2);
				ajout_clauseFormule(f, clause);
			}
		}

	}


}

void contrainte6(Grille *g, SATmap *m, CNFformule *f){
	if (g==NULL || f==NULL || m==NULL){
		return;
	}
	Position c;
	int nb_arbres;
	Position arbres_voisins[4];
	int idA1;
	int idA2;
	Clause clause;
	for (int i=0; i<g->empty_nbr; i++){
		c = g->empty_cases[i];
		nb_arbres = adj_trees(g, c, arbres_voisins);
		
		for (int j=0; j<nb_arbres; j++){
			for (int k=j+1; k<nb_arbres; k++){
				idA1=assoc_var(m, arbres_voisins[j], c);
				idA2=assoc_var(m, arbres_voisins[k], c);

				clause =creer_clause();
				if (clause.litt ==NULL){
					return;
				}
				ajout_LittClause(&clause, -idA1);
				ajout_LittClause(&clause, -idA2);
				ajout_clauseFormule(f, clause);
			}
		}
	}

}

/*
FONCTION DE COMPARAISON
*/
int comparPosition(Position a, Position b) {
    if (a.ligne < b.ligne) {
        return 1;
    }
    if (a.ligne == b.ligne && a.colonne < b.colonne) {
        return 1;
    }
    return 0;
}




void contrainte7(Grille *g, CNFformule *f){
	if (g==NULL || f==NULL){
		return;
	}
	Position c;
	int nb_voisins;
	Position voisins[8];
	Position d;
	Clause clause;
	int idTc;
	int idTd;

	for (int i=0; i<g->empty_nbr; i++){
		c = g->empty_cases[i];
		nb_voisins = n8_voisins(g,c, voisins);

		for (int j=0; j<nb_voisins; j++){
			d = voisins[j];

			if (!case_est_vide(g,d)){
				continue;
			}
			if (comparPosition(c,d)){
				idTc=tente_var(g,c);
				idTd=tente_var(g,d);

				clause = creer_clause();
				if (clause.litt ==NULL){
					return;
				}
			ajout_LittClause(&clause, -idTc);
			ajout_LittClause(&clause, -idTd);
			ajout_clauseFormule(f, clause);

			}
		}
	}


}
