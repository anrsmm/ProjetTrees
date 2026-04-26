#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "3sat.h"
#include "cnf.h"


/*
 Lit un fichier DIMACS CNF en mémoire dans CNFformule.
Hypothèse utilisée ici : l'en-tête "p cnf ..." est lue directement.
 */
CNFformule lire_dimacs(const char *nmfichier, int *nb_var) {
    FILE *fichier;

    CNFformule formule;
    formule = creer_cnfFormule();//creer une formule CNF vide

    if (nb_var != NULL) {
        *nb_var = 0;
    }

    if (nmfichier == NULL || nb_var == NULL) {//si le fichier ou le nb de variables nul-> formule vide
        return formule;
    }

    fichier = fopen(nmfichier, "r");//ouverture du fichier en lecture
    if (fichier == NULL) {//si le fichier n'est pas ouvert-> formule vide
        return formule;
    }

    char p[16];//en-tête "p cnf ..."
    char cnf[16];//en-tête "cnf ..."
    int nb_cl;//nb de clauses
    if (fscanf(fichier, "%15s %15s %d %d", p, cnf, nb_var, &nb_cl) != 4) {//si le format n'est pas correct-> formule vide
        fclose(fichier);
        return formule;
    }

    if (strcmp(p, "p") != 0 || strcmp(cnf, "cnf") != 0) {//si le format n'est pas correct-> formule vide
        fclose(fichier);
        return formule;
    }

    Clause clause;//clause courante
    clause = creer_clause();
    if (clause.litt == NULL) {
        fclose(fichier);
        return formule;
    }

    int x;//litt
    while (fscanf(fichier, "%d", &x) == 1) {//lecture litt
        if (x == 0) {
            // 0 = fin de clause en DIMACS.
            ajout_clauseFormule(&formule, clause);//ajout de la clause a la formule

            clause = creer_clause();
            if (clause.litt == NULL) {//si la clause n'est pas creer-> formule vide
                fclose(fichier);
                return formule;
            }
        } else {
            ajout_LittClause(&clause, x);//ajout du litt a la clause
        }
    }

    free_clause(&clause);

    fclose(fichier);
    return formule;
}
/*
fonction ecrire_dimacs3sat:
ecrit la formule 3-SAT au format DIMACS.
 */
int ecrire_dimacs3sat(const char *nom_fichier, CNFformule *f, int nb_vars) {
    FILE *out;//fichier de sortie
    if (nom_fichier == NULL || f == NULL) {//si le fichier ou la formule nul-> formule vide
        return 0;
    }

    out = fopen(nom_fichier, "w");//ouvvre fichier en ecriture
    if (out == NULL) {
        return 0;
    }

    fprintf(out, "p cnf %d %d\n", nb_vars, f->num);//ecriture de l'en-tête "p cnf ..."

    for (int i = 0; i < f->num; i++) {//pour chaque clause
        for (int j = 0; j < f->clauses[i].taille; j++) {//pour chaque litt
            fprintf(out, "%d ", f->clauses[i].litt[j]);//ecriture du litt
        }
        fprintf(out, "0\n");//ecriture de la fin de clause
    }

    fclose(out);
    return 1;
}

/*
fonction transform_Clause3sat:
convertit 1 clause CNF en clauses de taille <= 3.
 */
void transform_Clause3sat(Clause *cl, CNFformule *f3, int *next_var) {
    if (cl == NULL || f3 == NULL || next_var == NULL) {//si la clause ou la formule ou le nb de variables nul-> formule vide
        return;
    }

    int n;//taille de la clause
    n = cl->taille;
    Clause c;
    if (n <= 3) {
        // Déjà 3-SAT (ou moins) ;recopie direct
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
    int prev;//litt precedente
    int next;//litt suivante
    prev = *next_var;//initialis 
    (*next_var)++;//pour nb de variables

    c = creer_clause();
    if (c.litt == NULL) {
        return;
    }

    ajout_LittClause(&c, cl->litt[0]);//ajout du litt a la clause
    ajout_LittClause(&c, cl->litt[1]);//ajout du litt a la clause
    ajout_LittClause(&c, prev);//ajout du litt a la clause
    ajout_clauseFormule(f3, c);//ajout de la clause a la formule
    // 1ere clause: (l1 v l2 v y1)

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
        // clause inter: (-yi v li v y(i+1))

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
    // derniere clause: (-ylast v l(n-1) v ln)
}

/*
fonction transform_Formule3sat:
applique transform_Clause3sat a toute la formule.
 */
CNFformule transform_Formule3sat(CNFformule *f, int *total_vars) {
    CNFformule f3;//formule 3-SAT
    f3 = creer_cnfFormule();//creer une formule 3-SAT vide

    if (f == NULL || total_vars == NULL || f3.clauses == NULL) {
        return f3;
    }

    int next_var;//nb de variables
    next_var = *total_vars + 1;//initialis 

    for (int i = 0; i < f->num; i++) {//pour chaque clause
        transform_Clause3sat(&f->clauses[i], &f3, &next_var);//applique transform_Clause3sat a la clause
    }

    *total_vars = next_var - 1;//pour nb de variables

    return f3;
}
