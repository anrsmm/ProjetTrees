#include <stdio.h>
#include <stdlib.h>

#include "grille.h"
#include "sat.h"

int main(int argc, char *argv[]) {
    Grille *g;
    Position p;

    if (argc < 4) {
        printf("Usage: %s <fichier> <ligne> <colonne>\n", argv[0]);
        return 1;
    }

    g = lire_grille_fichier(argv[1]);
    if (g == NULL) {
        printf("Erreur lecture fichier\n");
        return 1;
    }

    p.ligne = atoi(argv[2]);
    p.colonne = atoi(argv[3]);

    afficher_grille(g);

    printf("\nTest variable tente pour (%d,%d)\n", p.ligne, p.colonne);

    int id = tente_var(g, p);

    printf("ID DIMACS = %d\n", id);

    free_grille(g);
    return 0;
}
