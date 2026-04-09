CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

all:

grille.o: grille.c grille.h
	$(CC) $(CFLAGS) -c grille.c

voisinage.o: voisinage.c voisinage.h
	$(CC) $(CFLAGS) -c voisinage.c

clean:
	rm -f *.o test_grille
