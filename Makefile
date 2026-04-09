CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

all: test_grille

test_grille: test_grille.o grille.o
	$(CC) $(CFLAGS) -o test_grille test_grille.o grille.o

test_grille.o: test_grille.c grille.h
	$(CC) $(CFLAGS) -c test_grille.c

grille.o: grille.c grille.h
	$(CC) $(CFLAGS) -c grille.c

clean:
	rm -f *.o test_grille
