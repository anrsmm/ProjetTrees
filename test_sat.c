#include <stdio.h>
#include <stdlib.h>

#include "grille.h"
#include "sat.h"

int main(int argc, char *argv[]) {
    Grille *g;
    Position p;

    int id;
    int nb_assoc;

    SATmap *m;
    int i;

    Position arbre_test;
    Position case_test;
    int id_assoc;

    if (argc < 4) {
        printf("Usage: %s <fichier> <ligne> <colonne>\n", argv[0]);
        return 1;
    }

    // =========================
    // Lecture de la grille
    // =========================
    g = lire_grille_fichier(argv[1]);
    if (g == NULL) {
        printf("Erreur lecture fichier\n");
        return 1;
    }

    p.ligne = atoi(argv[2]);
    p.colonne = atoi(argv[3]);

    afficher_grille(g);

    // =========================
    // TEST tente_var
    // =========================
    printf("\nTest variable tente pour (%d,%d)\n", p.ligne, p.colonne);

    id = tente_var(g, p);
    printf("ID DIMACS = %d\n", id);

    // =========================
    // TEST compter_varsAssoc
    // =========================
    nb_assoc = compter_varsAssoc(g);
    printf("\nNombre total de variables d'association = %d\n", nb_assoc);

    // =========================
    // TEST creer_mapSAT
    // =========================
    m = creer_mapSAT(g);
    if (m == NULL) {
        printf("Erreur creation mapping SAT\n");
        free_grille(g);
        return 1;
    }

    printf("\nMapping SAT:\n");
    printf("assoc_var_num = %d\n", m->assoc_var_num);
    printf("total_var_num = %d\n", m->total_var_num);

    printf("\nListe des variables d'association:\n");

    for (i = 0; i < m->assoc_var_num; i++) {
        printf("A_%d : arbre (%d,%d) <-> case (%d,%d), id = %d\n",
               i,
               m->assoc_vars[i].tree.ligne,
               m->assoc_vars[i].tree.colonne,
               m->assoc_vars[i].case_vide.ligne,
               m->assoc_vars[i].case_vide.colonne,
               m->assoc_vars[i].dimacs_id);
    }

    // =========================
    // TEST assoc_var (cas existant)
    // =========================
    arbre_test.ligne = 0;
    arbre_test.colonne = 4;

    case_test.ligne = 0;
    case_test.colonne = 5;

    id_assoc = assoc_var(m, arbre_test, case_test);

    printf("\nTest assoc_var pour arbre (%d,%d) et case (%d,%d)\n",
           arbre_test.ligne, arbre_test.colonne,
           case_test.ligne, case_test.colonne);

    printf("ID association = %d\n", id_assoc);

    // =========================
    // TEST assoc_var (cas inexistant)
    // =========================
    arbre_test.ligne = 0;
    arbre_test.colonne = 4;

    case_test.ligne = 7;
    case_test.colonne = 7;

    id_assoc = assoc_var(m, arbre_test, case_test);

    printf("\nTest assoc_var pour arbre (%d,%d) et case (%d,%d)\n",
           arbre_test.ligne, arbre_test.colonne,
           case_test.ligne, case_test.colonne);

    printf("ID association = %d\n", id_assoc);

    // =========================
    // Libération mémoire
    // =========================
    free_mapSAT(m);
    free_grille(g);

    return 0;
}
