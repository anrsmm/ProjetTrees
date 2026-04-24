#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grille.h"



// Affichage simple de la grille d'entrée :
// T = arbre, . = case vide (pas la solution SAT).
void afficher_grille(Grille *g) {
    int i, j;

    if (g == NULL) {
        printf("Grille NULL\n");
        return;
    }

    printf("\nGrille (%d x %d):\n", g->Hauteur, g->Largeur);

    for (i = 0; i < g->Hauteur; i++) {
        for (j = 0; j < g->Largeur; j++) {
            if (g->est_tree[i][j]) {
                printf("T");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}




// Lecture d'une instance depuis fichier texte.
// Format attendu :
// 1) Hauteur Largeur
// 2) Hauteur entiers (indices de lignes)
// 3) Largeur entiers (indices de colonnes)
// 4) Hauteur lignes de caracteres : 'T' (arbre) ou '.' (vide)
Grille *lire_grille_fichier(const char *nom_fichier) {
    FILE *f;
    Grille *g;
    int Hauteur, Largeur;
    char *ligne;

    f = fopen(nom_fichier, "r");
    if (f == NULL) {
        return NULL;
    }

    if (fscanf(f, "%d %d", &Hauteur, &Largeur) != 2) {
        fclose(f);
        return NULL;
    }

    g = creer_grille(Hauteur, Largeur);
    if (g == NULL) {
        fclose(f);
        return NULL;
    }

    for (int i = 0; i < Hauteur; i++) {
        if (fscanf(f, "%d", &g->ligne_nbr[i]) != 1) {
            fclose(f);
            free_grille(g);
            return NULL;
        }
    }

    for (int j = 0; j < Largeur; j++) {
        if (fscanf(f, "%d", &g->col_nbr[j]) != 1) {
            fclose(f);
            free_grille(g);
            return NULL;
        }
    }

    ligne = malloc((Largeur + 1) * sizeof(char));
    if (ligne == NULL) {
        fclose(f);
        free_grille(g);
        return NULL;
    }

    for (int i = 0; i < Hauteur; i++) {
        if (fscanf(f, "%s", ligne) != 1) {
            free(ligne);
            fclose(f);
            free_grille(g);
            return NULL;
        }

        for (int j = 0; j < Largeur; j++) {
            Position p;
            p.ligne = i;
            p.colonne = j;

            if (ligne[j] == 'T') {
                // On enregistre l'arbre dans la matrice + dans la liste trees[]
                // (utile pour itérer plus vite ensuite).
                g->est_tree[i][j] = 1;
                g->trees[g->tree_nbr] = p;
                g->tree_nbr++;
            }
            else if (ligne[j] == '.') {
                // Idem pour les cases vides : matrice + liste empty_cases[].
                g->est_tree[i][j] = 0;
                g->empty_cases[g->empty_nbr] = p;
                g->empty_nbr++;
            }
            else {
                free(ligne);
                fclose(f);
                free_grille(g);
                return NULL;
            }
        }
    }

    free(ligne);
    fclose(f);
    return g;
}



// Alloue et initialise toutes les structures de la grille.
// On maintient en parallèle :
// - une matrice est_tree pour les tests rapides
// - des listes trees / empty_cases pour les parcours ciblés.
Grille *creer_grille(int Hauteur, int Largeur) {
    Grille *g;
    int i, j;

    if (Hauteur <= 0 || Largeur <= 0) {
        return NULL;
    }

    g = malloc(sizeof(Grille));
    if (g == NULL) {
        return NULL;
    }

    g->Hauteur = Hauteur;
    g->Largeur = Largeur;
    g->tree_nbr = 0;
    g->empty_nbr = 0;

    g->ligne_nbr = malloc(Hauteur * sizeof(int));
    if (g->ligne_nbr == NULL) {
        free(g);
        return NULL;
    }

    g->col_nbr = malloc(Largeur * sizeof(int));
    if (g->col_nbr == NULL) {
        free(g->ligne_nbr);
        free(g);
        return NULL;
    }

    for (i = 0; i < Hauteur; i++) {
        g->ligne_nbr[i] = 0;
    }

    for (i = 0; i < Largeur; i++) {
        g->col_nbr[i] = 0;
    }

    g->trees = malloc(Hauteur * Largeur * sizeof(Position));
    if (g->trees == NULL) {
        free(g->col_nbr);
        free(g->ligne_nbr);
        free(g);
        return NULL;
    }

    g->empty_cases = malloc(Hauteur * Largeur * sizeof(Position));
    if (g->empty_cases == NULL) {
        free(g->trees);
        free(g->col_nbr);
        free(g->ligne_nbr);
        free(g);
        return NULL;
    }

    g->est_tree = malloc(Hauteur * sizeof(int *));
    if (g->est_tree == NULL) {
        free(g->empty_cases);
        free(g->trees);
        free(g->col_nbr);
        free(g->ligne_nbr);
        free(g);
        return NULL;
    }

    for (i = 0; i < Hauteur; i++) {
        g->est_tree[i] = malloc(Largeur * sizeof(int));
        if (g->est_tree[i] == NULL) {
            for (j = 0; j < i; j++) {
                free(g->est_tree[j]);
            }
            free(g->est_tree);
            free(g->empty_cases);
            free(g->trees);
            free(g->col_nbr);
            free(g->ligne_nbr);
            free(g);
            return NULL;
        }
    }

    for (i = 0; i < Hauteur; i++) {
        for (j = 0; j < Largeur; j++) {
            g->est_tree[i][j] = 0;
        }
    }

    return g;
}



// Libère proprement la grille.
// Important : libération inverse des allocations.
void free_grille(Grille *g) {
	int i;

	if (g == NULL) {
        	return;
    	}

   	if (g->est_tree != NULL) {
        	for (i = 0; i < g->Hauteur; i++) {
            		free(g->est_tree[i]);
        	}
        free(g->est_tree);
    	}

    	free(g->ligne_nbr);
    	free(g->col_nbr);
    	free(g->trees);
    	free(g->empty_cases);


	free(g);
}

// Vérifie qu'une position (ligne,colonne) est dans les bornes de la grille.
int pos_valide(Grille *g, int ligne, int col){
	if (g == NULL){
//		printf("Grille vide");
		return 0;
	}
	if (ligne >=0 && ligne < g->Hauteur && col >=0 && col < g->Largeur){
//		printf("Position valide");
		return 1;
	} else {
//		printf("Position non valide");
		return 0;
	}
}

// Renvoie 1 si la case contient un arbre, 0 sinon.
// Fonction "safe" : protège contre grille NULL et positions invalides.
int case_est_tree(Grille *g, Position p){
	if (g == NULL) {
//		printf("Grille vide");
		return 0; //sans grille pas de case valide
	}
	if (!pos_valide(g, p.ligne, p.colonne)){
		return 0;//or grille on ne fait pas g->... car erreur mémoire
	}
	return (g->est_tree[p.ligne][p.colonne]);//on renvoie la valeur stockée
}


// Renvoie 1 si la case est vide.
// Ici "vide" est défini comme l'opposé de case_est_tree.
int case_est_vide(Grille *g, Position p){
	if (g == NULL) {
//		printf("Grille vide");
		return 0;// si la grille n'existe pas la case n'est pas vide
	}
	if (!pos_valide(g, p.ligne, p.colonne)){
		return 0;
	}
	return (!case_est_tree(g,p));
}
