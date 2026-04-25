#include <stdlib.h>


#include "contraintes.h"
#include "contraintesCard.h"
#include "sat.h"
#include "grille.h"
#include "cnf.h"


/*

CONTRAINTE 1 :
Une case contenant un arbre ne peut pas contenur de tente.

*/

// CONTRAINTE 1 :
// Si une case contient un arbre, alors cette case ne peut pas être une tente.
// On ajoute donc la clause unitaire : (¬T_case) pour chaque arbre.
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
		// id représente ici la variable booléenne T_(i,j) :
		// "la case (i,j) contient une tente".

		Clause clause = creer_clause();
		if (clause.litt == NULL){
			return;
		}

		// Clause unitaire : (¬T_(i,j)).
		// En CNF, une clause unitaire impose directement la valeur d'une variable.
		ajout_LittClause(&clause, -id);
		ajout_clauseFormule(f, clause);
	}
}

// CONTRAINTE 2 :
// Si une variable d'association A_{arbre,case} est vraie,
// alors la case correspondante doit effectivement contenir une tente.
// Forme logique : A_{t,c} => T_c, donc (¬A_{t,c} v T_c).
void contrainte2(Grille *g, SATmap *m, CNFformule *f){
	int idA;
	int idT;
	if (g==NULL || f==NULL || m==NULL){
		return;
	}
	Position c;
	Clause clause;
	for (int i=0; i<m->assoc_var_num; i++){
		// idA = variable d'association A_{arbre,case}
		// idT = variable "la case contient une tente"
		idA= m->assoc_vars[i].dimacs_id;
		c =m->assoc_vars[i].case_vide;
		idT= tente_var(g,c);
		// On code l'implication A -> T sous forme CNF :
		// A -> T  <=>  (¬A v T).

		clause =creer_clause();
		if (clause.litt ==NULL){
			return;
		}

		ajout_LittClause(&clause, -idA);
		ajout_LittClause(&clause, idT);
		// Pourquoi cette implication :
		// si on dit "cet arbre est lié à cette case", la case doit être une tente.
		ajout_clauseFormule(f, clause);
	}

}


// CONTRAINTE 3 :
// Si une case vide c contient une tente, alors cette tente doit être liée
// à au moins un arbre voisin (adjacent en 4-voisinage).
// Forme logique : T_c => (A_{t1,c} v A_{t2,c} v ...).
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
		// adj_trees retourne uniquement les arbres en voisinage 4.
		// Donc la suite respecte bien la règle "une tente est orthogonalement
		// adjacente à un arbre".

		clause = creer_clause();
		if (clause.litt ==NULL){
			return;
		}
		ajout_LittClause(&clause, -idT);
		// Début de la forme : (¬T_c v ...)

		for (int j=0; j<nb_arbres; j++){
			idA=assoc_var(m, arbres_voisins[j], c);
			ajout_LittClause(&clause, idA);
			// ... on ajoute toutes les associations possibles A_(arbre_j,c)
			// pour obtenir : (¬T_c v A1 v A2 v ...).
		}
		// Sens de la clause : une tente "isolée" (sans arbre voisin lié) est interdite.
		ajout_clauseFormule(f,clause);
	}
}

// CONTRAINTE 4 :
// Chaque arbre doit être associé à au moins une case voisine vide
// (donc à au moins une tente candidate).
// On crée une clause "au moins un" sur les associations possibles de cet arbre.
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
		// Pour un arbre donné, on récupère toutes les cases vides voisines
		// candidates à une association.

		clause = creer_clause();
		if (clause.litt ==NULL){
			return;
		}

		for (int j=0; j<nb_voisins_vides; j++){
			idA=assoc_var(m, tree, voisins[j]);
			ajout_LittClause(&clause, idA);
			// Clause de type "au moins un" :
			// (A_(t,c1) v A_(t,c2) v ...).
		}
		// Cette clause garantit "au moins une tente" autour de chaque arbre.
		ajout_clauseFormule(f, clause);
	}

}


