CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -gdwarf-4

all: fcts_tests/test_grille1 fcts_tests/test_grille2 fcts_tests/test_voisin4 fcts_tests/test_voisin8 fcts_tests/test_sat fcts_tests/test_cnf fcts_tests/test_contrainte1 fcts_tests/test_contrainte2 fcts_tests/test_contrainte3 fcts_tests/test_contrainte4 fcts_tests/test_contrainte5 fcts_tests/test_contrainte6 fcts_tests/test_contrainte7 fcts_tests/test_card1 fcts_tests/test_card2 fcts_tests/test_card3 fcts_tests/test_ligne1 fcts_tests/test_lignes fcts_tests/test_colonnes fcts_tests/test_contraintesCOMPLET fcts_tests/test_dimacs fcts_tests/test_solution fcts_tests/test_lireDimacs fcts_tests/test_3sat grille-cnf minisat-grille resolutionTents cnf-3sat

# Cibles methode IA (cardinalite avec variables auxiliaires)
ia: resolutionTentsIA grille-cnfIA

fcts_tests/test_grille1: fcts_tests/test_grille1.o grille.o
	$(CC) $(CFLAGS) -o $@ $^

fcts_tests/test_grille2: fcts_tests/test_grille2.o grille.o
	$(CC) $(CFLAGS) -o $@ $^

fcts_tests/test_voisin4: fcts_tests/test_voisin4.o voisinage.o grille.o
	$(CC) $(CFLAGS) -o $@ $^

fcts_tests/test_voisin8: fcts_tests/test_voisin8.o voisinage.o grille.o
	$(CC) $(CFLAGS) -o $@ $^

fcts_tests/test_sat: fcts_tests/test_sat.o grille.o sat.o voisinage.o
	$(CC) $(CFLAGS) -o $@ $^

fcts_tests/test_cnf: fcts_tests/test_cnf.o cnf.o
	$(CC) $(CFLAGS) -o $@ $^

fcts_tests/test_contrainte1: fcts_tests/test_contrainte1.o grille.o cnf.o sat.o contraintes.o contraintesCard.o voisinage.o
	$(CC) $(CFLAGS) -o $@ $^

fcts_tests/test_contrainte2: fcts_tests/test_contrainte2.o grille.o cnf.o sat.o contraintes.o contraintesCard.o voisinage.o

fcts_tests/test_contrainte3: fcts_tests/test_contrainte3.o grille.o cnf.o sat.o contraintes.o contraintesCard.o voisinage.o

fcts_tests/test_contrainte4: fcts_tests/test_contrainte4.o grille.o cnf.o sat.o contraintes.o contraintesCard.o voisinage.o

fcts_tests/test_contrainte5: fcts_tests/test_contrainte5.o grille.o cnf.o sat.o contraintes.o contraintesCard.o voisinage.o

fcts_tests/test_contrainte6: fcts_tests/test_contrainte6.o grille.o cnf.o sat.o contraintes.o contraintesCard.o voisinage.o

fcts_tests/test_contrainte7: fcts_tests/test_contrainte7.o grille.o cnf.o sat.o contraintes.o contraintesCard.o voisinage.o


fcts_tests/test_card1: fcts_tests/test_card1.o cnf.o contraintesCard.o sat.o voisinage.o grille.o
fcts_tests/test_card2: fcts_tests/test_card2.o cnf.o contraintesCard.o sat.o voisinage.o grille.o
fcts_tests/test_card3: fcts_tests/test_card3.o cnf.o contraintesCard.o sat.o voisinage.o grille.o
fcts_tests/test_ligne1: fcts_tests/test_ligne1.o grille.o cnf.o sat.o contraintesCard.o voisinage.o
fcts_tests/test_lignes: fcts_tests/test_lignes.o grille.o cnf.o sat.o contraintesCard.o voisinage.o
fcts_tests/test_colonnes: fcts_tests/test_colonnes.o grille.o cnf.o sat.o contraintesCard.o voisinage.o

fcts_tests/test_contraintesCOMPLET: fcts_tests/test_contraintesCOMPLET.o grille.o cnf.o sat.o contraintes.o voisinage.o contraintesCard.o
fcts_tests/test_dimacs: fcts_tests/test_dimacs.o grille.o cnf.o sat.o contraintes.o voisinage.o contraintesCard.o dimacs.o

fcts_tests/test_solution: fcts_tests/test_solution.o grille.o cnf.o sat.o contraintes.o voisinage.o contraintesCard.o dimacs.o

fcts_tests/test_lireDimacs: fcts_tests/test_lireDimacs.o cnf.o 3sat.o
fcts_tests/test_3sat: fcts_tests/test_3sat.o cnf.o 3sat.o

