#ifndef CONTRAINTES_H
#define CONTRAINTES_H

#include "grille.h"
#include "cnf.h"
#include "sat.h"

/*------------------------------------------------------
CONTRAINTE 1 : UNE CASE CONTENANT UN ARBRE NE PEUT PAS
CONTENIR DE TENTE
------------------------------------------------------*/

void contrainte1(Grille *g, CNFformule *f);

/*------------------------------------------------------
CONTRAINTE 2 : SI UNE CASE EST ASSOCIEE A UN ARBRE ALORS
C'EST UNE TENTE
------------------------------------------------------*/
void contrainte2(Grille *g, SATmap *m, CNFformule *f);

/*------------------------------------------------------
CONTRAINTE 3 : "SI UNE CASE CONTIENT UNE TENTE, ELLE DOIT
ETRE ASSOCIEE A AU MOINS UN ARBRE VOISIN".
------------------------------------------------------*/
void contrainte3(Grille *g, SATmap *m, CNFformule *f);

/*------------------------------------------------------
CONTRAINTE 4 : "CHAQUE ARBRE DOIT ETRE ASSOCIE A AU
MOINS UNE CASE VIDE VOISINE".
------------------------------------------------------*/
void contrainte4(Grille *g, SATmap *m, CNFformule *f);

/*------------------------------------------------------
CONTRAINTE 5 : "UN ARBRE NE PEUT PAS ETRE ASSOCIE A 
DEUX TERMES DIFFERENTS
------------------------------------------------------*/
void contrainte5(Grille *g, SATmap *m, CNFformule *f);

/*------------------------------------------------------
CONTRAINTE 6 : "UNE TENTE NE PEUT PAS ETRE ASSOCIEE
A DEUX ARBRES DIFFERENTS". 
------------------------------------------------------*/
void contrainte6(Grille *g, SATmap *m, CNFformule *f);

////////////////////////////////////////////////////////
/*FONCTION DE COMPARAISON*/
int comparPosition(Position a, Position b);

/*------------------------------------------------------
CONTRAINTE 7 : "LES TENTES NE PEUVENT PAS SE TOUCHEES
------------------------------------------------------*/
void contrainte7(Grille *g, CNFformule *f);

////////////////////////////////////////////////////////
/*------------------------------------------------------
CONSTRUCTION DE LA FORMULE COMPLETE
------------------------------------------------------*/
void construire_cnf_complete(Grille *g, SATmap *m, CNFformule *f);


#endif
