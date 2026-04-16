#include <stdlib.h>

#include "cnf.h"

Clause creer_clause(){
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

void free_clause(Clause *clause) {
    if (clause == NULL) {
        return;
    }

    free(clause->litt);
	//eviter erreur on remet a zero tout
    clause->litt = NULL;
    clause->taille = 0;
    clause->max = 0;
}
//ajout a la fin
void ajout_LittClause(Clause *clause, int litt){
	if (clause ==NULL){
		return;
	}
	if (clause->litt ==NULL){
		return;
	}
	int *temp;
	if (clause->taille >= clause->max) {
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


CNFformule creer_cnfFormule() {
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



void ajout_clauseFormule(CNFformule *formule, Clause clause){
    Clause *temp;

    if (formule == NULL) {
        return;
    }

    if (formule->clauses == NULL) {
        return;
    }

    if (formule->num >= formule->max) {
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

void free_cnfFormule(CNFformule *formule) {
    int i;

    if (formule == NULL) {
        return;
    }

    if (formule->clauses != NULL) {
        for (i = 0; i < formule->num; i++) {
            free_clause(&formule->clauses[i]);
        }

        free(formule->clauses);
    }

    formule->clauses = NULL;
    formule->num = 0;
    formule->max = 0;
}