grille-cnf: grille-cnf.o grille.o sat.o cnf.o contraintes.o contraintesCard.o voisinage.o dimacs.o
minisat-grille: minisat-grille.o grille.o sat.o voisinage.o dimacs.o
resolutionTents: resolutionTents.o grille.o sat.o cnf.o contraintes.o contraintesCard.o voisinage.o dimacs.o
cnf-3sat: cnf-3sat.o cnf.o 3sat.o

resolutionTentsIA: resolutionTents_IA.o grille.o sat.o cnf.o contraintes.o contraintesCard_IA.o voisinage.o dimacs.o
	$(CC) $(CFLAGS) -o $@ $^

grille-cnfIA: grille-cnf.o grille.o sat.o cnf.o contraintes.o contraintesCard_IA.o voisinage.o dimacs.o
	$(CC) $(CFLAGS) -o $@ $^


%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

fcts_tests/%.o: fcts_tests/%.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

grille.o: grille.h
fcts_tests/test_grille1.o: grille.h
fcts_tests/test_grille2.o: grille.h

voisinage.o: voisinage.h grille.h
fcts_tests/test_voisin4.o: voisinage.h grille.h
fcts_tests/test_voisin8.o: voisinage.h grille.h

sat.o: sat.h voisinage.h grille.h
fcts_tests/test_sat.o: sat.h grille.h

cnf.o: cnf.h
fcts_tests/test_cnf.o: cnf.h

contraintes.o: contraintes.h grille.h cnf.h sat.h contraintesCard.h
fcts_tests/test_contrainte1.o: grille.h cnf.h sat.h contraintes.h voisinage.h
fcts_tests/test_contrainte2.o: grille.h cnf.h sat.h contraintes.h voisinage.h
fcts_tests/test_contrainte3.o: grille.h cnf.h sat.h contraintes.h voisinage.h
fcts_tests/test_contrainte4.o: grille.h cnf.h sat.h contraintes.h voisinage.h
fcts_tests/test_contrainte5.o: grille.h cnf.h sat.h contraintes.h voisinage.h
fcts_tests/test_contrainte6.o: grille.h cnf.h sat.h contraintes.h voisinage.h
fcts_tests/test_contrainte7.o: grille.h cnf.h sat.h contraintes.h voisinage.h

contraintesCard.o: contraintesCard.h grille.h cnf.h sat.h
fcts_tests/test_card1.o: contraintesCard.h cnf.h
fcts_tests/test_card2.o: contraintesCard.h cnf.h
fcts_tests/test_card3.o: contraintesCard.h cnf.h
fcts_tests/test_ligne1.o: grille.h cnf.h sat.h contraintesCard.h
fcts_tests/test_lignes.o: grille.h cnf.h sat.h contraintesCard.h
fcts_tests/test_colonnes.o: grille.h cnf.h sat.h contraintesCard.h

fcts_tests/test_contraintesCOMPLET.o: grille.h cnf.h sat.h contraintes.h voisinage.h contraintesCard.h
dimacs.o: dimacs.h cnf.h sat.h
fcts_tests/test_dimacs.o: grille.h cnf.h sat.h contraintes.h voisinage.h contraintesCard.h dimacs.h
fcts_tests/test_solution.o: grille.h cnf.h sat.h contraintes.h voisinage.h contraintesCard.h dimacs.h

3sat.o: 3sat.h cnf.h
fcts_tests/test_lireDimacs.o: cnf.h 3sat.h
fcts_tests/test_3sat.o: cnf.h 3sat.h

grille-cnf.o: grille.h sat.h cnf.h contraintes.h dimacs.h
minisat-grille.o: grille.h sat.h dimacs.h
resolutionTents.o: grille.h sat.h cnf.h contraintes.h dimacs.h
resolutionTents_IA.o: grille.h sat.h cnf.h contraintes.h contraintesCard_IA.h dimacs.h
cnf-3sat.o: cnf.h 3sat.h
contraintesCard_IA.o: contraintesCard_IA.h grille.h cnf.h sat.h

clean:
	rm -f *.o fcts_tests/*.o fcts_tests/test_grille1 fcts_tests/test_grille2 fcts_tests/test_voisin4 fcts_tests/test_voisin8 fcts_tests/test_sat fcts_tests/test_cnf fcts_tests/test_contrainte1 fcts_tests/test_contrainte2 fcts_tests/test_contrainte3 fcts_tests/test_contrainte4 fcts_tests/test_contrainte5 fcts_tests/test_contrainte6 fcts_tests/test_contrainte7 fcts_tests/test_card1 fcts_tests/test_card2 fcts_tests/test_card3 fcts_tests/test_ligne1 fcts_tests/test_lignes fcts_tests/test_colonnes fcts_tests/test_contraintesCOMPLET fcts_tests/test_dimacs fcts_tests/test_solution fcts_tests/test_lireDimacs fcts_tests/test_3sat grille-cnf minisat-grille resolutionTents cnf-3sat resolutionTentsIA grille-cnfIA


.PHONY: all ia clean
