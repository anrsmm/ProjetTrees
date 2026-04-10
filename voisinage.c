#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "voisinage.h"


int n4_voisins(Grille *g, Position p, Position voisins[4]) {
    int nb = 0;
    Position q;

    if (g == NULL) {
        return 0;
    }

    if (!pos_valide(g, p.ligne, p.colonne)) {
        return 0;
    }

    // haut
    q.ligne = p.ligne - 1;
    q.colonne = p.colonne;
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q;
        nb++;
    }

    // bas
    q.ligne = p.ligne + 1;
    q.colonne = p.colonne;
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q;
        nb++;
    }

    // gauche
    q.ligne = p.ligne;
    q.colonne = p.colonne - 1;
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q;
        nb++;
    }

    // droite
    q.ligne = p.ligne;
    q.colonne = p.colonne + 1;
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q;
        nb++;
    }

    return nb;
}

int n8_voisins(Grille *g, Position p, Position voisins[8]) {
    int nb;
    Position q;

    if (g == NULL) {
        return 0;
    }

    if (!pos_valide(g, p.ligne, p.colonne)) {
        return 0;
    }

    // on commence par les voisins N4
    nb = n4_voisins(g, p, voisins);

    // haut-gauche
    q.ligne = p.ligne - 1;
    q.colonne = p.colonne - 1;
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q;
        nb++;
    }

    // haut-droite
    q.ligne = p.ligne - 1;
    q.colonne = p.colonne + 1;
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q;
        nb++;
    }

    // bas-gauche
    q.ligne = p.ligne + 1;
    q.colonne = p.colonne - 1;
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q;
        nb++;
    }

    // bas-droite
    q.ligne = p.ligne + 1;
    q.colonne = p.colonne + 1;
    if (pos_valide(g, q.ligne, q.colonne)) {
        voisins[nb] = q;
        nb++;
    }

    return nb;
}
