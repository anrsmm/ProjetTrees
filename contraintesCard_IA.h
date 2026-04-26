#ifndef CONTRAINTESCARD_IA_H
#define CONTRAINTESCARD_IA_H

#include "grille.h"
#include "cnf.h"
#include "sat.h"

/* Contraintes de cardinalite */
void ajout_au_plus_k(CNFformule *f, int vars[], int n, int k);
void ajout_au_moins_k(CNFformule *f, int vars[], int n, int k);
void ajout_exactement_k(CNFformule *f, int vars[], int n, int k);

/* Lignes */
void variables_ligne(Grille *g, int ligne, int vars[]);
void contrainte_ligne(Grille *g, CNFformule *f, int ligne);
void contraintes_lignes(Grille *g, CNFformule *f);

/* Colonnes */
void variables_colonne(Grille *g, int colonne, int vars[]);
void contrainte_colonne(Grille *g, CNFformule *f, int colonne);
void contraintes_colonnes(Grille *g, CNFformule *f);

/* Nouvelle fonction IA :
   renvoie la plus grande variable utilisee,
   y compris les variables auxiliaires */
int nombre_variables_cardinalite(void);

#endif