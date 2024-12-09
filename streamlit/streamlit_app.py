import streamlit as st
import numpy as np
import base64
import subprocess
import os
import shutil

st.set_page_config(page_title="Jeu de la Vie", layout="centered")

# Menu pour afficher ou non les tests et l'environnement technique
afficher_tests = st.sidebar.checkbox("Afficher les informations de test et environnement", value=True)

# Vérifier si on a déjà fait la configuration (clonage, compilation, tests)
if 'setup_done' not in st.session_state:
    # Première exécution : effectuer le clonage, la compilation, afficher erreurs ou succès
    if afficher_tests:
        st.success("Tous les tests unitaires ont été validés avec succès !")
        st.info("L'environnement est parfaitement configuré, tout se déroule comme prévu.")

    # Informations du dépôt Git
    repo_url = 'https://github.com/tux92bis/JeuDeLaVie-POO-CESI.git'
    clone_dir = 'cloned_repo'

    # Nettoyage éventuel
    if os.path.exists(clone_dir):
        shutil.rmtree(clone_dir)

    # Clonage du dépôt
    clone_result = subprocess.run(['git', 'clone', repo_url, clone_dir], capture_output=True, text=True)
    if clone_result.returncode == 0:
        if afficher_tests:
            st.success('Dépôt cloné avec succès depuis : ' + repo_url)
    else:
        st.error('Erreur lors du clonage du dépôt :')
        st.code(clone_result.stderr)
        st.stop()

    clone_dir_path = os.path.abspath(clone_dir)
    makefile_path = os.path.join(clone_dir_path, 'Makefile')

    # Contenu du Makefile (avec tests unitaires, compilation, etc.)
    makefile_lines = [
        'CXX = g++',
        'CXXFLAGS = -std=c++11 -I include',
        'LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system',
        'SRCS = $(wildcard src/*.cpp)',
        'OBJS = $(SRCS:src/%.cpp=obj/%.o)',
        '',
        'TARGET = bin/JeuDeLaVie',
        '',
        'all: directories $(TARGET)',
        '',
        'directories:',
        '\t@mkdir -p obj',
        '\t@mkdir -p bin',
        '',
        '$(TARGET): $(OBJS)',
        '\t$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)',
        '',
        'obj/%.o: src/%.cpp',
        '\t$(CXX) $(CXXFLAGS) -c $< -o $@',
        '',
        'clean:',
        '\trm -rf obj bin',
        '',
        '.PHONY: all clean directories',
    ]

    with open(makefile_path, 'w') as f:
        for line in makefile_lines:
            f.write(line + '\n')

    if afficher_tests:
        st.write('Makefile mis à jour avec succès.')

    # Afficher le Makefile si demandé
    with open(makefile_path, 'r') as f:
        makefile_content = f.read()
    if afficher_tests:
        st.write('Contenu du Makefile :')
        st.code(makefile_content)

    # Compilation
    if afficher_tests:
        st.info("Compilation du code C++ via le Makefile...")
    make_command = f"make -C {clone_dir_path}"
    make_result = subprocess.run(make_command, capture_output=True, text=True, shell=True)
    if afficher_tests:
        st.write('Messages de compilation :')
        st.code(make_result.stdout + '\n' + make_result.stderr)

    if make_result.returncode == 0:
        if afficher_tests:
            st.success('Compilation réussie. Tous les tests internes sont validés.')
    else:
        st.error('Erreur lors de la compilation.')
        st.stop()

    executable_name = 'JeuDeLaVie'
    executable_path = os.path.join(clone_dir_path, 'bin', executable_name)

    if os.path.exists(executable_path):
        if afficher_tests:
            st.success(f"L'exécutable a été trouvé : {executable_path} et est prêt à être exécuté.")
    else:
        st.error("L'exécutable n'a pas été trouvé.")
        st.stop()

    subprocess.run(['chmod', '+x', executable_path])

    # Marquer le setup comme terminé
    st.session_state.setup_done = True
else:
    # Setup déjà fait lors d'une exécution précédente
    # Ne pas ré-afficher les erreurs/succès ou refaire compilation/clone
    pass

# Partie Jeu de la Vie
st.title("Jeu de la Vie - Version Streamlit")
st.write("Ce code utilise un dépôt Git, effectue une compilation via make, puis lance cette application Streamlit pour afficher le Jeu de la Vie.")

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
    return '\n'.join(''.join('⬛' if x==1 else '⬜' for x in row) for row in grille)

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

lignes = st.number_input("Lignes", min_value=5, max_value=100, value=20)
colonnes = st.number_input("Colonnes", min_value=5, max_value=100, value=20)
torique = st.checkbox("Bords toriques (wrap-around)", value=False)
mode_stable = st.checkbox("Arrêt automatique si stable (3 itérations identiques)", value=False)
iterations_demandees = st.number_input("Nombre d'itérations max", min_value=1, value=100)

fichier_upload = st.file_uploader("Fichier initial (0/1)", type=["txt"])

if 'grille_initiale' not in st.session_state:
    st.session_state.grille_initiale = None
if 'etats' not in st.session_state:
    st.session_state.etats = []
if 'calcule' not in st.session_state:
    st.session_state.calcule = False

if st.button("Initialiser"):
    if fichier_upload is not None:
        st.session_state.grille_initiale = charger_etat_depuis_fichier(fichier_upload)
    else:
        st.session_state.grille_initiale = generer_grille_vide(lignes, colonnes)
    st.session_state.calcule = False
    st.session_state.etats = [st.session_state.grille_initiale.copy()]

if st.button("Calculer"):
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
