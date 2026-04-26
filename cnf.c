#include <stdlib.h>

#include "cnf.h"

/*
 * creer_clause:
 * cree une clause vide (tab dynamique).
 */
Clause creer_clause(){
	// Etape principale de la fonction.
	Clause c;

	c.taille=0;
	c.max=4;
	c.litt=malloc(c.max *sizeof(int));

	if (c.litt==NULL){
		c.taille =0;
		c.max=0;

	}
	return c;
}

/*
 * free_clause:
 * libere le tableau de litteraux d'une clause.
 */
void free_clause(Clause *clause) {
    if (clause == NULL) {
        return;
    }

    free(clause->litt);
	// reset champs apres free
    clause->litt = NULL;
    clause->taille = 0;
    clause->max = 0;
}
/*
 * ajout_LittClause:
 * ajoute 1 litt en fin de clause.
 */
void ajout_LittClause(Clause *clause, int litt){
	if (clause ==NULL){
		return;
	}
	if (clause->litt ==NULL){
		return;
	}
	int *temp;
	if (clause->taille >= clause->max) {
		// double la capacite si plein
		temp =realloc(clause->litt, 2 * clause->max * sizeof(int));
		if (temp ==NULL) {
			return;
		}
		clause->litt=temp;
		clause->max = 2* clause->max;
	}
	clause->litt[clause->taille] = litt;
	clause->taille++;
}


/*
 * creer_cnfFormule:
 * cree une formule CNF vide.
 */
CNFformule creer_cnfFormule() {
    // Etape principale de la fonction.
    CNFformule f;

    f.num = 0;
    f.max = 4;
    f.clauses = malloc(f.max * sizeof(Clause));

    if (f.clauses == NULL) {
        f.num = 0;
        f.max = 0;
    }

    return f;
}
/*
 * ajout_clauseFormule:
 * ajoute 1 clause a la formule.
 */
void ajout_clauseFormule(CNFformule *formule, Clause clause){
    if (formule == NULL) {
        return;
    }

    if (formule->clauses == NULL) {
        return;
    }


    Clause *temp;
    if (formule->num >= formule->max) {
        // meme logique d'agrandissement
        temp = realloc(formule->clauses, 2 * formule->max * sizeof(Clause));
        if (temp == NULL) {
            return;
        }

        formule->clauses = temp;
        formule->max = 2 * formule->max;
    }

    formule->clauses[formule->num] = clause;
    formule->num++;
}

/*
 * free_cnfFormule:
 * libere toutes les clauses puis la formule.
 */
void free_cnfFormule(CNFformule *formule) {
    if (formule == NULL) {
        return;
    }

    if (formule->clauses != NULL) {
        // liberation profonde clause par clause
        for (int i = 0; i < formule->num; i++) {
            free_clause(&formule->clauses[i]);
        }

        free(formule->clauses);
    }

    formule->clauses = NULL;
    formule->num = 0;
    formule->max = 0;
}
