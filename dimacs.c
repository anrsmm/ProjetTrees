#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cnf.h"
#include "sat.h"

/*
Ecrit la formule CNF au format DIMACS standard.
Chaque clause finit par 0, comme attendu par les SAT-solvers.
 */
int ecrire_dimacs(const char *nom_fichier, CNFformule *f, SATmap *m) {
    if (nom_fichier == NULL || f == NULL || m == NULL) {//si le fichier ou la formule ou le mapping nul-> formule vide
        return 0;
    }


    FILE *out;
    out = fopen(nom_fichier, "w");//ouverture du fichier en ecriture
    if (out == NULL) {
        return 0;
    }

    fprintf(out, "p cnf %d %d\n", m->total_var_num, f->num);//ecriture de l'en-tête "p cnf ..."

    for (int i = 0; i < f->num; i++) {//pour chaque clause
        for (int j = 0; j < f->clauses[i].taille; j++) {//pour chaque litt
            fprintf(out, "%d ", f->clauses[i].litt[j]);//ecriture du litt
        }
        fprintf(out, "0\n");//ecrire 0 en fin de clause
    }

    fclose(out);
    return 1;
}
/*
lire_solution:
lit la sortie SAT et remplit val[var].
 */
int lire_solution(const char *nom, int *val, int nb_vars) {

    if (nom == NULL || val == NULL) {
        return 0;
    }

    FILE *f = fopen(nom, "r");//ouverture du fichier en lecture
    if (f == NULL) {
        return 0;
    }

    char buffer[10];//tableau pour lire la ligne
    if (fscanf(f, "%9s", buffer) != 1) {//si format inccorrect-> formule vide
        fclose(f);
        return 0;
    }

    if (strcmp(buffer, "SAT") != 0) {//si format inccorrect-> formule vide
        // si UNSAT/autre, pas d'affectation
        fclose(f);
        return 0;
    }

    int x;
    while (fscanf(f, "%d", &x) == 1 && x != 0) {//tant que la lecture est correcte et que x != 0
        int var = abs(x);//variable courante
        if (var <= nb_vars) {//si var valide
            val[var] = (x > 0) ? 1 : 0;//si x > 0-> vrai sinon-> faux
        }
    }

    fclose(f);
    return 1;
}

/*
afficher_solution:
affiche la grille resolue (T/X/.).
 */
void afficher_solution(Grille *g, int *val) {
    if (g == NULL || val == NULL) {
        return;
    }

    for (int i = 0; i < g->Hauteur; i++) {//pour chaque ligne
        for (int j = 0; j < g->Largeur; j++) {//pour chaque colonne
            Position p = {i, j};//position courante
            if (case_est_tree(g, p)) {//si la case contient un arbre
                printf("T ");
            } else {
                int var = tente_var(g, p);//variable courante
                if (val[var] == 1) {//si la variable est vraie
                    printf("X ");//tente
                } else {
                    printf(". ");//vide
                }
            }
        }
        printf("\n");//ecriture fin ligne
    }
}
/*
ecrire_donnees_grilles_json:
exporte grille initiale + solution en JSON.
 */
int ecrire_donnees_grilles_json(const char *nom_fichier, Grille *g, int *val) {
    if (nom_fichier == NULL || g == NULL || val == NULL) {//si le fichier ou la grille ou la valeur nul-> formule vide
        return 0;
    }

    FILE *out = fopen(nom_fichier, "w");//ouverture du fichier en ecriture
    out = fopen(nom_fichier, "w");//ouverture du fichier en ecriture
    if (out == NULL) {
        return 0;
    }

    fprintf(out, "{\n");//ecriture de l'en-tête "{"

    fprintf(out, "  \"indicesLignes\": [");//ecriture de l'en-tête "indicesLignes": [
    for (int i = 0; i < g->Hauteur; i++) {//pour chaque ligne
        fprintf(out, "%d", g->ligne_nbr[i]);//ecriture de l'indice de la ligne
        if (i < g->Hauteur - 1) {//si existe autre ligne
            fprintf(out, ", ");//ecrit virgule
        }
    }
    fprintf(out, "],\n");//ecriture de la fin de la liste "indicesLignes": [

    fprintf(out, "  \"indicesColonnes\": [");//ecriture de l'en-tête "indicesColonnes": [
    for (int j = 0; j < g->Largeur; j++) {//pour chaque colonne
        fprintf(out, "%d", g->col_nbr[j]);//ecriture de l'indice de la colonne
        if (j < g->Largeur - 1) {//si existe autre col
            fprintf(out, ", ");//ecrit virgule
        }
    }
    fprintf(out, "],\n");//ecriture de la fin de la liste "indicesColonnes": [

    fprintf(out, "  \"grilleInitiale\": [\n");//ecriture de l'en-tête "grilleInitiale": [
    for (int i = 0; i < g->Hauteur; i++) {//pour chaque ligne
        fprintf(out, "    [");//ecriture de l'en-tête "    ["
        for (int j = 0; j < g->Largeur; j++) {//pour chaque colonne

            Position p = {i, j};//position courante
            
            if (case_est_tree(g, p)) {//si la case contient un arbre
                fprintf(out, "\"A\"");
            
            } else {//si la case contient une case vide
                fprintf(out, "\".\"");
            }
            
            
            
            
            if (j < g->Largeur - 1) {
                fprintf(out, ", ");
            }
        }
        fprintf(out, "]");//ecriture de la fin de la liste 
        if (i < g->Hauteur - 1) {//si existe autre ligne
            fprintf(out, ",");
        }
        fprintf(out, "\n");//ecriture de la fin de la ligne
    }
    fprintf(out, "  ],\n");//ecriture de la fin de la liste "grilleInitiale": [

    fprintf(out, "  \"grilleSolution\": [\n");//ecriture de l'en-tête "grilleSolution": [
    for (int i = 0; i < g->Hauteur; i++) {//pour chaque ligne

        fprintf(out, "    [");//ecriture de l'en-tête "    ["

        for (int j = 0; j < g->Largeur; j++) {//pour chaque colonne

            Position p = {i, j};//position courante


            if (case_est_tree(g, p)) {//si la case contient un arbre
                fprintf(out, "\"A\"");
            
            } else if (val[tente_var(g, p)] == 1) {
                fprintf(out, "\"T\"");
            
            } else {
                fprintf(out, "\".\"");
            }
            
            if (j < g->Largeur - 1) {//si existe autre col
                fprintf(out, ", ");
            }
        }
        fprintf(out, "]");//ecriture de la fin de la liste 
        if (i < g->Hauteur - 1) {
            fprintf(out, ",");
        }
        fprintf(out, "\n");
    }
    fprintf(out, "  ]\n");//ecriture de la fin de la liste "grilleSolution": [

    fprintf(out, "}\n");//ecriture de la fin de l'objet "{"
    fclose(out);
    return 1;
}

