#include <stdlib.h>


#include "contraintesCard.h"
#include "sat.h"
#include "grille.h"
#include "cnf.h"


/*
CONTRAINTE CARD "AU PLUS k"
sur vars[0..n-1], on veut <- k vars vraies

idee:
- on prend tous les groupes de taille (k+1)
- pour chaque groupe, on ajoute (nonx1 v nonx2 v ... )
-> impossible d'avoir (k+1) vraies en mme temps
*/

static void gen_au_plus_k(CNFformule *f, int vars[], int n, int taille_groupe, int choix[], int profondeur, int depart) {
    int i;
    Clause clause;

    if (profondeur == taille_groupe) {
        /* cas terminal: un groupe complet de taille (k+1) */
        clause = creer_clause();
        if (clause.litt == NULL) {
            return;
        }

        for (i = 0; i < taille_groupe; i++) {
            // clause d'interdiction: pas toutes vraies ensemble
            ajout_LittClause(&clause, -vars[choix[i]]);
        }

        // 1 clause par combinaison de taille (k+1)
        ajout_clauseFormule(f, clause);
        return;
    }

    for (i = depart; i < n; i++) {
        // on fixe l'indice suivant dans la combi
        choix[profondeur] = i;
        // rec avec i+1 pour eviter doublons
        gen_au_plus_k(f, vars, n, taille_groupe, choix, profondeur + 1, i + 1);
    }
}


/*
ajout_au_plus_k:
ajoute l'encodage SAT de "au plus k" sur vars.
 */
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
        // tjs vrai: max n vraies sur n vars
        return;
    }

    // taille des groupes a interdire
    taille_groupe = k + 1;

    choix = malloc(taille_groupe * sizeof(int));
    if (choix == NULL) {
        return;
    }

    // depart rec: aucun choix, indice de depart = 0
    gen_au_plus_k(f, vars, n, taille_groupe, choix, 0, 0);

    free(choix);
}


/*
CONTRAINTE CARD "AU MOINS k"
sur vars[0..n-1], on veut >= k vars vraies
idee: pour chaque groupe de taille (n-k+1), on ajoute (x1 v x2 v ...).
*/
static void gen_au_moins_k(CNFformule *f, int vars[], int n, int taille_groupe, int choix[], int profondeur, int depart) {
    Clause clause;

    if (profondeur == taille_groupe) {
        // cas terminal: groupe complet
        clause = creer_clause();
        if (clause.litt == NULL) {
            return;
        }

        for (int i = 0; i < taille_groupe; i++) {
            // clause positive: au moins une var du groupe vraie
            ajout_LittClause(&clause, vars[choix[i]]);
        }

        // 1 clause par groupe de taille (n-k+1)
        ajout_clauseFormule(f, clause);
        return;
    }

    for (int i = depart; i < n; i++) {
        // mme construction que au_plus_k
        choix[profondeur] = i;
        gen_au_moins_k(f, vars, n, taille_groupe, choix, profondeur + 1, i + 1);
    }
}

/*
ajout_au_moins_k: ajoute l'encodage SAT de "au moins k" sur vars
 */
void ajout_au_moins_k(CNFformule *f, int vars[], int n, int k) {
    int *choix;
    int taille_groupe;

    if (f == NULL || vars == NULL) {
        return;
    }

    if (k <= 0) {
        // tjs vrai
        return;
    }

    if (k > n) {
        /*
        cas impossible (k>n):
        on force UNSAT avec une clause vide.
        */
        Clause clause = creer_clause();
        if (clause.litt == NULL) {
            return;
        }
        ajout_clauseFormule(f, clause);
        return;
    }

    // taille des groupes a generer
    taille_groupe = n - k + 1;

    choix = malloc(taille_groupe * sizeof(int));
    if (choix == NULL) {
        return;
    }

    // generation recc de toutes les combinaisons utiles
    gen_au_moins_k(f, vars, n, taille_groupe, choix, 0, 0);

    free(choix);
}

/*
CONTRAINTE CARD "EXACTEMENT k"
exactement_k correspond a(au_moins_k) ET (au_plus_k)
*/
void ajout_exactement_k(CNFformule *f, int vars[], int n, int k) {
    if (f == NULL || vars == NULL) {
        return;
    }

    // exact k = au moins k + au plus k
    ajout_au_moins_k(f, vars, n, k);
    ajout_au_plus_k(f, vars, n, k);
}

/*
variables_ligne:
remplit vars[j] avec id DIMACS de T_(ligne,j)
 */
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
        // conversion position -> var SAT tente
        vars[j] = tente_var(g, p);
    }
}

/*
contrainte_ligne:
impose nb tentes ligne = g->ligne_nbr[lign]
 */
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

    // vars = toutes les vars tente de la ligne
    variables_ligne(g, ligne, vars);
    k = g->ligne_nbr[ligne];

    // somme_j T_(ligne,j) = k
    ajout_exactement_k(f, vars, g->Largeur, k);

    free(vars);
}

/*
contraintes_lignes: applique contrainte_ligne sur chaque ligne
 */
void contraintes_lignes(Grille *g, CNFformule *f) {
    int i;

    if (g == NULL || f == NULL) {
        return;
    }

    for (i = 0; i < g->Hauteur; i++) {
        // 1 contrainte par ligne
        contrainte_ligne(g, f, i);
    }
}

/* ======================================================
   CONTRAINTES SUR LES COLONNES
   ====================================================== */

/*
variables_colonne:
remplit vars[i] avec id DIMACS de T_(i,colonne)
 */
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
        // conversion position -> var SAT tente
        vars[i] = tente_var(g, p);
    }
}



/*
contrainte_colonne: impose nb tentes colonne = g->col_nbr[colonne]
 */
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

    // vars = toutes les vars tente de la colonne
    variables_colonne(g, colonne, vars);
    k = g->col_nbr[colonne];

    // somme_i T_(i,colonne) = k
    ajout_exactement_k(f, vars, g->Hauteur, k);

    free(vars);
}

/*
contraintes_colonnes: applique contrainte_colonne sur chaque colonne
 */
void contraintes_colonnes(Grille *g, CNFformule *f) {
    int j;

    if (g == NULL || f == NULL) {
        return;
    }

    // 1 contrainte par colonne
    for (j = 0; j < g->Largeur; j++) {
        contrainte_colonne(g, f, j);
    }
}


