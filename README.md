# Projet Trees - Solveur SAT pour Tents and Trees

![Langage](https://img.shields.io/badge/langage-C-blue)
![Build](https://img.shields.io/badge/build-Makefile-success)
![Plateforme](https://img.shields.io/badge/plateforme-linux-lightgrey)
![Statut](https://img.shields.io/badge/statut-actif-brightgreen)
![INF402](https://moodle.caseine.org/course/view.php?id=1271)


Ce projet transforme le puzzle **Tents and Trees** en probleme **SAT**:
- lecture d'une grille au format texte
- generation d'une formule **CNF**
- export au format **DIMACS**
- resolution via un solveur SAT externe
- affichage de la grille resolue

---

## Demo rapide

```bash
make test_dimacs test_solution
./test_dimacs Tests/Tents66_1.txt Tests/Tents66.cnf
minisat Tests/Tents66.cnf Tests/resultat.txt
./test_solution Tests/Tents66_1.txt Tests/resultat.txt
```

Resultat: affichage de la **grille initiale** puis de la **grille solution** avec les tentes placees.

---

## Vue d'ensemble

```mermaid
flowchart LR
    A[Grille .txt] --> B[Mapping SAT]
    B --> C[Contraintes CNF]
    C --> D[Export DIMACS .cnf]
    D --> E[Solveur SAT externe]
    E --> F[Fichier SAT resultat]
    F --> G[Affichage de la solution]
```

Pipeline logique:
1. creation des variables SAT (cases tentes + associations arbre/case)
2. construction des contraintes (adjacence, cardinalites, lignes, colonnes)
3. ecriture DIMACS
4. lecture de la valuation SAT
5. rendu final de la grille

---

## Architecture du code

```text
.
├── grille.c / grille.h             # lecture, creation, destruction de grille
├── voisinage.c / voisinage.h       # voisinages 4/8 directions
├── sat.c / sat.h                   # variables SAT et mapping
├── cnf.c / cnf.h                   # clauses et formule CNF
├── contraintes.c / contraintes.h   # contraintes principales du puzzle
├── contraintesCard.c / .h          # contraintes de cardinalite
├── dimacs.c / dimacs.h             # ecriture .cnf, lecture solution SAT
├── test_*.c                        # programmes de tests / validation
├── Tests/                          # jeux d'instances et resultats
└── Makefile
```

---

## Prerequis

- `gcc`
- `make`
- solveur SAT externe (ex: `minisat`, `kissat`) pour la resolution finale

---

## Compilation

Tout compiler:

```bash
make
```

Nettoyer les binaires/objets:

```bash
make clean
```

---

## Format des fichiers d'entree

Exemple: `Tests/Tents66_1.txt`

```txt
6 6
2 1 1 1 0 2
1 1 1 1 0 3
...T..
.T...T
......
.T...T
T.....
....T.
```

- ligne 1: `Hauteur Largeur`
- ligne 2: nombres de tentes attendues par ligne
- ligne 3: nombres de tentes attendues par colonne
- lignes suivantes:
  - `T` = arbre
  - `.` = case vide

---

## Utilisation detaillee

### 1) Generer le DIMACS

```bash
./test_dimacs Tests/Tents66_1.txt Tests/Tents66.cnf
```

### 2) Lancer le solveur SAT

```bash
minisat Tests/Tents66.cnf Tests/resultat.txt
```

Le fichier resultat doit commencer par `SAT` puis contenir les litteraux affectes.

### 3) Afficher la solution sur la grille

```bash
./test_solution Tests/Tents66_1.txt Tests/resultat.txt
```

---

## Programmes principaux

- `test_dimacs`: construit la CNF complete et ecrit le fichier DIMACS
- `test_solution`: lit la solution SAT et affiche la grille resolue
- `test_contraintesCOMPLET`: affiche un extrait des clauses generees
- `test_grille1`, `test_voisin4`, `test_sat`, `test_cnf`, etc.: tests unitaires/module

---

## Rendre le repo encore plus visuel

Pour un rendu GitHub encore plus fort, tu peux ajouter:
- une capture `docs/demo-before-after.png`
- un GIF court `docs/demo.gif`
- un badge CI GitHub Actions

Exemple a inserer:

```md
![Demo](docs/demo-before-after.png)
```

---

## Roadmap

- [ ] script unique `run.sh` (CNF -> solveur -> affichage)
- [ ] integration CI (compilation + tests)
- [ ] benchmark sur plusieurs tailles de grilles
- [ ] visualisation graphique (terminal colore ou interface simple)

---

## Contexte

Projet realise dans le cadre du module INF402.

