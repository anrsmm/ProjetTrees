#include <stdlib.h>


#include "contraintesCard.h"
#include "sat.h"
#include "grille.h"
#include "cnf.h"


/*
Encodage cardinalité "AU PLUS k" (at-most-k)
------------------------------------------------
But : sur un ensemble de variables booléennes vars[0..n-1], imposer qu'au plus k
variables soient vraies.

Idée SAT classique utilisée ici :
- Si on veut "au plus k", alors toute combinaison de (k+1) variables
  ne peut pas être simultanément vraie.
- Pour chaque sous-ensemble S de taille (k+1), on ajoute :
    OR_{x in S} (¬x)
  Ce qui interdit "toutes vraies en même temps" pour ce sous-ensemble.

Rôle des paramètres récursifs :
- profondeur : combien d'indices sont déjà choisis dans 'choix'
- depart     : prochain indice minimal autorisé (évite les doublons de combinaisons)
- taille_groupe = k+1
*/

static void gen_au_plus_k(CNFformule *f, int vars[], int n, int taille_groupe, int choix[], int profondeur, int depart) {
    int i;
    Clause clause;

    if (profondeur == taille_groupe) {
        /*
        Cas terminal :
        On a construit un sous-ensemble S de taille (k+1) via choix[].
        On crée la clause :
            (¬vars[choix[0]] v ¬vars[choix[1]] v ... )
        */
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
        // On fixe le prochain élément de la combinaison.
        choix[profondeur] = i;
        // Appel récursif avec i+1 : garantit des combinaisons strictement croissantes.
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
        // Cas incohérent : "au plus -1" est impossible.
        return;
    }

    if (k >= n) {
        // Toujours vrai : on ne peut jamais avoir plus de n vraies sur n variables.
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
Encodage cardinalité "AU MOINS k" (at-least-k)
------------------------------------------------
But : imposer qu'au moins k variables soient vraies.

Transformation utilisée ici :
"Au moins k vraies" équivaut à dire :
- pour tout sous-ensemble T de taille (n-k+1), au moins une variable de T est vraie.

Donc pour chaque sous-ensemble T de taille (n-k+1), on ajoute la clause :
    OR_{x in T} x

Intuition :
si on avait moins de k vraies, alors il y aurait au moins (n-k+1) variables fausses ;
en prenant exactement ces variables, on violerait une clause de ce type.
*/
static void gen_au_moins_k(CNFformule *f, int vars[], int n,
                           int taille_groupe, int choix[],
                           int profondeur, int depart) {
    int i;
    Clause clause;

    if (profondeur == taille_groupe) {
        // Cas terminal : on a un sous-ensemble T complet.
        // On ajoute la clause positive (x1 v x2 v ...).
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
        // Construction combinatoire identique à "au plus k".
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
        // Toujours vrai : "au moins 0" ne contraint rien.
        return;
    }

    if (k > n) {
        /*
        Impossible d'avoir au moins k vraies si k > n.
        On encode l'insatisfiabilité avec une clause vide.
        En CNF, une clause vide est toujours fausse => formule UNSAT.
        */
        Clause clause = creer_clause();
        if (clause.litt == NULL) {
            return;
        }
        ajout_clauseFormule(f, clause);
        return;
    }

    // Taille des sous-ensembles T à générer pour l'encodage au-moins-k.
    taille_groupe = n - k + 1;

    choix = malloc(taille_groupe * sizeof(int));
    if (choix == NULL) {
        return;
    }

    gen_au_moins_k(f, vars, n, taille_groupe, choix, 0, 0);

    free(choix);
}

/*
Encodage "EXACTEMENT k"
-----------------------
exactement_k  <=>  (au_moins_k) ET (au_plus_k)
On combine donc simplement les deux encodages précédents.
*/
void ajout_exactement_k(CNFformule *f, int vars[], int n, int k) {
    if (f == NULL || vars == NULL) {
        return;
    }

    ajout_au_moins_k(f, vars, n, k);
    ajout_au_plus_k(f, vars, n, k);
}

// Récupère les identifiants DIMACS T_(ligne,j) pour toutes les colonnes j.
// vars[j] devient la variable "tente sur la case (ligne,j)".
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

// Contrainte d'une ligne :
// le nombre de tentes de la ligne doit être exactement g->ligne_nbr[ligne].
// On applique donc "exactement_k" sur les variables de la ligne.
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

    // Formule logique : somme_j T_(ligne,j) = k
    ajout_exactement_k(f, vars, g->Largeur, k);

    free(vars);
}

// Applique la contrainte de comptage à chaque ligne de la grille.
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

// Récupère les identifiants DIMACS T_(i,colonne) pour toutes les lignes i.
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

    // Formule logique : somme_i T_(i,colonne) = k
    ajout_exactement_k(f, vars, g->Hauteur, k);

    free(vars);
}

void contraintes_colonnes(Grille *g, CNFformule *f) {
    int j;

    if (g == NULL || f == NULL) {
        return;
    }

    // Applique la contrainte de comptage à chaque colonne.
    for (j = 0; j < g->Largeur; j++) {
        contrainte_colonne(g, f, j);
    }
}


