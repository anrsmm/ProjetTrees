CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -gdwarf-4

all: test_grille1 test_grille2 test_voisin4 test_voisin8

test_grille1: test_grille1.o grille.o
	$(CC) $(CFLAGS) -o $@ $^

test_grille2: test_grille2.o grille.o
	$(CC) $(CFLAGS) -o $@ $^

test_voisin4: test_voisin4.o voisinage.o grille.o
	$(CC) $(CFLAGS) -o $@ $^

test_voisin8: test_voisin8.o voisinage.o grille.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

grille.o: grille.h
test_grille1.o: grille.h
test_grille2.o: grille.h
test_voisin4.o: voisinage.h grille.h
test_voisin8.o: voisinage.h grille.h

clean:
	rm -f *.o test_grille1 test_grille2 test_voisin4 test_voisin8

.PHONY: all clean
