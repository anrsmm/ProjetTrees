CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -gdwarf-4

all: test_grille1 test_grille2 test_voisin4 test_voisin8 test_sat test_cnf

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
%.o: %.c
	$(CC) $(CFLAGS) -c $<

grille.o: grille.h
test_grille1.o: grille.h
test_grille2.o: grille.h
test_voisin4.o: voisinage.h grille.h
test_voisin8.o: voisinage.h grille.h
voisinage.o: voisinage.h grille.h
sat.o: sat.h voisinage.h grille.h
test_sat.o: sat.h grille.h
cnf.o: cnf.h
test_cnf.o: cnf.h

clean:
	rm -f *.o test_grille1 test_grille2 test_voisin4 test_voisin8 test_sat test_cnf

.PHONY: all clean
