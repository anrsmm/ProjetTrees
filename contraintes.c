#include <stdlib.h>


#include "contraintes.h"
#include "contraintesCard.h"
#include "sat.h"
#include "grille.h"
#include "cnf.h"


/*
CONTRAINTE 1 : Une case contenant un arbre ne peut pas contenir une tente.

Pour chaque case (i,j), on utilise une variable T_(i,j) : 
vrai si la case contient une tente, si un arbre alors T_(i,j) faux : ¬T_(i,j)

 Cela donne une clause unitaire pour chaque arbre
 */
void contrainte1(Grille *g, CNFformule *f){
	if (g==NULL || f == NULL){
		return;
	}

	for (int i = 0; i < g->tree_nbr; i++){
	/* 
	Position arbre = g->est_tree[i]; 
	il faut parcourir les arbres
	est_tree repond a question - est ce qu il y a un arbre en (i,j) ? et 
	trees sert a parcourir - quels sont tous les arbres?
	*/
		Position arbre = g->trees[i];
		int id = tente_var(g, arbre);
		/*
		id est la variable DIMACS associée à la case de l'arbre.
		Elle représente la proposition : "il y a une tente sur case (i,j)".
	*/
		Clause clause = creer_clause();
		if (clause.litt == NULL){
			return;
		}
		/*
		Comme la case contient un arbre elle ne peut pas avoir une tente.
		On ajoute littéral négatif (-id )qui correspond à ¬T_(i,j). */
		ajout_LittClause(&clause, -id);

		//ajout clause unitaire nonT_(i,j) à formule CNF
		ajout_clauseFormule(f, clause);
	}
}

/*
CONTRAINTE 2 :
Si association arbre->case est vraie alors la case doit être marquée comme tente.
rappel : 
- A_(t,c) : "l'arbre t est associé à la case c"
- T_c     : "la case c contient une tente"

on a que : A_(t,c) => T_c   donc   ¬A_(t,c) v T_c
 */
void contrainte2(Grille *g, SATmap *m, CNFformule *f){
	int idA;
	int idT;
	if (g==NULL || f==NULL || m==NULL){
		return;
	}
	Position c;
	Clause clause;
	for (int i=0; i<m->assoc_var_num; i++){
		/*
		 * Chaque m->assoc_vars[i] une association possible entre un A et une case vide voisine
		idA = variable dassoc  A_{arbre,case}
		idT = variable "la case contient une tente"
		*/
		idA= m->assoc_vars[i].dimacs_id;
		c =m->assoc_vars[i].case_vide;
		idT= tente_var(g,c);
		
		clause =creer_clause();
		if (clause.litt ==NULL){
			return;
		}
		/*
		On ajoute (¬A v T) :
		si A vrai, T vrai ;
		si A faux, la clause est satisf sans contrainte sur T.
		 */
		ajout_LittClause(&clause, -idA);
		ajout_LittClause(&clause, idT);
		// si on dit "cet arbre est lié à cette case", la case doit être une tente.

		ajout_clauseFormule(f, clause);
	}

}

/*
CONTRAINTE 3 :
Si une case vide contient une tente, alors cette tente doit être liée a au moins un arbre voisin (adj 4-voisin)

Pour chaque case vide c :
	T_c => (A_(t1,c) v A_(t2,c) v ... v A_(tk,c))
 */
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
		/*
		recuper arbres adj 4-voisin a la case c : peuvent être associés à une tente en c
adj_trees retourne uniquement les arbres en voisinage 4.
 Donc la suite respecte bien la règle "une tente est orthogonalement adjacente à un arbre".
*/
		clause = creer_clause();
		if (clause.litt ==NULL){
			return;
		}
		
		//On construit (¬T_c v ...), puis on complétera la clause avec toutes les associations possibles a partir des arbres voisins.

		ajout_LittClause(&clause, -idT);
		// debut forme (¬T_c v ...)

		for (int j=0; j<nb_arbres; j++){
			idA=assoc_var(m, arbres_voisins[j], c);
			ajout_LittClause(&clause, idA);
			// on ajoute toutes associations A_(arbre_j,c) pour avoir (nonT_c v A1 ...).
		}
		/* une tente sans arbre voisin lié : interdit
		 Si T_c est vrai au moins un litt A_(t,c) vrai
		 */
		ajout_clauseFormule(f,clause);
	}
}

/*
CONTRAINTE 4 : Chaque arbre doit être associé à au moins une case voisine vide
 
Pour un arbre t, si ses cases vides voisines sont c1..ck : (A_(t,c1)v ...vA_(t,ck)
au moins une association valide par arbre.
 */
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
		/*
		voisins[] contient cases vides adj a tree
		clause "au moins un" sur ces associations.
		 */
		clause = creer_clause();
		if (clause.litt ==NULL){
			return;
		}

		for (int j=0; j<nb_voisins_vides; j++){
			idA=assoc_var(m, tree, voisins[j]);
			/*chaque litt ajout correspond à une assoc possible A_(t,c) entre A et case vide voisine*/
			ajout_LittClause(&clause, idA);
			// clause "au moins un" : (A_(t,c 1)...).
		}
		/* Si aucun idA vrai clause fausse*/
		ajout_clauseFormule(f, clause);
	}

}

