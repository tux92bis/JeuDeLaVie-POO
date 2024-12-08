import streamlit as st
import numpy as np
import time
import base64

st.set_page_config(page_title="Jeu de la Vie", layout="centered")

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
            # Règles de Conway
            if grille[i, j] == 1:
                nouvelle[i, j] = 1 if vivants in [2, 3] else 0
            else:
                nouvelle[i, j] = 1 if vivants == 3 else 0
    return nouvelle

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

def detecter_stabilite(etats, nb=3):
    if len(etats) < nb:
        return False
    last = etats[-1]
    for i in range(2, nb+1):
        if not np.array_equal(last, etats[-i]):
            return False
    return True

def grille_to_emoji(grille):
    # Affiche la grille en utilisant des carrés emojis
    # Vivant = ⬛, Mort = ⬜
    ligne_str = []
    for i in range(grille.shape[0]):
        row = ''.join('⬛' if x == 1 else '⬜' for x in grille[i,:])
        ligne_str.append(row)
    return '\n'.join(ligne_str)

def telecharger_etats(etats, filename="historique_etats.txt"):
    output = []
    for idx, e in enumerate(etats):
        output.append(f"Generation {idx}:")
        l, c = e.shape
        output.append(f"{l} {c}")
        for i in range(l):
            output.append(' '.join(str(val) for val in e[i,:]))
        output.append("")
    txt = "\n".join(output)
    b64 = base64.b64encode(txt.encode()).decode()
    href = f'<a href="data:file/txt;base64,{b64}" download="{filename}">Télécharger l\'historique des états</a>'
    return href

st.title("Jeu de la Vie - Optimisé")
st.write("Configuration, calcul en une fois, puis navigation entre les itérations.")

# Barre latérale
st.sidebar.title("Paramètres")
lignes = st.sidebar.number_input("Lignes", min_value=5, max_value=200, value=20, step=1)
colonnes = st.sidebar.number_input("Colonnes", min_value=5, max_value=200, value=20, step=1)
torique = st.sidebar.checkbox("Bords toriques (wrap-around)", value=False)
mode_stable = st.sidebar.checkbox("Arrêt automatique si stable (3 itérations identiques)", value=False)
iterations_demandees = st.sidebar.number_input("Nombre d'itérations max", min_value=1, value=100, step=10)

fichier_upload = st.sidebar.file_uploader("Fichier initial (0/1)", type=["txt"])

if 'grille_initiale' not in st.session_state:
    st.session_state.grille_initiale = None
if 'etats' not in st.session_state:
    st.session_state.etats = []
if 'calcule' not in st.session_state:
    st.session_state.calcule = False

if st.sidebar.button("Initialiser"):
    if fichier_upload is not None:
        st.session_state.grille_initiale = charger_etat_depuis_fichier(fichier_upload)
    else:
        st.session_state.grille_initiale = generer_grille_vide(lignes, colonnes)
    st.session_state.calcule = False
    st.session_state.etats = [st.session_state.grille_initiale.copy()]

if st.sidebar.button("Calculer"):
    # Calcul de toutes les générations en une fois
    if st.session_state.grille_initiale is not None:
        etats = [st.session_state.grille_initiale.copy()]
        for i in range(iterations_demandees):
            nouvelle = prochaine_generation(etats[-1], torique)
            etats.append(nouvelle)
            if mode_stable and detecter_stabilite(etats, nb=3):
                st.info(f"Arrêt car la grille est stable à l'itération {i+1}.")
                break
        st.session_state.etats = etats
        st.session_state.calcule = True
    else:
        st.warning("Veuillez initialiser d'abord.")

if st.session_state.calcule and len(st.session_state.etats) > 0:
    iteration_max = len(st.session_state.etats)-1
    iteration_select = st.slider("Itération à afficher", 0, iteration_max, 0)
    st.write(f"Itération sélectionnée : {iteration_select}")
    grille = st.session_state.etats[iteration_select]
    st.text(grille_to_emoji(grille))
    if len(st.session_state.etats) > 0:
        st.markdown(telecharger_etats(st.session_state.etats, "historique_etats.txt"), unsafe_allow_html=True)
else:
    st.info("Initialisez puis calculez. Ensuite, utilisez le slider pour naviguer parmi les itérations.")

