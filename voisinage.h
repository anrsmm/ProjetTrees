#ifndef VOISINAGE_H
#define VOISINAGE_H

#include "grille.h"

//renvoie les voisins orthogonaux valides
int n4_voisins(Grille *g, Position p, Position voisins[4]);

//renvoie les voisins complets valides
int n8_voisins(Grille *g, Position p, Position voisins[8]);

// Adj(t)= N4(t) et Empty, renvoi les cases vides voisines trouvées
int est_caseVide_Adj(Grille *g, Position tree, Position voisins[4]);

//TreeAdj(c) = N4 et Trees, renvoi les cases arbres voisines trouvées
int adj_trees(Grille *g, Position p, Position tree_voisins[4]);

#endif

