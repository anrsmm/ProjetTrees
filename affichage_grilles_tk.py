import json
import math
import tkinter as tk
from pathlib import Path
from tkinter import messagebox


FICHIER_DONNEES = "donnees_grilles.json"
FICHIER_IMAGE_ARBRE = "arbre.png"
FICHIER_IMAGE_TENTE = "tente.png"
MARGE_BORD = 36
FACTEUR_REDUCTION = 0.96
LARGEUR_MIN_FENETRE = 1300
HAUTEUR_MIN_FENETRE = 780
DECALAGE_VERTICAL_GRILLE = 30


def charger_donnees(nom_fichier):
    chemin = Path(nom_fichier)
    if not chemin.exists():
        raise FileNotFoundError(
            f"Fichier introuvable: {nom_fichier}\n"
            "Lance d'abord: ./resolutionTents Tests/Grille1_66.txt"
        )

    try:
        return json.loads(chemin.read_text(encoding="utf-8"))
    except json.JSONDecodeError as erreur:
        raise ValueError("Format invalide dans donnees_grilles.json") from erreur


def charger_images_cases(root):
    chemin_arbre = Path(FICHIER_IMAGE_ARBRE)
    if not chemin_arbre.exists():
        raise FileNotFoundError(f"Image introuvable: {FICHIER_IMAGE_ARBRE}")

    chemin_tente = Path(FICHIER_IMAGE_TENTE)
    if not chemin_tente.exists():
        raise FileNotFoundError(f"Image introuvable: {FICHIER_IMAGE_TENTE}")

    return {
        "A": tk.PhotoImage(file=str(chemin_arbre), master=root),
        "T": tk.PhotoImage(file=str(chemin_tente), master=root),
    }


def image_pour_case(cle, taille_case, images_brutes, cache_images):
    taille_cible = max(6, int(taille_case * 0.82))
    cle_cache = (cle, taille_cible)

    if cle_cache in cache_images:
        return cache_images[cle_cache]

    image = images_brutes[cle]
    largeur = image.width()
    hauteur = image.height()

    # On réduit fortement les grandes images pour les faire tenir dans une case.
    facteur = max(
        1,
        math.ceil(largeur / taille_cible),
        math.ceil(hauteur / taille_cible),
    )
    image_reduite = image.subsample(facteur, facteur)

    cache_images[cle_cache] = image_reduite
    return image_reduite


def dessiner_une_grille(
    canvas, x0, y0, titre, grille, indices_lignes, indices_colonnes, style, images_brutes, cache_images
):
    taille_case = style["taille_case"]
    marge_haut = style["marge_haut"]
    marge_gauche = style["marge_gauche"]
    police_titre = style["police_titre"]
    police_indices = style["police_indices"]

    lignes = len(grille)
    colonnes = len(grille[0])

    x_grille = x0 + marge_gauche
    y_grille = y0 + marge_haut + DECALAGE_VERTICAL_GRILLE

    canvas.create_text(
        x_grille + (colonnes * taille_case) / 2,
        y0 + 14,
        text=titre,
        font=("Arial", police_titre, "bold"),
    )

    for c in range(colonnes):
        x = x_grille + c * taille_case + taille_case / 2
        y = y_grille - int(taille_case * 0.45)
        canvas.create_text(x, y, text=str(indices_colonnes[c]), font=("Arial", police_indices, "bold"))

    for l in range(lignes):
        x = x_grille - int(marge_gauche * 0.45)
        y = y_grille + l * taille_case + taille_case / 2
        canvas.create_text(x, y, text=str(indices_lignes[l]), font=("Arial", police_indices, "bold"))

    canvas.create_rectangle(
        x_grille,
        y_grille,
        x_grille + colonnes * taille_case,
        y_grille + lignes * taille_case,
        width=3,
        outline="black",
    )

    for l in range(lignes):
        for c in range(colonnes):
            x1 = x_grille + c * taille_case
            y1 = y_grille + l * taille_case
            x2 = x1 + taille_case
            y2 = y1 + taille_case

            canvas.create_rectangle(
                x1, y1, x2, y2, fill="#8EE68E", outline="#1F6B1F", width=1
            )

            valeur = grille[l][c]

            if valeur == "A":
                image_arbre = image_pour_case("A", taille_case, images_brutes, cache_images)
                canvas.create_image(
                    x1 + taille_case / 2, y1 + taille_case / 2, image=image_arbre
                )
            elif valeur == "T":
                image_tente = image_pour_case("T", taille_case, images_brutes, cache_images)
                canvas.create_image(
                    x1 + taille_case / 2, y1 + taille_case / 2, image=image_tente
                )


