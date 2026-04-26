#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "voisinage.h"

/*
fonction n4_voisins:
rempli les voisins orthogonaux valides
 */
int n4_voisins(Grille *g, Position p, Position voisins[4]) {
    int nb = 0;
    Position q;

    if (g == NULL) {
        return 0; //retourne 0 si la grille n'est pas valide
    }

    if (!pos_valide(g, p.ligne, p.colonne)) {
        return 0; //retourne 0 si la position n'est pas valide
    }

    // haut
    q.ligne = p.ligne - 1; //position haut
    q.colonne = p.colonne; //position colonne
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q; //ajoute le voisin a la liste
        nb++; //pour avoir le nb de voisins
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

/*
fonction n8_voisins:
remplit les voisins orthogonaux + diagonaux.
 */
int n8_voisins(Grille *g, Position p, Position voisins[8]) {
    int nb;
    Position q;

    if (g == NULL) {
        return 0; //retourne 0 si la grille n'est pas valide
    }

    if (!pos_valide(g, p.ligne, p.colonne)) {
        return 0; //retourne 0 si la position n'est pas valide
    }

    // base: voisins n4
    nb = n4_voisins(g, p, voisins);//appel fct n4_voisins pour recuperer voisins orthog

    // haut-gauche
    q.ligne = p.ligne - 1; //position haut-gauche
    q.colonne = p.colonne - 1; //position colonne
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q; 
        nb++;   
    }

    // haut-droite
    q.ligne = p.ligne - 1;//position haut-droite
    q.colonne = p.colonne + 1;//position colonne
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

/*
fonction est_caseVide_Adj:
renvoie les cases vides voisines d'un arbre.
 */
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
	int nb_n4;//nb de voisins orthog
	Position voisins_n4[4];//liste de voisins orthog
	nb_n4= n4_voisins(g, tree, voisins_n4);//appel fct n4_voisins pour recuperer voisins orthog

	int nb_vides = 0;//nb de voisins vides

	for (int i=0; i < nb_n4; i++) { // garde uniquement les cases vides
		if (case_est_vide(g,voisins_n4[i])){
			voisins[nb_vides] = voisins_n4[i];
			nb_vides++;
		}
	}
	return nb_vides;
}

/*
adj_trees:
renvoie les arbres voisins d'une case.
 */
int adj_trees(Grille *g, Position p, Position tree_voisins[4]){

	if (g == NULL){
		return 0;
	}
	if (!pos_valide(g, p.ligne, p.colonne)){
		return 0;
	}

	int nb_n4;//nb de voisins orthog
	Position voisins_n4[4];//liste de voisins orthog
	nb_n4= n4_voisins(g, p, voisins_n4);
	int nb_trees;//nb de voisins arbres
	nb_trees = 0;

	for (int i=0; i< nb_n4; i++){//pour chaque voisin orthog
		if (case_est_tree(g, voisins_n4[i])){
			tree_voisins[nb_trees] = voisins_n4[i];
			nb_trees++;
		}
	}
	return nb_trees;
}
