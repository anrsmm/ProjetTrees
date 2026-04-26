/*#########################################################
 Version modifiée du projet développé avec l'aide de l'IA
#########################################################*/


/*=============================================================================
Changement principal :
L’ancien encodage base sur les combinaisons a ete remplace
par un encodage utilisant un compteur sequentiel.

Cela permet d’eviter la generation d’un trop grand nombre de clauses 
pour les grandes grilles, comme les grilles 30×30.
==============================================================================*/




#include <stdlib.h>
#include "contraintesCard_IA.h"
#include "sat.h"
#include "grille.h"
#include "cnf.h"



/*   Compteur des variables auxiliaires.
     Ces variables ne correspondent pas a des cases de la grille.
     Elles servent seulement au compteur sequentiel    */

static int prochaine_var_aux = 0;





/*   Renvoie la valeur absolue d’un entier 

     Un litteral SAT peut etre positif ou negatif,
     mais le numwro de variable reste positif  */

static int valeur_absolue(int x)
{
    if (x < 0) return -x;
    return x;
}






/*  Cherche la plus grande variable deja utilise
    dans la formule CNF.
    Cela evite de reutiliser un numero deja pris */

static int max_variable_formule(CNFformule *f)
{
    int i, j;
    int max = 0;
    int v;

    if (f == NULL) return 0;

    for (i = 0; i < f->num; i++) {
        for (j = 0; j < f->clauses[i].taille; j++) {
            v = valeur_absolue(f->clauses[i].litt[j]);

            if (v > max) {
                max = v;
            }
        }
    }

    return max;
}





/*  Initialise le compteur des variables auxiliaires 
    On commence apres la plus grande variable deja utilise */

static void initialiser_variables_aux(CNFformule *f, int vars[], int n)
{
    int i;
    int max = max_variable_formule(f);

    for (i = 0; i < n; i++) {
        if (valeur_absolue(vars[i]) > max) {
            max = valeur_absolue(vars[i]);
        }
    }

    if (prochaine_var_aux < max) {
        prochaine_var_aux = max;
    }
}






/*  Cre une nouvelle variable auxiliaire  */

static int nouvelle_variable(void)
{
    prochaine_var_aux++;
    return prochaine_var_aux;
}






/*  Renvoie le plus grand numero de variable utilise.
    Cette fonction est utile dans le programme principal
    pour allouer assez de memoire   */

int nombre_variables_cardinalite(void)
{
    return prochaine_var_aux;
}





/*  Ajoute une clause avec un seul litteral  */

static void ajouter_clause_1(CNFformule *f, int a)
{
    Clause c = creer_clause();
    ajout_LittClause(&c, a);
    ajout_clauseFormule(f, c);
}





/*  Ajoute une clause avec deux litteraux */

static void ajouter_clause_2(CNFformule *f, int a, int b)
{
    Clause c = creer_clause();
    ajout_LittClause(&c, a);
    ajout_LittClause(&c, b);
    ajout_clauseFormule(f, c);
}






/*  Ajoute une clause avec trois litteraux.  */

static void ajouter_clause_3(CNFformule *f, int a, int b, int d)
{
    Clause c = creer_clause();
    ajout_LittClause(&c, a);
    ajout_LittClause(&c, b);
    ajout_LittClause(&c, d);
    ajout_clauseFormule(f, c);
}





/*Encodage de : au plus k variables vraies.

Old methode : on generait toutes les combinaisons possibles.

New methode : on utilise un compteur sequentiel 
              avec des variables auxiliaires.

s[i][j] signifie : jusqu’a la position i, 
                   on a deja atteint au moins j+1 
                   variables vraies    */

