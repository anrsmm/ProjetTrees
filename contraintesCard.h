#ifndef CONTRAINTESCARD_H
#define CONTRAINTESCARD_H

#include "grille.h"
#include "cnf.h"
#include "sat.h"

//void ajout_au_plus_1(CNFformule *f, int vars[], int n);
void ajout_au_plus_k(CNFformule *f, int vars[], int n, int k);
void ajout_au_moins_k(CNFformule *f, int vars[], int n, int k);
void ajout_exactement_k(CNFformule *f, int vars[], int n, int k);

void variables_ligne(Grille *g, int ligne, int vars[]);
void variables_colonne(Grille *g, int colonne, int vars[]);

void contrainte_ligne(Grille *g, CNFformule *f, int ligne);
void contraintes_lignes(Grille *g, CNFformule *f);

void contrainte_colonne(Grille *g, CNFformule *f, int colonne);
void contraintes_colonnes(Grille *g, CNFformule *f);

#endif

