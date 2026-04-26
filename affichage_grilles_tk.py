import json
import math
import tkinter as tk
from pathlib import Path
from tkinter import messagebox

# Fichiers utilises par l'affichage.
FICHIER_DONNEES = "donnees_grilles.json"
FICHIER_IMAGE_ARBRE = "Images/arbre.png"
FICHIER_IMAGE_TENTE = "Images/tente.png"
# Parametres de mise en page.
MARGE_BORD = 36
FACTEUR_REDUCTION = 0.96
LARGEUR_MIN_FENETRE = 1300
HAUTEUR_MIN_FENETRE = 780
DECALAGE_VERTICAL_GRILLE = 30


"""
charger_donnees:
charge le JSON produit par le solveur.

 problemes a prendre en compte:
- fichier absent: la resolution n'a pas encore ete lancee.
- JSON invalide: ecriture coupee ou format non conforme.
"""
def charger_donnees(nom_fichier):
    chemin = Path(nom_fichier)
    if not chemin.exists():
        # Demande ia : CORRECTION : oubli de generer le JSON avant d'ouvrir l'UI. important sinon on ne peut pas afficher les grilles
        raise FileNotFoundError(
            f"Fichier introuvable: {nom_fichier}\n"
            "Lance d'abord: ./resolutionTents Tests/Grille1_66.txt"
        )

    try:
        return json.loads(chemin.read_text(encoding="utf-8"))
    except json.JSONDecodeError as erreur:
        raise ValueError("Format invalide dans donnees_grilles.json") from erreur


"""
charger_images_cases:
charge les images arbre/tente en objets PhotoImage Tkinter.

probleme classique:
- si le chemin image est faux, Tk ne peut rien afficher.
"""
def charger_images_cases(root):
    chemin_arbre = Path(FICHIER_IMAGE_ARBRE)
    if not chemin_arbre.exists():
        # demande ia : correction
        # mauvais chemin relatif selon le dossier courant.
        raise FileNotFoundError(f"Image introuvable: {FICHIER_IMAGE_ARBRE}")

    chemin_tente = Path(FICHIER_IMAGE_TENTE)
    if not chemin_tente.exists():
        raise FileNotFoundError(f"Image introuvable: {FICHIER_IMAGE_TENTE}")

    return {
        "A": tk.PhotoImage(file=str(chemin_arbre), master=root),#arbre
        "T": tk.PhotoImage(file=str(chemin_tente), master=root),#tente
    }


"""
image_pour_case:
retourne une image redimensionnee pour une taille de case donnee.

pourquoi ce cache:
- sans cache: redimensionnement refait a chaque case -> lent.
- avec cache: 1 seule image par (type, taille), reuse partout.
"""
def image_pour_case(cle, taille_case, images_brutes, cache_images):
    taille_cible = max(6, int(taille_case * 0.82))
    cle_cache = (cle, taille_cible)

    # Reutilise l'image deja calculee si dispo.
    if cle_cache in cache_images:
        return cache_images[cle_cache]

    image = images_brutes[cle]
    largeur = image.width()
    hauteur = image.height()

    # On réduit fortement les grandes images pour les faire tenir dans une case.
    # demande ia : correction
    # calcul du facteur, sinon image trop grande/pixellisee.
    facteur = max(
        1,
        math.ceil(largeur / taille_cible),
        math.ceil(hauteur / taille_cible),
    )
    image_reduite = image.subsample(facteur, facteur)

    # Garde en memoire pour les prochains dessins.
    cache_images[cle_cache] = image_reduite
    return image_reduite


"""
dessiner_une_grille:
dessine une grille (initiale ou solution) avec indices + images.

etapes:
1) positionner titre et indices
2) tracer contour + cases
3) dessiner arbre/tente selon la valeur de case
"""
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
    # demande ia : correction
    # IndexError si grille vide ou mal formee.

    x_grille = x0 + marge_gauche
    y_grille = y0 + marge_haut + DECALAGE_VERTICAL_GRILLE

    # Titre au-dessus de la grille.
    canvas.create_text(
        x_grille + (colonnes * taille_case) / 2,
        y0 + 14,
        text=titre,
        font=("Arial", police_titre, "bold"),
    )

    # Indices colonnes.
    for c in range(colonnes):
        x = x_grille + c * taille_case + taille_case / 2
        y = y_grille - int(taille_case * 0.45)
        canvas.create_text(x, y, text=str(indices_colonnes[c]), font=("Arial", police_indices, "bold"))

    # Indices lignes.
    for l in range(lignes):
        x = x_grille - int(marge_gauche * 0.45)
        y = y_grille + l * taille_case + taille_case / 2
        canvas.create_text(x, y, text=str(indices_lignes[l]), font=("Arial", police_indices, "bold"))

    # Cadre externe de la grille.
    canvas.create_rectangle(
        x_grille,
        y_grille,
        x_grille + colonnes * taille_case,
        y_grille + lignes * taille_case,
        width=3,
        outline="black",
    )

    # Dessin de chaque case + contenu eventuel (A/T).
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
                # Arbre: on place l'image centree dans la case.
                image_arbre = image_pour_case("A", taille_case, images_brutes, cache_images)
                canvas.create_image(
                    x1 + taille_case / 2, y1 + taille_case / 2, image=image_arbre
                )
            elif valeur == "T":
                # Tente: meme logique que pour l'arbre.
                image_tente = image_pour_case("T", taille_case, images_brutes, cache_images)
                canvas.create_image(
                    x1 + taille_case / 2, y1 + taille_case / 2, image=image_tente
                )


