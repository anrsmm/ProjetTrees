#ifndef VOISINAGE_H
#define VOISINAGE_H

#include "grille.h"

//renvoie les voisins orthogonaux valides
int n4_voisins(Grille *g, Position p, Position voisins[4]);

//renvoie les voisins complets valides
int n8_voisins(Grille *g, Position p, Position voisins[8]);



#endif

