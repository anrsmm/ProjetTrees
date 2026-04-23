#include <stdlib.h>


#include "contraintesCard.h"
#include "sat.h"
#include "grille.h"
#include "cnf.h"


/*
profondeur : combien d'indices on a déjà choisis
depart : a partir de quel indice on a le droit de continuer a choisir : ca evite les doublons et garantit un ordre croissant
cas terminal : if (profondeur == taille_groupe) ca veut dire : on a choisi assez d'éléments, on peut créer une clause

*/

static void gen_au_plus_k(CNFformule *f, int vars[], int n, int taille_groupe, int choix[], int profondeur, int depart) {
    int i;
    Clause clause;

    if (profondeur == taille_groupe) {
        clause = creer_clause();
        if (clause.litt == NULL) {
            return;
        }

        for (i = 0; i < taille_groupe; i++) {
            ajout_LittClause(&clause, -vars[choix[i]]);
        }

        ajout_clauseFormule(f, clause);
        return;
    }

    for (i = depart; i < n; i++) {
        choix[profondeur] = i;
        gen_au_plus_k(f, vars, n, taille_groupe, choix, profondeur + 1, i + 1);
    }
}


void ajout_au_plus_k(CNFformule *f, int vars[], int n, int k) {
    int *choix;
    int taille_groupe;

    if (f == NULL || vars == NULL) {
        return;
    }

    if (k < 0) {
        return;
    }

    if (k >= n) {
        return;
    }

    taille_groupe = k + 1;

    choix = malloc(taille_groupe * sizeof(int));
    if (choix == NULL) {
        return;
    }

    gen_au_plus_k(f, vars, n, taille_groupe, choix, 0, 0);

    free(choix);
}


/*
AU MOINS K
*/
static void gen_au_moins_k(CNFformule *f, int vars[], int n,
                           int taille_groupe, int choix[],
                           int profondeur, int depart) {
    int i;
    Clause clause;

    if (profondeur == taille_groupe) {
        clause = creer_clause();
        if (clause.litt == NULL) {
            return;
        }

        for (i = 0; i < taille_groupe; i++) {
            ajout_LittClause(&clause, vars[choix[i]]);
        }

        ajout_clauseFormule(f, clause);
        return;
    }

    for (i = depart; i < n; i++) {
        choix[profondeur] = i;
        gen_au_moins_k(f, vars, n, taille_groupe, choix, profondeur + 1, i + 1);
    }
}

void ajout_au_moins_k(CNFformule *f, int vars[], int n, int k) {
    int *choix;
    int taille_groupe;

    if (f == NULL || vars == NULL) {
        return;
    }

    if (k <= 0) {
        return;
    }

    if (k > n) {
        Clause clause = creer_clause();
        if (clause.litt == NULL) {
            return;
        }
        ajout_clauseFormule(f, clause);
        return;
    }

    taille_groupe = n - k + 1;

    choix = malloc(taille_groupe * sizeof(int));
    if (choix == NULL) {
        return;
    }

    gen_au_moins_k(f, vars, n, taille_groupe, choix, 0, 0);

    free(choix);
}

/*
EXACTEMENT k
*/
void ajout_exactement_k(CNFformule *f, int vars[], int n, int k) {
    if (f == NULL || vars == NULL) {
        return;
    }

    ajout_au_moins_k(f, vars, n, k);
    ajout_au_plus_k(f, vars, n, k);
}

// RECUPERATION DES VARIABLES DE LA LIGNE
void variables_ligne(Grille *g, int ligne, int vars[]) {
    int j;
    Position p;

    if (g == NULL || vars == NULL) {
        return;
    }

    if (ligne < 0 || ligne >= g->Hauteur) {
        return;
    }

    for (j = 0; j < g->Largeur; j++) {
        p.ligne = ligne;
        p.colonne = j;
        vars[j] = tente_var(g, p);
    }
}

//FONCTION POUR CHAQUE LIGNE
void contrainte_ligne(Grille *g, CNFformule *f, int ligne) {
    int *vars;
    int k;

    if (g == NULL || f == NULL) {
        return;
    }

    if (ligne < 0 || ligne >= g->Hauteur) {
        return;
    }

    vars = malloc(g->Largeur * sizeof(int));
    if (vars == NULL) {
        return;
    }

    variables_ligne(g, ligne, vars);
    k = g->ligne_nbr[ligne];

    ajout_exactement_k(f, vars, g->Largeur, k);

    free(vars);
}

//appliquer contrainte a toutes les lignes
void contraintes_lignes(Grille *g, CNFformule *f) {
    int i;

    if (g == NULL || f == NULL) {
        return;
    }

    for (i = 0; i < g->Hauteur; i++) {
        contrainte_ligne(g, f, i);
    }
}

/*/////////////////////////////////////////////////////////
               CONTRAINTES SUR LES COLONNES
*/////////////////////////////////////////////////////////

void variables_colonne(Grille *g, int colonne, int vars[]) {
    int i;
    Position p;

    if (g == NULL || vars == NULL) {
        return;
    }

    if (colonne < 0 || colonne >= g->Largeur) {
        return;
    }

    for (i = 0; i < g->Hauteur; i++) {
        p.ligne = i;
        p.colonne = colonne;
        vars[i] = tente_var(g, p);
    }
}



void contrainte_colonne(Grille *g, CNFformule *f, int colonne) {
    int *vars;
    int k;

    if (g == NULL || f == NULL) {
        return;
    }

    if (colonne < 0 || colonne >= g->Largeur) {
        return;
    }

    vars = malloc(g->Hauteur * sizeof(int));
    if (vars == NULL) {
        return;
    }

    variables_colonne(g, colonne, vars);
    k = g->col_nbr[colonne];

    ajout_exactement_k(f, vars, g->Hauteur, k);

    free(vars);
}

void contraintes_colonnes(Grille *g, CNFformule *f) {
    int j;

    if (g == NULL || f == NULL) {
        return;
    }

    for (j = 0; j < g->Largeur; j++) {
        contrainte_colonne(g, f, j);
    }
}