void ajout_au_plus_k(CNFformule *f, int vars[], int n, int k)
{
    int i, j;
    int **s;

    if (f == NULL || vars == NULL) return;

    initialiser_variables_aux(f, vars, n);

    if (k < 0) {
        Clause c = creer_clause();
        ajout_clauseFormule(f, c);
        return;
    }

    if (k >= n) return;

    /* Si k = 0, toutes les variables doivent etre fausses */
    if (k == 0) {
        for (i = 0; i < n; i++) {
            ajouter_clause_1(f, -vars[i]);
        }
        return;
    }

    /* Tableau des variables auxiliaires */
    s = malloc(n * sizeof(int *));
    if (s == NULL) return;

    for (i = 0; i < n; i++) {
        s[i] = malloc(k * sizeof(int));
        if (s[i] == NULL) return;

        for (j = 0; j < k; j++) {
            s[i][j] = nouvelle_variable();
        }
    }

    /* 1iere variable */
    ajouter_clause_2(f, -vars[0], s[0][0]);

    for (j = 1; j < k; j++) {
        ajouter_clause_1(f, -s[0][j]);
    }


    for (i = 1; i < n; i++) {

        ajouter_clause_2(f, -vars[i], s[i][0]);

        for (j = 0; j < k; j++) {
            ajouter_clause_2(f, -s[i - 1][j], s[i][j]);
        }

        for (j = 1; j < k; j++) {
            ajouter_clause_3(f, -vars[i], -s[i - 1][j - 1], s[i][j]);
        }

        /* Empeche de depasser k */
        ajouter_clause_2(f, -vars[i], -s[i - 1][k - 1]);
    }

    for (i = 0; i < n; i++) {
        free(s[i]);
    }

    free(s);
}





/*   Encodage de : au moins k variables vraies.
     On utilise la relation : 
     au moins k vraies = au plus n-k fausses
     
     On inverse donc les variables puis on reutilise
     ajout_au_plus_k        */

void ajout_au_moins_k(CNFformule *f, int vars[], int n, int k)
{
    int i;
    int *vars_neg;

    if (f == NULL || vars == NULL) return;

    if (k <= 0) return;

    if (k > n) {
        Clause c = creer_clause();
        ajout_clauseFormule(f, c);
        return;
    }

    vars_neg = malloc(n * sizeof(int));
    if (vars_neg == NULL) return;

    for (i = 0; i < n; i++) {
        vars_neg[i] = -vars[i];
    }

    ajout_au_plus_k(f, vars_neg, n, n - k);

    free(vars_neg);
}

/*   Exactement k =  au moins k
            AND
    au plus k     */
void ajout_exactement_k(CNFformule *f, int vars[], int n, int k)
{
    if (f == NULL || vars == NULL) return;

    ajout_au_moins_k(f, vars, n, k);
    ajout_au_plus_k(f, vars, n, k);
}





/*    Les fonctions suivantes gardent la meme ide de
      recupere les variables d’une ligne ou colonne
      puis appliquer exactement k   */

void variables_ligne(Grille *g, int ligne, int vars[])
{
    int j;
    Position p;

    if (g == NULL || vars == NULL) return;
    if (ligne < 0 || ligne >= g->Hauteur) return;

    for (j = 0; j < g->Largeur; j++) {
        p.ligne = ligne;
        p.colonne = j;
        vars[j] = tente_var(g, p);
    }
}

void contrainte_ligne(Grille *g, CNFformule *f, int ligne)
{
    int *vars;
    int k;

    if (g == NULL || f == NULL) return;
    if (ligne < 0 || ligne >= g->Hauteur) return;

    vars = malloc(g->Largeur * sizeof(int));
    if (vars == NULL) return;

    variables_ligne(g, ligne, vars);

    /* Nombre demandé sur cette ligne */
    k = g->ligne_nbr[ligne];

    ajout_exactement_k(f, vars, g->Largeur, k);

    free(vars);
}

void contraintes_lignes(Grille *g, CNFformule *f)
{
    int i;

    if (g == NULL || f == NULL) return;

    for (i = 0; i < g->Hauteur; i++) {
        contrainte_ligne(g, f, i);
    }
}

void variables_colonne(Grille *g, int colonne, int vars[])
{
    int i;
    Position p;

    if (g == NULL || vars == NULL) return;
    if (colonne < 0 || colonne >= g->Largeur) return;

    for (i = 0; i < g->Hauteur; i++) {
        p.ligne = i;
        p.colonne = colonne;
        vars[i] = tente_var(g, p);
    }
}

void contrainte_colonne(Grille *g, CNFformule *f, int colonne)
{
    int *vars;
    int k;

    if (g == NULL || f == NULL) return;
    if (colonne < 0 || colonne >= g->Largeur) return;

    vars = malloc(g->Hauteur * sizeof(int));
    if (vars == NULL) return;

    variables_colonne(g, colonne, vars);

    /* Nombre demandé sur cette colonne */
    k = g->col_nbr[colonne];

    ajout_exactement_k(f, vars, g->Hauteur, k);

    free(vars);
}

void contraintes_colonnes(Grille *g, CNFformule *f)
{
    int j;

    if (g == NULL || f == NULL) return;

    for (j = 0; j < g->Largeur; j++) {
        contrainte_colonne(g, f, j);
    }
}