def calculer_style(lignes, colonnes, largeur_zone, hauteur_zone):
    espace_entre_grilles = 64
    marge_gauche = 34
    marge_haut = 60
    hauteur_extra = 20

    largeur_disponible = max(420, largeur_zone - 2 * MARGE_BORD)
    hauteur_disponible = max(320, hauteur_zone - 2 * MARGE_BORD)

    largeur_cases_disponible = largeur_disponible - espace_entre_grilles - 2 * marge_gauche
    hauteur_cases_disponible = hauteur_disponible - marge_haut - hauteur_extra

    taille_par_largeur = (
        largeur_cases_disponible
    ) / (2 * colonnes)
    taille_par_hauteur = (
        hauteur_cases_disponible
    ) / lignes

    taille_case = int(min(taille_par_largeur, taille_par_hauteur) * FACTEUR_REDUCTION)
    taille_case = max(10, min(28, taille_case))

    police_indices = int(taille_case * 0.55)
    police_indices = max(10, police_indices)

    police_titre = int(taille_case * 0.75)
    police_titre = max(13, police_titre)

    return {
        "taille_case": taille_case,
        "marge_haut": max(48, int(taille_case * 1.90)),
        "marge_gauche": max(20, int(taille_case * 1.00)),
        "espace_entre_grilles": max(40, int(taille_case * 2.20)),
        "police_titre": police_titre,
        "police_indices": police_indices,
    }


def dessiner_tout(canvas, donnees, largeur_zone, hauteur_zone, images_brutes, cache_images):
    grille_initiale = donnees["grilleInitiale"]
    grille_solution = donnees["grilleSolution"]
    indices_lignes = donnees["indicesLignes"]
    indices_colonnes = donnees["indicesColonnes"]
    lignes = len(grille_initiale)
    colonnes = len(grille_initiale[0])
    style = calculer_style(lignes, colonnes, largeur_zone, hauteur_zone)

    largeur_grille = style["marge_gauche"] + colonnes * style["taille_case"]
    hauteur_grille = style["marge_haut"] + lignes * style["taille_case"]
    largeur_totale = largeur_grille * 2 + style["espace_entre_grilles"]

    x_depart = max(MARGE_BORD, int((largeur_zone - largeur_totale) / 2))
    y_depart = MARGE_BORD

    canvas.delete("all")

    dessiner_une_grille(
        canvas,
        x_depart,
        y_depart,
        "Grille initiale",
        grille_initiale,
        indices_lignes,
        indices_colonnes,
        style,
        images_brutes,
        cache_images,
    )

    dessiner_une_grille(
        canvas,
        x_depart + largeur_grille + style["espace_entre_grilles"],
        y_depart,
        "Grille solution",
        grille_solution,
        indices_lignes,
        indices_colonnes,
        style,
        images_brutes,
        cache_images,
    )
def lancer_fenetre():
    root = tk.Tk()
    root.title("Affichage des grilles - Tents and Trees")
    root.geometry(f"{LARGEUR_MIN_FENETRE}x{HAUTEUR_MIN_FENETRE}")

    try:
        donnees = charger_donnees(FICHIER_DONNEES)
        images_brutes = charger_images_cases(root)
    except Exception as erreur:
        root.withdraw()
        messagebox.showerror("Erreur", str(erreur), parent=root)
        root.destroy()
        return

    cache_images = {}

    canvas = tk.Canvas(root, bg="white")
    canvas.pack(fill="both", expand=True)
    canvas.images_cases = images_brutes
    canvas.images_cache = cache_images

    def au_redimensionnement(event):
        if event.width < 50 or event.height < 50:
            return
        dessiner_tout(canvas, donnees, event.width, event.height, images_brutes, cache_images)

    canvas.bind("<Configure>", au_redimensionnement)

    root.mainloop()


if __name__ == "__main__":
    lancer_fenetre()
