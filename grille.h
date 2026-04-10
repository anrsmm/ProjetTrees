#ifndef GRILLE_H
#define GRILLE_H

typedef struct {
	int ligne;
	int colonne;
} Position;

typedef struct {
	int Hauteur;
	int Largeur;

	int **est_tree;

	int *ligne_nbr;
	int *col_nbr;

	Position *trees;
	int tree_nbr;

	Position *empty_cases;
	int empty_nbr;
} Grille;


//lecture de la grille rpésente dans le fichier .txt
Grille *lire_grille_fichier(const char *nom_fichier); 
// création/destruction
Grille*creer_grille(int Hauteur, int Largeur);
void free_grille(Grille*g);

// position dans la grille?
int pos_valide(Grille *g, int ligne, int col);

//type de case
//case contient un arbre?retourne 1 si arbre, 0 sinon
int case_est_tree(Grille *g, Position p);

//case est vide?retourne 1 si vide
int case_est_vide(Grille *g, Position p);

#endif