"""
calculer_style:
calcule tailles/marges/polices selon dimensions fenetre et grille.

pourquoi bornes min/max:
- eviter texte illisible quand fenetre petite.
- eviter cases enormes quand fenetre tres grande.
"""
def calculer_style(lignes, colonnes, largeur_zone, hauteur_zone):
    espace_entre_grilles = 64
    marge_gauche = 34
    marge_haut = 60
    hauteur_extra = 20
#demande ia : correction
    largeur_disponible = max(420, largeur_zone - 2 * MARGE_BORD)
    hauteur_disponible = max(320, hauteur_zone - 2 * MARGE_BORD)

    largeur_cases_disponible = largeur_disponible - espace_entre_grilles - 2 * marge_gauche
    hauteur_cases_disponible = hauteur_disponible - marge_haut - hauteur_extra

    # demande ia : correction
    # trouver une taille unique qui tient horizontalement ET verticalement.
    taille_par_largeur = (
        largeur_cases_disponible
    ) / (2 * colonnes)
    taille_par_hauteur = (
        hauteur_cases_disponible
    ) / lignes

    # demande ia : correction
    #  calcul de la taille des cases, sinon images trop grandes/petites.
    taille_case = int(min(taille_par_largeur, taille_par_hauteur) * FACTEUR_REDUCTION)
    taille_case = max(10, min(28, taille_case))

    police_indices = int(taille_case * 0.55)
    police_indices = max(10, police_indices)

    police_titre = int(taille_case * 0.75)
    police_titre = max(13, police_titre)

    return {#
        "taille_case": taille_case,
        "marge_haut": max(48, int(taille_case * 1.90)),
        "marge_gauche": max(20, int(taille_case * 1.00)),
        "espace_entre_grilles": max(40, int(taille_case * 2.20)),
        "police_titre": police_titre,
        "police_indices": police_indices,
    }


"""
dessiner_tout:
dessine les 2 grilles cote a cote: initiale puis solution.

"""
def dessiner_tout(canvas, donnees, largeur_zone, hauteur_zone, images_brutes, cache_images):
    # Donnees attendues dans le JSON.
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

    # demande ia : correction
    # Alignement : centrer 2 grilles + espace sans couper le contenu.
    x_depart = max(MARGE_BORD, int((largeur_zone - largeur_totale) / 2))
    y_depart = MARGE_BORD

    # Probleme correction IA : Nettoyage 
    canvas.delete("all")

    dessiner_une_grille(#pour grille initiale
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

    dessiner_une_grille(#pour grille solution
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


"""
lancer_fenetre:
point d'entree UI Tkinter.

gestion d'erreurs:
- si donnees/images invalides: popup explicite + fermeture propre.
"""
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

    # Canvas : aide pour dessiner les grilles.
    canvas = tk.Canvas(root, bg="white")
    canvas.pack(fill="both", expand=True)
    # demande ia : CORRECTION : conserver des refere Python pour eviter que les images soient "garbage-collectees".
    # Point piege Tkinter: sans references, les images peuvent disparaitre.
    canvas.images_cases = images_brutes
    canvas.images_cache = cache_images

    def au_redimensionnement(event):
        # demande ia : correction : ignore les evenements transitoires tres petits.
        # Evite redraw inutiles/instables pendant les phases de resize rapides.
        if event.width < 50 or event.height < 50:
            return
        # Redessine completement a la nouvelle taille.
        dessiner_tout(canvas, donnees, event.width, event.height, images_brutes, cache_images)

    canvas.bind("<Configure>", au_redimensionnement)

    root.mainloop()


if __name__ == "__main__":
    lancer_fenetre()
