#ifndef CNF_H
#define CNF_H

#include "grille.h"
#include "voisinage.h"

typedef struct {
    int *litt;
    int taille;
    int max;
} Clause;

typedef struct {
    Clause *clauses;
    int num;
    int max;
} CNFformule;

//fonctions pour manipuler les clauses
Clause creer_clause();
void ajout_LittClause(Clause *clause, int litt);
void free_clause(Clause *clause);

//fonctions pour manipuler les CNF
CNFformule creer_cnfFormule();
void ajout_clauseFormule(CNFformule *formule, CLause clause);
void free_cnfFormule(CNFformule *formule);
