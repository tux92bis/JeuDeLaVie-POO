import streamlit as st
import numpy as np
import time
from io import StringIO
from PIL import Image, ImageDraw
import base64
from streamlit_drawable_canvas import st_canvas

st.set_page_config(page_title="Jeu de la Vie", layout="centered")

# -----------------------------------
# Fonctions utilitaires
# -----------------------------------

def generer_grille_vide(lignes, colonnes):
    return np.zeros((lignes, colonnes), dtype=int)

def prochaine_generation(grille, torique=False):
    lignes, colonnes = grille.shape
    nouvelle = np.zeros((lignes, colonnes), dtype=int)
    for i in range(lignes):
        for j in range(colonnes):
            vivants = 0
            for x in [-1, 0, 1]:
                for y in [-1, 0, 1]:
                    if x == 0 and y == 0:
                        continue
                    nx = i + x
                    ny = j + y
                    if torique:
                        nx = nx % lignes
                        ny = ny % colonnes
                    else:
                        if nx < 0 or nx >= lignes or ny < 0 or ny >= colonnes:
                            continue
                    vivants += grille[nx, ny]
            # Règles standard de Conway
            if grille[i, j] == 1:
                nouvelle[i, j] = 1 if vivants in [2, 3] else 0
            else:
                nouvelle[i, j] = 1 if vivants == 3 else 0
    return nouvelle

def dessiner_grille(grille, taille_cellule=20):
    lignes, colonnes = grille.shape
    img = Image.new('RGB', (colonnes * taille_cellule, lignes * taille_cellule), color='white')
    draw = ImageDraw.Draw(img)
    for i in range(lignes):
        for j in range(colonnes):
            couleur = (0, 0, 0) if grille[i, j] == 1 else (255, 255, 255)
            x1 = j * taille_cellule
            y1 = i * taille_cellule
            x2 = x1 + taille_cellule
            y2 = y1 + taille_cellule
            draw.rectangle([x1, y1, x2, y2], fill=couleur, outline=(200,200,200))
    return img

def grille_to_text(grille):
    l, c = grille.shape
    lines = []
    lines.append(f"{l} {c}")
    for i in range(l):
        row = []
        for j in range(c):
            row.append(str(grille[i, j]))
        lines.append(' '.join(row))
    return "\n".join(lines)

def charger_etat_depuis_fichier(fichier) -> np.ndarray:
    content = fichier.read().decode('utf-8')
    lignes_f = content.strip().split('\n')
    # Première ligne : nombre de lignes et de colonnes
    dims = lignes_f[0].split()
    l, c = int(dims[0]), int(dims[1])
    grille = np.zeros((l, c), dtype=int)
    for i, line in enumerate(lignes_f[1:]):
        vals = line.split()
        for j, val in enumerate(vals):
            cellule = int(val)
            grille[i, j] = 1 if cellule != 0 else 0
    return grille

def telecharger_etats(etats, filename="historique_etats.txt"):
    output = []
    for idx, e in enumerate(etats):
        output.append(f"Generation {idx}:")
        output.append(grille_to_text(e))
        output.append("")  
    txt = "\n".join(output)
    b64 = base64.b64encode(txt.encode()).decode()
    href = f'<a href="data:file/txt;base64,{b64}" download="{filename}">Télécharger l\'historique des états</a>'
    return href

def detecter_stabilite(etats, nb=3):
    # Détecte si les nb derniers états sont identiques
    if len(etats) < nb:
        return False
    last = etats[-1]
    for i in range(2, nb+1):
        if not np.array_equal(last, etats[-i]):
            return False
    return True

# -----------------------------------
# État de l'application
# -----------------------------------
if 'grille' not in st.session_state:
    st.session_state.grille = None
if 'iteration' not in st.session_state:
    st.session_state.iteration = 0
if 'en_cours' not in st.session_state:
    st.session_state.en_cours = False
if 'etats' not in st.session_state:
    st.session_state.etats = []

# -----------------------------------
# Barre latérale de configuration
# -----------------------------------
st.sidebar.title("Paramètres")
lignes = st.sidebar.number_input("Lignes", min_value=10, max_value=200, value=20, step=1)
colonnes = st.sidebar.number_input("Colonnes", min_value=10, max_value=200, value=20, step=1)
torique = st.sidebar.checkbox("Bords toriques (wrap-around)", value=False)
mode_stable = st.sidebar.checkbox("Arrêter automatiquement quand stable (3 itérations identiques)", value=False)

