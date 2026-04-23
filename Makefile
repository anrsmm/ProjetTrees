CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -gdwarf-4

all: test_grille1 test_grille2 test_voisin4 test_voisin8 test_sat test_cnf test_contrainte1 test_contrainte2 test_contrainte3 test_contrainte4 test_contrainte5 test_contrainte6 test_contrainte7 test_card1 test_card2 test_card3 test_ligne1 test_lignes test_colonnes test_contraintesCOMPLET test_dimacs test_solution

test_grille1: test_grille1.o grille.o
	$(CC) $(CFLAGS) -o $@ $^

test_grille2: test_grille2.o grille.o
	$(CC) $(CFLAGS) -o $@ $^

test_voisin4: test_voisin4.o voisinage.o grille.o
	$(CC) $(CFLAGS) -o $@ $^

test_voisin8: test_voisin8.o voisinage.o grille.o
	$(CC) $(CFLAGS) -o $@ $^

test_sat: test_sat.o grille.o sat.o voisinage.o
	$(CC) $(CFLAGS) -o $@ $^

test_cnf: test_cnf.o cnf.o
	$(CC) $(CFLAGS) -o $@ $^

test_contrainte1: test_contrainte1.o grille.o cnf.o sat.o contraintes.o contraintesCard.o voisinage.o
	$(CC) $(CFLAGS) -o $@ $^

test_contrainte2: test_contrainte2.o grille.o cnf.o sat.o contraintes.o contraintesCard.o voisinage.o

test_contrainte3: test_contrainte3.o grille.o cnf.o sat.o contraintes.o contraintesCard.o voisinage.o

test_contrainte4: test_contrainte4.o grille.o cnf.o sat.o contraintes.o contraintesCard.o voisinage.o

test_contrainte5: test_contrainte5.o grille.o cnf.o sat.o contraintes.o contraintesCard.o voisinage.o

test_contrainte6: test_contrainte6.o grille.o cnf.o sat.o contraintes.o contraintesCard.o voisinage.o

test_contrainte7: test_contrainte7.o grille.o cnf.o sat.o contraintes.o contraintesCard.o voisinage.o


test_card1: test_card1.o cnf.o contraintesCard.o sat.o voisinage.o grille.o
test_card2: test_card2.o cnf.o contraintesCard.o sat.o voisinage.o grille.o
test_card3: test_card3.o cnf.o contraintesCard.o sat.o voisinage.o grille.o
test_ligne1: test_ligne1.o grille.o cnf.o sat.o contraintesCard.o voisinage.o
test_lignes: test_lignes.o grille.o cnf.o sat.o contraintesCard.o voisinage.o
test_colonnes: test_colonnes.o grille.o cnf.o sat.o contraintesCard.o voisinage.o

test_contraintesCOMPLET: test_contraintesCOMPLET.o grille.o cnf.o sat.o contraintes.o voisinage.o contraintesCard.o
test_dimacs: test_dimacs.o grille.o cnf.o sat.o contraintes.o voisinage.o contraintesCard.o dimacs.o

test_solution: test_solution.o grille.o cnf.o sat.o contraintes.o voisinage.o contraintesCard.o dimacs.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

grille.o: grille.h
test_grille1.o: grille.h
test_grille2.o: grille.h

voisinage.o: voisinage.h grille.h
test_voisin4.o: voisinage.h grille.h
test_voisin8.o: voisinage.h grille.h

sat.o: sat.h voisinage.h grille.h
test_sat.o: sat.h grille.h

cnf.o: cnf.h
test_cnf.o: cnf.h

contraintes.o: contraintes.h grille.h cnf.h sat.h contraintesCard.h
test_contrainte1.o: grille.h cnf.h sat.h contraintes.h voisinage.h
test_contrainte2.o: grille.h cnf.h sat.h contraintes.h voisinage.h
test_contrainte3.o: grille.h cnf.h sat.h contraintes.h voisinage.h
test_contrainte4.o: grille.h cnf.h sat.h contraintes.h voisinage.h
test_contrainte5.o: grille.h cnf.h sat.h contraintes.h voisinage.h
test_contrainte6.o: grille.h cnf.h sat.h contraintes.h voisinage.h
test_contrainte7.o: grille.h cnf.h sat.h contraintes.h voisinage.h

contraintesCard.o: contraintesCard.h grille.h cnf.h sat.h
test_card1.o: contraintesCard.h cnf.h
test_card2.o: contraintesCard.h cnf.h
test_card3.o: contraintesCard.h cnf.h
test_ligne1.o: grille.h cnf.h sat.h contraintesCard.h
test_lignes.o: grille.h cnf.o sat.h contraintesCard.h
test_colonnes.o: grille.h cnf.o sat.h contraintesCard.h

test_contraintesCOMPLET.o: grille.h cnf.h sat.h contraintes.h voisinage.h contraintesCard.h
dimacs.o: dimacs.h cnf.h sat.h
test_dimacs.o: grille.h cnf.h sat.h contraintes.h voisinage.h contraintesCard.h dimacs.h
test_solution.o: grille.h cnf.h sat.h contraintes.h voisinage.h contraintesCard.h dimacs.h

clean:
	rm -f *.o test_grille1 test_grille2 test_voisin4 test_voisin8 test_sat test_cnf test_contrainte1 test_contrainte2 test_contrainte3 test_contrainte4 test_contrainte5 test_contrainte6 test_contrainte7 test_card1 test_card2 test_card3 test_ligne1 test_lignes test_colonnes test_contraintesCOMPLET test_dimacs test_solution


.PHONY: all clean