/*
CONTRAINTE 5 :
Un arbre ne peut être associé qu'à une seule tente.
pour chq paires d'assoc (A1, A2) d'un même arbre : (nona1 v nonA2) -> au plus une assoc par arbre.
 */
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
		
		//On enumere assoc possibles de cet arbre puis interdit chaque paire prise en meme temps.
		 
	
		for (int j=0; j<nb_voisins_vides; j++){
			for (int k=j+1; k<nb_voisins_vides; k++){
				// j<k pour parcourir chaque paire une seule fois.
				idA1=assoc_var(m, tree, voisins[j]);
				idA2=assoc_var(m, tree, voisins[k]);
				// Pour "au plus un", on interdit toutes les paires vraies
				// C'est la conversion classique : au plus un parmi {A1, A2, ..., An} donc pour tt (Ai, Aj), (nonAi v nonAj)
				clause= creer_clause();
				if (clause.litt==NULL){
					return;
				}
				
				//2 assoc diff pour le même arbre : interdit.
				ajout_LittClause(&clause, -idA1);
				ajout_LittClause(&clause, -idA2);
				// "Pas les deux en même temps" -> au plus une tente liee  arbre
				ajout_clauseFormule(f, clause);
			}
		}

	}


}

/*
CONTRAINTE 6 :
Une case tente ne peut appartenir qu'à un seul arbre.
"au plus un" côté case : pour case et deux arbres voisins t1, t2 : (nonA_(t1,c) v nonA_(t2,c))
On applique cette regle à toutes paires d'arbres voisins de c.
 */
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
		
		//On liste les arbres voisins de la case c (4-voisinage);interdit doubles assoc vers c
		
		for (int j=0; j<nb_arbres; j++){
			for (int k=j+1; k<nb_arbres; k++){
				// meme principe que contrainte5, mais vu du cote "case".
				idA1=assoc_var(m, arbres_voisins[j], c);
				idA2=assoc_var(m, arbres_voisins[k], c);
				// une tente choisit au plus un arbre.
				clause =creer_clause();
				if (clause.litt ==NULL){
					return;
				}

				ajout_LittClause(&clause, -idA1);
				ajout_LittClause(&clause, -idA2);
				// une case tente est liee a un seul arbre
				ajout_clauseFormule(f, clause);
			}
		}
	}

}

/*
FONCTION DE COMPARAISON
Sert à éviter les doublons (c,d);(d,c) dans la contrainte 7 :
on ne traite qu'un seul ordre pour la paire (c,d).

-vaut 1 si a est strictement "avant" b
dans l'ordre ligne puis colonne) 
- sinon 0

 */
int comparPosition(Position a, Position b) {
    // Ordre total: ligne puis colonne.
    if (a.ligne < b.ligne) {
        return 1;
    }
    if (a.ligne == b.ligne && a.colonne < b.colonne) {
        return 1;
    }
    return 0;
}


/*
CONTRAINTE 7 :
Deux tentes 8-voisin impossible
Pour chaque paire de cases vides voisines (c,d) : (nonT_c v nonT_d)
interdit cote gauche, droite, haut, bas, diagonal
 */
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
		//n8_voisins renvoie toutes les cases voisines de c (orthog + diag)


		for (int j=0; j<nb_voisins; j++){
			d = voisins[j];

			if (!case_est_vide(g,d)){
				continue;
			}
			// On traite la paire (c,d) qu'une fois pour eviter les doublons ((c,d) et (d,c) donneraient la meme clause)
			if (comparPosition(c,d)){
				idTc=tente_var(g,c);
				idTd=tente_var(g,d);
				// idTc et idTd sont les var DIMACS representant T_c et T_d
				// on ajoute une clause binaire d'exclusion locale : (nonT_c v nonT_d) qui interdit  2tentes voisines en 8-voisins.

				clause = creer_clause();
				if (clause.litt ==NULL){
					return;
				}
				// les deux tentes ne peuvent pas etre vraies en mm temps
				ajout_LittClause(&clause, -idTc);
				ajout_LittClause(&clause, -idTd);
				// Interdit 2 tentes voisines (horiz, vert, diag)
				ajout_clauseFormule(f, clause);

			}
		}
	}


}

//////////////////////////////////////////////////////////
/*
FONCTION CONSTRUCTION CNF 

Fonction principale : construit toute la formule CNF du jeu.
Elle regroupe les contraintes structurelles (1..7) + compteurs lignes/colonnes.
-> solveur SAT travaille sur cette formule CNF complète.
 */
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