iterations_demandees = st.sidebar.number_input("Nombre d'itérations max", min_value=1, value=100, step=10)

fichier_upload = st.sidebar.file_uploader("Charger un fichier texte (0/1)", type=["txt"])

if st.sidebar.button("Initialiser / Réinitialiser"):
    if fichier_upload is not None:
        st.session_state.grille = charger_etat_depuis_fichier(fichier_upload)
    else:
        st.session_state.grille = generer_grille_vide(lignes, colonnes)
    st.session_state.iteration = 0
    st.session_state.en_cours = False
    st.session_state.etats = [st.session_state.grille.copy()]

if st.sidebar.button("Lancer"):
    if st.session_state.grille is not None:
        st.session_state.en_cours = True

if st.sidebar.button("Pause"):
    st.session_state.en_cours = False

if st.sidebar.button("Étape suivante"):
    if st.session_state.grille is not None:
        st.session_state.grille = prochaine_generation(st.session_state.grille, torique)
        st.session_state.iteration += 1
        st.session_state.etats.append(st.session_state.grille.copy())

# -----------------------------------
# Zone principale
# -----------------------------------
st.title("Jeu de la Vie de Conway (0 = mort, 1 = vivant)")

st.write("Veuillez initialiser la grille. Vous pouvez :")
st.write("- Charger un fichier texte (0/1) dans la barre latérale.")
st.write("- Ou dessiner vous-même la grille (noir = vivant, blanc = mort) puis ré-initialiser.")
st.write("Ensuite, lancez, mettez en pause, ou passez des étapes.")

if st.session_state.grille is None:
    st.info("Veuillez initialiser une grille dans la barre latérale.")
else:
    taille_cellule = 15
    largeur = colonnes * taille_cellule
    hauteur = lignes * taille_cellule

    img_init = dessiner_grille(st.session_state.grille, taille_cellule=taille_cellule)

    st.write("Dessinez la grille (noir = vivant, blanc = mort), puis re-cliquez sur 'Initialiser/Réinitialiser' après avoir modifié le dessin pour l'appliquer :")

    canvas_result = st_canvas(
        fill_color="black",
        stroke_width=1,
        stroke_color="black",
        background_color="white",
        width=largeur,
        height=hauteur,
        drawing_mode="freedraw",
        key="canvas"
    )

    # Note : le canvas est utilisé pour aider à dessiner. 
    # Pour appliquer réellement le dessin dans la grille, il faut cliquer "Initialiser/Réinitialiser".
    # Si vous souhaitez appliquer en direct sans re-cliquer, décommentez le code suivant :
    # if canvas_result.image_data is not None:
    #     img_array = canvas_result.image_data
    #     new_grille = np.zeros((lignes, colonnes), dtype=int)
    #     for i in range(lignes):
    #         for j in range(colonnes):
    #             px = img_array[i*taille_cellule+(taille_cellule//2), j*taille_cellule+(taille_cellule//2)]
    #             if px[0]<128 and px[1]<128 and px[2]<128:
    #                 new_grille[i,j] = 1
    #             else:
    #                 new_grille[i,j] = 0
    #     st.session_state.grille = new_grille

    st.write(f"Itération actuelle : {st.session_state.iteration}")

    if st.session_state.en_cours and st.session_state.grille is not None:
        if st.session_state.iteration < iterations_demandees:
            nouvelle = prochaine_generation(st.session_state.grille, torique)
            st.session_state.iteration += 1
            st.session_state.etats.append(nouvelle.copy())
            st.session_state.grille = nouvelle
            # Vérification de la stabilité si mode_stable
            if mode_stable and detecter_stabilite(st.session_state.etats, nb=3):
                st.info("La grille est stable, arrêt automatique.")
                st.session_state.en_cours = False
            else:
                time.sleep(0.1)
                st.experimental_rerun()
        else:
            st.info("Nombre d'itérations maximal atteint.")
            st.session_state.en_cours = False

    # Lien de téléchargement des états
    if len(st.session_state.etats) > 0:
        st.markdown(telecharger_etats(st.session_state.etats, "historique_etats.txt"), unsafe_allow_html=True)
