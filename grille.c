#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grille.h"


// création grille
Grille *creer_grille(int Hauteur, int Largeur) {
    Grille *g;
    int i, j;

    if (Hauteur <= 0 || Largeur <= 0) {
        return NULL;
    }

    g = malloc(sizeof(Grille));
    if (g == NULL) {
        return NULL;
    }

    g->Hauteur = Hauteur;
    g->Largeur = Largeur;
    g->tree_nbr = 0;
    g->empty_nbr = 0;

    g->ligne_nbr = malloc(Hauteur * sizeof(int));
    if (g->ligne_nbr == NULL) {
        free(g);
        return NULL;
    }

    g->col_nbr = malloc(Largeur * sizeof(int));
    if (g->col_nbr == NULL) {
        free(g->ligne_nbr);
        free(g);
        return NULL;
    }

    for (i = 0; i < Hauteur; i++) {
        g->ligne_nbr[i] = 0;
    }

    for (i = 0; i < Largeur; i++) {
        g->col_nbr[i] = 0;
    }

    g->trees = malloc(Hauteur * Largeur * sizeof(Position));
    if (g->trees == NULL) {
        free(g->col_nbr);
        free(g->ligne_nbr);
        free(g);
        return NULL;
    }

    g->empty_cases = malloc(Hauteur * Largeur * sizeof(Position));
    if (g->empty_cases == NULL) {
        free(g->trees);
        free(g->col_nbr);
        free(g->ligne_nbr);
        free(g);
        return NULL;
    }

    g->est_tree = malloc(Hauteur * sizeof(int *));
    if (g->est_tree == NULL) {
        free(g->empty_cases);
        free(g->trees);
        free(g->col_nbr);
        free(g->ligne_nbr);
        free(g);
        return NULL;
    }

    for (i = 0; i < Hauteur; i++) {
        g->est_tree[i] = malloc(Largeur * sizeof(int));
        if (g->est_tree[i] == NULL) {
            for (j = 0; j < i; j++) {
                free(g->est_tree[j]);
            }
            free(g->est_tree);
            free(g->empty_cases);
            free(g->trees);
            free(g->col_nbr);
            free(g->ligne_nbr);
            free(g);
            return NULL;
        }
    }

    for (i = 0; i < Hauteur; i++) {
        for (j = 0; j < Largeur; j++) {
            g->est_tree[i][j] = 0;
        }
    }

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
