import streamlit as st
import numpy as np
import time
from PIL import Image, ImageDraw
import base64

st.set_page_config(page_title="Jeu de la Vie", layout="wide")

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
        row = [str(val) for val in grille[i, :]]
        lines.append(' '.join(row))
    return "\n".join(lines)

def charger_etat_depuis_fichier(fichier) -> np.ndarray:
    content = fichier.read().decode('utf-8')
    lignes_f = content.strip().split('\n')
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

st.sidebar.title("Paramètres")
lignes = st.sidebar.number_input("Lignes", min_value=5, max_value=40, value=20, step=1)
colonnes = st.sidebar.number_input("Colonnes", min_value=5, max_value=40, value=20, step=1)
torique = st.sidebar.checkbox("Bords toriques (wrap-around)", value=False)
mode_stable = st.sidebar.checkbox("Arrêter automatiquement quand stable (3 itérations identiques)", value=False)

iterations_demandees = st.sidebar.number_input("Nombre d'itérations max", min_value=1, value=100, step=10)

fichier_upload = st.sidebar.file_uploader("Charger un fichier texte (0/1)", type=["txt"])

col1, col2, col3, col4 = st.sidebar.columns(4)
if col1.button("Initialiser"):
    if fichier_upload is not None:
        st.session_state.grille = charger_etat_depuis_fichier(fichier_upload)
        # Ajuste le nombre de lignes/colonnes aux données chargées
        l, c = st.session_state.grille.shape
        lignes = l
        colonnes = c
    else:
        st.session_state.grille = generer_grille_vide(lignes, colonnes)
    st.session_state.iteration = 0
    st.session_state.en_cours = False
    st.session_state.etats = [st.session_state.grille.copy()]

if col2.button("Lancer"):
    if st.session_state.grille is not None:
        st.session_state.en_cours = True

if col3.button("Pause"):
    st.session_state.en_cours = False

if col4.button("Etape +1"):
    if st.session_state.grille is not None:
        st.session_state.grille = prochaine_generation(st.session_state.grille, torique)
        st.session_state.iteration += 1
        st.session_state.etats.append(st.session_state.grille.copy())

st.title("Jeu de la Vie (0 = mort, 1 = vivant)")
st.write("1. Initialisez la grille. Si aucun fichier n'est chargé, une grille vide s'affiche.")
st.write("2. Cliquez sur les cellules pour toggler leur état (0/1) AVANT de lancer.")
st.write("3. Lancez le jeu, mettez en pause, ou avancez étape par étape.")

if st.session_state.grille is None:
    st.info("Veuillez initialiser une grille.")
else:
    st.write(f"Itération actuelle : {st.session_state.iteration}")

    # Affichage de la grille en mode "tableau de boutons"
    # Chaque bouton correspond à une cellule. En cliquant dessus, on toggle son état.
    # On fait attention à la performance : pas trop grand.
    grille = st.session_state.grille
    # Construction d'une grille de widgets
    for i in range(lignes):
        cols = st.columns(colonnes)
        for j in range(colonnes):
            cell_label = "1" if grille[i, j] == 1 else "0"
            # Chaque bouton doit avoir une clé unique
            if cols[j].button(cell_label, key=f"cell_{i}_{j}"):
                # Toggle la cellule
                st.session_state.grille[i, j] = 1 - st.session_state.grille[i, j]
                st.experimental_rerun()

    # Si en cours, on avance
    if st.session_state.en_cours and st.session_state.grille is not None:
        if st.session_state.iteration < iterations_demandees:
            nouvelle = prochaine_generation(st.session_state.grille, torique)
            st.session_state.iteration += 1
            st.session_state.etats.append(nouvelle.copy())
            st.session_state.grille = nouvelle

            if mode_stable and detecter_stabilite(st.session_state.etats, nb=3):
                st.info("La grille est stable, arrêt automatique.")
                st.session_state.en_cours = False
            else:
                time.sleep(0.1)
                st.experimental_rerun()
        else:
            st.info("Nombre d'itérations maximal atteint.")
            st.session_state.en_cours = False

    if len(st.session_state.etats) > 0:
        st.markdown(telecharger_etats(st.session_state.etats, "historique_etats.txt"), unsafe_allow_html=True)
