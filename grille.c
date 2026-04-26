#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grille.h"
/*
ffichage  de la grille d'entrée :
T = arbre, . = case vide (pas la solution SAT).
 */
void afficher_grille(Grille *g) {
    int i, j;

    if (g == NULL) {
        printf("Grille NULL\n");
        return;
    }

    printf("\nGrille (%d x %d):\n", g->Hauteur, g->Largeur);//ecriture de l'en-tête "Grille (Hauteur x Largeur):"

    for (i = 0; i < g->Hauteur; i++) {//pour chaque ligne
        for (j = 0; j < g->Largeur; j++) {//pour chaque colonne
            if (g->est_tree[i][j]) {//si la case contient un arbre
                printf("T");
            } else {//si la case contient une case vide
                printf(".");
            }
        }
        printf("\n");//ecriture de la fin de ligne
    }
}
/*
lire_grille_fichier:
lit instance depuis fichier texte
 Format attendu :
 Hauteur Largeur
Hauteur entiers (indices de lignes)
Largeur entiers (indices de colonnes)
Hauteur lignes de caracteres : 'T' (arbre) ou '.' (vide)
 */
Grille *lire_grille_fichier(const char *nom_fichier) {
    FILE *f;
    int Hauteur;
    int Largeur;


    f = fopen(nom_fichier, "r");//ouverture du fichier en lecture
    if (f == NULL) {
        return NULL;
    }

    if (fscanf(f, "%d %d", &Hauteur, &Largeur) != 2) {//si le format n'est pas correct-> grille vide
        fclose(f);
        return NULL;
    }
    Grille *g;
    g = creer_grille(Hauteur, Largeur);//creation de la grille
    if (g == NULL) {
        fclose(f);
        return NULL;
    }

    char *ligne;//ligne courante
    for (int i = 0; i < Hauteur; i++) {//pour chaque ligne
        if (fscanf(f, "%d", &g->ligne_nbr[i]) != 1) {//si le format n'est pas correct-> grille vide
            fclose(f);
            free_grille(g);
            return NULL;
        }
    }

    for (int j = 0; j < Largeur; j++) {//pour chaque colonne
        if (fscanf(f, "%d", &g->col_nbr[j]) != 1) {//si le format n'est pas correct-> grille vide
            fclose(f);
            free_grille(g);
            return NULL;
        }
    }

    ligne = malloc((Largeur + 1) * sizeof(char));//aloc ligne
    if (ligne == NULL) {
        fclose(f);
        free_grille(g);
        return NULL;
    }

    for (int i = 0; i < Hauteur; i++) {//pour chaque ligne
        if (fscanf(f, "%s", ligne) != 1) {//si le format n'est pas correct-> grille vide
            free(ligne);
            fclose(f);
            free_grille(g);
            return NULL;
        }

        for (int j = 0; j < Largeur; j++) {//pour chaque colonne
            Position p;//position courante
            p.ligne = i;//ligne courante
            p.colonne = j;//colonne courante

            if (ligne[j] == 'T') {
                // enregistre arbre dans la matrice + dans la liste trees[] -> utile pour accéder plus vite aux arbres
                g->est_tree[i][j] = 1;//arbre dans la matrice
                g->trees[g->tree_nbr] = p;//arbre dans la liste trees[]
                g->tree_nbr++;//pour nb d arbres
            }

            else if (ligne[j] == '.') {
                // vide: matrice + liste empty_cases[]
                g->est_tree[i][j] = 0;//case vide dans la matrice
                g->empty_cases[g->empty_nbr] = p;//case vide dans la liste empty_cases[]
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
/*
creer_grille:
alloue et initialise toutes les structures de la grille.
 */
Grille *creer_grille(int Hauteur, int Largeur) {
    Grille *g;

    if (Hauteur <= 0 || Largeur <= 0) {
        return NULL;
    }

    g = malloc(sizeof(Grille));//allocagrille
    if (g == NULL) {
        return NULL;
    }

    g->Hauteur = Hauteur;//hauteur de la grille
    g->Largeur = Largeur;//largeur de la grille
    g->tree_nbr = 0;//nb d arbres
    g->empty_nbr = 0;//nb de cases vides

    g->ligne_nbr = malloc(Hauteur * sizeof(int));//alloc indices de lignes
    if (g->ligne_nbr == NULL) {//si l'allocation a echoue-> grille vide
        free(g);
        return NULL;
    }

    g->col_nbr = malloc(Largeur * sizeof(int));//alloc indices de colonnes
    if (g->col_nbr == NULL) {//si l'allocation a echoue-> grille vide
        free(g->ligne_nbr);
        free(g);
        return NULL;
    }

    for (int i = 0; i < Hauteur; i++) {//pour chaque ligne
        g->ligne_nbr[i] = 0;//initialisation des indices de lignes
    }

    for (int i = 0; i < Largeur; i++) {//pour chaque colonne
        g->col_nbr[i] = 0;//initialisation des indices de colonnes
    }

    g->trees = malloc(Hauteur * Largeur * sizeof(Position));//alloc liste d arbres
    if (g->trees == NULL) {//si l'allocation a echoue-> grille vide
        free(g->col_nbr);//libere les indices de colonnes
        free(g->ligne_nbr);//libere les indices de lignes
        free(g);
        return NULL;
    }

    g->empty_cases = malloc(Hauteur * Largeur * sizeof(Position));//alloc liste de cases vides
    if (g->empty_cases == NULL) {//si l'allocation a echoue-> grille vide
        free(g->trees);
        free(g->col_nbr);
        free(g->ligne_nbr);
        free(g);
        return NULL;
    }

    g->est_tree = malloc(Hauteur * sizeof(int *));//alloc matrice d arbres
    if (g->est_tree == NULL) {//si l'allocation a echoue-> grille vide
        free(g->empty_cases);
        free(g->trees);
        free(g->col_nbr);
        free(g->ligne_nbr);
        free(g);
        return NULL;
    }

    for (int i = 0; i < Hauteur; i++) {//pour chaque ligne
        g->est_tree[i] = malloc(Largeur * sizeof(int));//alloc ligne i de la matrice d arbres
        if (g->est_tree[i] == NULL) {//si l'allocation a echoue-> grille vide
            for (int j = 0; j < i; j++) {//pour chaque colonne
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

    for (int i = 0; i < Hauteur; i++) {//pour chaque ligne
        for (int j = 0; j < Largeur; j++) {//pour chaque colonne
            g->est_tree[i][j] = 0;//initialis matrice d arbres
        }
    }

    return g;//retourne grille
}
/*
fonction free_grille:
libere la grille et tous ses tableaux.
 */
void free_grille(Grille *g) {
	if (g == NULL) {
        	return;
    	}

   	if (g->est_tree != NULL) {//si la matrice n'est pas vide
        	for (int i = 0; i < g->Hauteur; i++) {//pour chaque ligne
            		free(g->est_tree[i]); //libere la ligne
        	}
        free(g->est_tree); //libere la matrice
    	}

    	free(g->ligne_nbr); //libere les indices de lignes
    	free(g->col_nbr); //libere les indices de colonnes
    	free(g->trees); 
    	free(g->empty_cases); //libere les cases vides


	free(g);
}

/*
pos_valide:
teste si (ligne,colonne) est dans la grille.
 */
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

/*
case_est_tree:
renvoie 1 si la case contient un arbre.
 */
int case_est_tree(Grille *g, Position p){
	if (g == NULL) {
//		printf("Grille vide");
		return 0; //sans grille pas de case valide
	}
	if (!pos_valide(g, p.ligne, p.colonne)){
		return 0;
	}
	return (g->est_tree[p.ligne][p.colonne]); // valeur matrice pourcase (i,j)
}

/*
case_est_vide:
renvoie 1 si la case est vide.
 */
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
