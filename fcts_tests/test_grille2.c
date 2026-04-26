#include <stdio.h>
#include "grille.h"

//test lecture de grille depuis fichier.
int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: %s <fichier>\n", argv[0]);
    		return 1;
	}

	Grille *g = lire_grille_fichier(argv[1]);

	if (g == NULL) {
        	printf("Erreur de lecture du fichier\n");
        	return 1;
    	}

    printf("Grille : %d x %d\n", g->Hauteur, g->Largeur);
    printf("Nombre d'arbres : %d\n", g->tree_nbr);
    printf("Nombre de cases vides : %d\n", g->empty_nbr);

    free_grille(g);
    return 0;
}