// CONTRAINTE 5 :
// Un arbre ne peut pas être relié à deux tentes différentes en même temps.
// Pour chaque paire d'associations (A1, A2) d'un même arbre, on ajoute :
// (¬A1 v ¬A2)  -> "au plus une association" par arbre.
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
				// j<k pour parcourir chaque paire une seule fois.
				idA1=assoc_var(m, tree, voisins[j]);
				idA2=assoc_var(m, tree, voisins[k]);
				// Pour "au plus un", on interdit toutes les paires vraies.
				// C'est la conversion classique :
				// au plus un parmi {A1, A2, ..., An}
				// <=> pour tout couple (Ai, Aj), (¬Ai v ¬Aj).

				clause= creer_clause();
				if (clause.litt==NULL){
					return;
				}
				ajout_LittClause(&clause, -idA1);
				ajout_LittClause(&clause, -idA2);
				// "Pas les deux en même temps" => au plus une tente liée à cet arbre.
				ajout_clauseFormule(f, clause);
			}
		}

	}


}

// CONTRAINTE 6 :
// Une case tente ne peut pas être associée à deux arbres différents.
// Pour chaque paire d'associations vers la même case, on ajoute :
// (¬A1 v ¬A2)  -> "au plus un arbre" par case tente.
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
				// Même principe que contrainte5, mais vu du côté "case".
				idA1=assoc_var(m, arbres_voisins[j], c);
				idA2=assoc_var(m, arbres_voisins[k], c);
				// Ici on impose l'unicité de l'arbre "propriétaire" de la tente.
				// Une tente choisit au plus un arbre.

				clause =creer_clause();
				if (clause.litt ==NULL){
					return;
				}
				ajout_LittClause(&clause, -idA1);
				ajout_LittClause(&clause, -idA2);
				// Une case tente est liée à un seul arbre (sinon ambiguïté).
				ajout_clauseFormule(f, clause);
			}
		}
	}

}

/*
FONCTION DE COMPARAISON
*/
// Sert à éviter les doublons dans la contrainte 7 :
// on ne traite qu'un seul ordre pour la paire (c,d).
int comparPosition(Position a, Position b) {
    if (a.ligne < b.ligne) {
        return 1;
    }
    if (a.ligne == b.ligne && a.colonne < b.colonne) {
        return 1;
    }
    return 0;
}




// CONTRAINTE 7 :
// Deux tentes ne peuvent pas se toucher, même en diagonale (8-voisinage).
// Pour chaque paire de cases vides voisines (c,d), on ajoute :
// (¬T_c v ¬T_d).
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
			// On ne traite la paire (c,d) qu'une fois pour éviter les doublons
			// ((c,d) et (d,c) donneraient la même clause).
			if (comparPosition(c,d)){
				idTc=tente_var(g,c);
				idTd=tente_var(g,d);
				// On ajoute une clause binaire d'exclusion locale :
				// (¬T_c v ¬T_d)
				// qui interdit deux tentes voisines en 8-voisinage.

				clause = creer_clause();
				if (clause.litt ==NULL){
					return;
				}
			ajout_LittClause(&clause, -idTc);
			ajout_LittClause(&clause, -idTd);
			// Interdit deux tentes voisines (horizontales, verticales, diagonales).
			ajout_clauseFormule(f, clause);

			}
		}
	}


}

//////////////////////////////////////////////////////////
/*
FONCTION CONSTRUCTION CNF 
*/
// Fonction principale : construit toute la formule CNF du puzzle.
// Elle regroupe les contraintes structurelles (1..7) + compteurs lignes/colonnes.
void construire_cnf_complete(Grille *g, SATmap *m, CNFformule *f) {
    if (g == NULL || m == NULL || f == NULL) {
        return;
    }

    // 1) Cohérence case/arbre/tente
    contrainte1(g, f);
    // 2) Liens association -> tente
    contrainte2(g, m, f);
    // 3) Tente -> au moins un arbre voisin
    contrainte3(g, m, f);
    // 4) Arbre -> au moins une tente
    contrainte4(g, m, f);
    // 5) Un arbre n'a pas deux tentes
    contrainte5(g, m, f);
    // 6) Une tente n'a pas deux arbres
    contrainte6(g, m, f);
    // 7) Deux tentes ne se touchent pas
    contrainte7(g, f);

    // 8) Contraintes de comptage (indices de la grille)
    contraintes_lignes(g, f);
    contraintes_colonnes(g, f);
}
