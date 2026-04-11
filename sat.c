#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sat.h"
#include "voisinage.h"

int tente_var(Grille *g, Position p){
	if (g == NULL) {
	//	printf("Erreur : grille NULL\n");
		return 0;
	}
	if (!pos_valide(g, p.ligne, p.colonne)){
	//	printf("Erreur : position invalide (%d, %d)\n", p.ligne, p.colonne);
		return 0;
	}
	int idT;
	idT = p.ligne * g->Largeur + p.colonne +1;

	return idT;
}

//
int compter_varsAssoc(Grille *g){
    if (g == NULL) {
        return 0;
    }

    int total =0;
    int nb_voisinsVides;
    Position voisins[4];
    for (int i = 0; i < g->tree_nbr; i++) {
        nb_voisinsVides = est_caseVide_Adj(g, g->trees[i], voisins);
        total += nb_voisinsVides;
    }

    return total;
} 


SATmap *creer_mapSAT(Grille *g){
	if (g==NULL){
		return NULL;
	}

	SATmap *m;
	m = malloc(sizeof(SATmap));
	if (m==NULL) {
		return NULL;
	}

	int id_courant;

	m->assoc_var_num = compter_varsAssoc(g);
	m->total_var_num = g->Hauteur * g->Largeur + m->assoc_var_num;


	m->assoc_vars = malloc( m->assoc_var_num * sizeof(AssociationVar));
	if (m->assoc_vars == NULL){
		free(m);
		return NULL;

	}
	id_courant = g->Hauteur * g->Largeur +1;

	//remplissage du tableau

	int nb_voisinsVides;
	int index = 0;
	Position voisins[4];

	for (int i = 0; i < g->tree_nbr; i++) {
        	nb_voisinsVides = est_caseVide_Adj(g, g->trees[i], voisins);

        	for (int j = 0; j < nb_voisinsVides; j++) {
            		m->assoc_vars[index].tree = g->trees[i];
            		m->assoc_vars[index].case_vide = voisins[j];
            		m->assoc_vars[index].dimacs_id = id_courant;

            		index++;
            		id_courant++;
        	}
    	}

	return m;
}

//liberation 
void free_mapSAT(SATmap *m){
	if (m==NULL){
		return;
	}
	free(m->assoc_vars);
	free(m);
}

int assoc_var(SATmap *m, Position tree, Position case_vide){
	if (m==NULL){
		return 0;
	}

	for (int i=0;i < m->assoc_var_num; i++) {
		if (m->assoc_vars[i].tree.ligne == tree.ligne && 
			m->assoc_vars[i].tree.colonne == tree.colonne && 
			m->assoc_vars[i].case_vide.ligne == case_vide.ligne &&
			m->assoc_vars[i].case_vide.colonne == case_vide.colonne) {
			return m->assoc_vars[i].dimacs_id;
		}
	}
	return 0;
}

