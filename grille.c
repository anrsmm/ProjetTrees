#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grille.h"


// création grille
Grille*creer_grille(int Hauteur, int Largeur){
	Grille *g;

	if (Hauteur <= 0 || Largeur <=0) {
		return NULL;
	}

	g = malloc(sizeof(Grille));
	if (g == NULL) {
		return NULL;
	}

	g->Hauteur = Hauteur;
	g->Largeur = Largeur;
	//initialiser tree_nbr et empty_nbr
	g->tree_nbr = 0;
	g->empty_nbr = 0;

	return g;
}

// destruction grille
void free_grille(Grille*g){

}

// position dans la grille?
int pos_valide(Grille *g, int ligne, int col){

}

//type de case 
//case contient un arbre?retourne 1 si arbre, 0 sinon
int case_est_tree(Grille *g, Position p);

//case est vide?retourne 1 si vide 
int case_est_vide(Grille *g, Position p);
