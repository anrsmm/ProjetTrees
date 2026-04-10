#include <stdio.h>
#include <stdlib.h>
#include "grille.h"
#include "voisinage.h"

int main(int argc, char *argv[]) {
    Grille *g;
    Position p;
    Position voisins[4];
    int nb, i;

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

    printf("\nTest voisins de (%d,%d)\n", p.ligne, p.colonne);

    nb = n4_voisins(g, p, voisins);

    printf("Nombre de voisins : %d\n", nb);
    for (i = 0; i < nb; i++) {
        printf("voisin %d : (%d,%d)\n", i, voisins[i].ligne, voisins[i].colonne);
    }

    free_grille(g);
    return 0;
}
