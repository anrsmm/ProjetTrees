#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "voisinage.h"


int n4_voisins(Grille *g, Position p, Position voisins[4]) {
    int nb = 0;
    Position q;

    if (g == NULL) {
        return 0;
    }

    if (!pos_valide(g, p.ligne, p.colonne)) {
        return 0;
    }

    // haut
    q.ligne = p.ligne - 1;
    q.colonne = p.colonne;
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q;
        nb++;
    }

    // bas
    q.ligne = p.ligne + 1;
    q.colonne = p.colonne;
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q;
        nb++;
    }

    // gauche
    q.ligne = p.ligne;
    q.colonne = p.colonne - 1;
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q;
        nb++;
    }

    // droite
    q.ligne = p.ligne;
    q.colonne = p.colonne + 1;
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q;
        nb++;
    }

    return nb;
}

int n8_voisins(Grille *g, Position p, Position voisins[8]) {
    int nb;
    Position q;

    if (g == NULL) {
        return 0;
    }

    if (!pos_valide(g, p.ligne, p.colonne)) {
        return 0;
    }

    // on commence par les voisins N4
    nb = n4_voisins(g, p, voisins);

    // haut-gauche
    q.ligne = p.ligne - 1;
    q.colonne = p.colonne - 1;
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q;
        nb++;
    }

    // haut-droite
    q.ligne = p.ligne - 1;
    q.colonne = p.colonne + 1;
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q;
        nb++;
    }

    // bas-gauche
    q.ligne = p.ligne + 1;
    q.colonne = p.colonne - 1;
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q;
        nb++;
    }

    // bas-droite
    q.ligne = p.ligne + 1;
    q.colonne = p.colonne + 1;
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q;
        nb++;
    }

    return nb;
}

//Adj(t)
int est_caseVide_Adj(Grille *g, Position tree, Position voisins[4]){
	if(g == NULL){
		return 0;
	}

	if (!pos_valide(g, tree.ligne, tree.colonne)){
		return 0;
	}

	if (!case_est_tree(g, tree)) {
		return 0;
	}
	int nb_n4;
	Position voisins_n4[4];
	nb_n4= n4_voisins(g, tree, voisins_n4);

	int nb_vides = 0;

	for (int i=0; i < nb_n4; i++) { // pour ne garder que les cases vides
		if (case_est_vide(g,voisins_n4[i])){
			voisins[nb_vides] = voisins_n4[i];
			nb_vides++;
		}
	}
	return nb_vides;
}

// TreeAdj(c)
int adj_trees(Grille *g, Position p, Position tree_voisins[4]){

	if (g == NULL){
		return 0;
	}
	if (!pos_valide(g, p.ligne, p.colonne)){
		return 0;
	}

	int nb_n4;
	Position voisins_n4[4];
	nb_n4= n4_voisins(g, p, voisins_n4);
	int nb_trees;
	nb_trees = 0;

	for (int i=0; i< nb_n4; i++){
		if (case_est_tree(g, voisins_n4[i])){
			tree_voisins[nb_trees] = voisins_n4[i];
			nb_trees++;
		}
	}
	return nb_trees;
}
