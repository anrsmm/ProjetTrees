#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sat.h"

int tente_var(Grille *g, Position p){
	if (g == NULL) {
	//	printf("Erreur : grille NULL\n");
		return 0;
	}
	if (!pos_valide(g, p.ligne, p.colonne)){
	//	printf("Erreur : position invalide (%d, %d)\n", p.ligne, p.colonne);
		return 0;
	}
	int idT;
	idT = p.ligne * g->Largeur + p.colonne +1;

	return idT;
}





// int assoc_var_id( Position tree, Position case, AssociationVar *assoc_vars, int>



