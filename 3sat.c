#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "3sat.h"
#include "cnf.h"


// Lit un fichier DIMACS CNF en mémoire dans CNFformule.
// Hypothèse utilisée ici : l'en-tête "p cnf ..." est lue directement.
CNFformule lire_dimacs(const char *nmfichier, int *nb_var) {
    FILE *fichier;

    CNFformule formule;
    formule = creer_cnfFormule();

    if (nb_var != NULL) {
        *nb_var = 0;
    }

    if (nmfichier == NULL || nb_var == NULL) {
        return formule;
    }

    fichier = fopen(nmfichier, "r");
    if (fichier == NULL) {
        return formule;
    }

    char p[16];
    char cnf[16];
    int nb_cl;
    if (fscanf(fichier, "%15s %15s %d %d", p, cnf, nb_var, &nb_cl) != 4) {
        fclose(fichier);
        return formule;
    }

    if (strcmp(p, "p") != 0 || strcmp(cnf, "cnf") != 0) {
        fclose(fichier);
        return formule;
    }

    Clause clause;
    clause = creer_clause();
    if (clause.litt == NULL) {
        fclose(fichier);
        return formule;
    }

    int x;
    while (fscanf(fichier, "%d", &x) == 1) {
        if (x == 0) {
            // 0 = fin de clause en DIMACS.
            ajout_clauseFormule(&formule, clause);

            clause = creer_clause();
            if (clause.litt == NULL) {
                fclose(fichier);
                return formule;
            }
        } else {
            ajout_LittClause(&clause, x);
        }
    }

    free_clause(&clause);

    fclose(fichier);
    return formule;
}
// Ecrit une formule (souvent transformée en 3-SAT) au format DIMACS.
int ecrire_dimacs3sat(const char *nom_fichier, CNFformule *f, int nb_vars) {
    FILE *out;
    if (nom_fichier == NULL || f == NULL) {
        return 0;
    }

    out = fopen(nom_fichier, "w");
    if (out == NULL) {
        return 0;
    }

    fprintf(out, "p cnf %d %d\n", nb_vars, f->num);

    for (int i = 0; i < f->num; i++) {
        for (int j = 0; j < f->clauses[i].taille; j++) {
            fprintf(out, "%d ", f->clauses[i].litt[j]);
        }
        fprintf(out, "0\n");
    }

    fclose(out);
    return 1;
}


void transform_Clause3sat(Clause *cl, CNFformule *f3, int *next_var) {
    if (cl == NULL || f3 == NULL || next_var == NULL) {
        return;
    }

    int n;
    n = cl->taille;
    Clause c;
    if (n <= 3) {
        // Déjà 3-SAT (ou moins) : on recopie telle quelle.
        c = creer_clause();
        if (c.litt == NULL) {
            return;
        }

        for (int i = 0; i < n; i++) {
            ajout_LittClause(&c, cl->litt[i]);
        }

        ajout_clauseFormule(f3, c);
        return;
    }

    // Cas n > 3 : transformation de Tseitin en chaîne.
    // On introduit des variables auxiliaires "next_var".
    int prev;
    int next;
    prev = *next_var;
    (*next_var)++;

    c = creer_clause();
    if (c.litt == NULL) {
        return;
    }

    ajout_LittClause(&c, cl->litt[0]);
    ajout_LittClause(&c, cl->litt[1]);
    ajout_LittClause(&c, prev);
    ajout_clauseFormule(f3, c);
    // Première clause : (l1 v l2 v y1)

    for (int i = 2; i < n - 2; i++) {
        next = *next_var;
        (*next_var)++;

        c = creer_clause();
        if (c.litt == NULL) {
            return;
        }

        ajout_LittClause(&c, -prev);
        ajout_LittClause(&c, cl->litt[i]);
        ajout_LittClause(&c, next);
        ajout_clauseFormule(f3, c);
        // Clauses intermédiaires : (¬y_i v l_{i+1} v y_{i+1})

        prev = next;
    }

    c = creer_clause();
    if (c.litt == NULL) {
        return;
    }

    ajout_LittClause(&c, -prev);
    ajout_LittClause(&c, cl->litt[n - 2]);
    ajout_LittClause(&c, cl->litt[n - 1]);
    ajout_clauseFormule(f3, c);
    // Dernière clause : (¬y_last v l_{n-1} v l_n)
}

// Transforme toute la formule en appliquant la transformation clause par clause.
// total_vars est mis à jour avec les variables auxiliaires créées.
CNFformule transform_Formule3sat(CNFformule *f, int *total_vars) {
    CNFformule f3;
    f3 = creer_cnfFormule();

    if (f == NULL || total_vars == NULL || f3.clauses == NULL) {
        return f3;
    }

    int next_var;
    next_var = *total_vars + 1;

    for (int i = 0; i < f->num; i++) {
        transform_Clause3sat(&f->clauses[i], &f3, &next_var);
    }

    *total_vars = next_var - 1;

    return f3;
}
