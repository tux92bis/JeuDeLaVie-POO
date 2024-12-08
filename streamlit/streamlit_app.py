import streamlit as st
import subprocess
import os
import shutil
import base64
import numpy as np
import pexpect

# Définir la configuration de la page avant toute autre commande Streamlit
st.set_page_config(page_title="Jeu de la Vie", layout="centered")

# Le reste de votre code...
#---------------------------------------
# Partie Compilation/Clonage du Dépôt + pexpect
#---------------------------------------

# URL du dépôt GitHub
repo_url = 'https://github.com/tux92bis/JeuDeLaVie-POO-CESI.git'

# Répertoire où le dépôt sera cloné
clone_dir = 'cloned_repo'

# Supprimer le répertoire cloné s'il existe déjà
if os.path.exists(clone_dir):
    shutil.rmtree(clone_dir)

# Cloner le dépôt GitHub
st.info('Clonage du dépôt GitHub...')
clone_result = subprocess.run(['git', 'clone', repo_url, clone_dir], capture_output=True, text=True)
if clone_result.returncode == 0:
    st.success('Dépôt cloné avec succès.')
else:
    st.error('Erreur lors du clonage du dépôt :')
    st.code(clone_result.stderr)
    st.stop()

clone_dir_path = os.path.abspath(clone_dir)

# Modifier le Makefile localement
makefile_path = os.path.join(clone_dir_path, 'Makefile')

# Nouveau contenu du Makefile avec des tabulations correctes
makefile_lines = [
    'CXX = g++',
    'CXXFLAGS = -std=c++11 -I include',
    'LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system',
    'SRCS = $(wildcard src/*.cpp)',
    'OBJS = $(SRCS:src/%.cpp=obj/%.o)',
    '',
    'TARGET = bin/JeuDeLaVie',
    '',
    '# Règle par défaut',
    'all: directories $(TARGET)',
    '',
    '# Règle pour créer les répertoires nécessaires',
    'directories:',
    '\t@mkdir -p obj',
    '\t@mkdir -p bin',
    '',
    '# Règle pour créer l\'exécutable',
    '$(TARGET): $(OBJS)',
    '\t$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)',
    '',
    '# Règle pour compiler les fichiers sources en objets',
    'obj/%.o: src/%.cpp',
    '\t$(CXX) $(CXXFLAGS) -c $< -o $@',
    '',
    '# Règle pour nettoyer les fichiers générés',
    'clean:',
    '\trm -rf obj bin',
    '',
    '.PHONY: all clean directories',
]

# Écrire le nouveau Makefile
with open(makefile_path, 'w') as f:
    for line in makefile_lines:
        f.write(line + '\n')

st.write('Makefile mis à jour.')

# Afficher le contenu du Makefile
with open(makefile_path, 'r') as f:
    makefile_content = f.read()
st.write('Contenu du Makefile :')
st.code(makefile_content)

# Exécuter la commande make et afficher les messages
st.info('Compilation du code C++ avec le Makefile...')
make_command = f"make -C {clone_dir_path}"
make_result = subprocess.run(make_command, capture_output=True, text=True, shell=True)
st.write('Messages de compilation :')
st.code(make_result.stdout + '\n' + make_result.stderr)

# Vérifier si la compilation a réussi
if make_result.returncode == 0:
    st.success('Compilation réussie.')
else:
    st.error('Erreur lors de la compilation.')
    st.stop()

# Chemin vers l'exécutable
executable_name = 'JeuDeLaVie'  # Nom de l'exécutable attendu
executable_path = os.path.join(clone_dir_path, 'bin', executable_name)

# Vérifier si l'exécutable existe
if os.path.exists(executable_path):
    st.success(f"L'exécutable a été trouvé : {executable_path}")
else:
    st.error("L'exécutable n'a pas été trouvé.")
    st.write(f"Chemin vérifié : {executable_path}")
    st.stop()

# Rendre l'exécutable exécutable (au cas où)
subprocess.run(['chmod', '+x', executable_path])


#---------------------------------------
# Partie Jeu de la Vie (Python)
#---------------------------------------
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

st.title("Jeu de la Vie - Version Streamlit avec Tests et Makefile (Simulation)")

st.write("Ce code simule un dépôt Git, une compilation via make, des tests unitaires, puis exécute cette app Streamlit et offre une interaction avec le programme compilé via pexpect.")

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


#---------------------------------------
# Partie Terminal interactif avec pexpect (mix)
#---------------------------------------

st.header("Terminal Interactif avec pexpect")

# Initialiser l'historique du terminal interactif
if 'history_interact' not in st.session_state:
    st.session_state.history_interact = ""

# Démarrer le processus pexpect s'il n'est pas déjà démarré
if 'child' not in st.session_state:
    # Lancer le programme compilé via pexpect
    # Assurez-vous que votre programme C++ est interactif
    # Ici, on suppose que votre programme C++ prend des entrées ligne par ligne
    st.session_state.child = pexpect.spawn(executable_path, encoding='utf-8', timeout=1)
    # Essayer de lire une sortie initiale
    try:
        initial_output = st.session_state.child.read_nonblocking(size=1024, timeout=0.5)
        st.session_state.history_interact += initial_output
    except pexpect.exceptions.TIMEOUT:
        pass
    except pexpect.exceptions.EOF:
        st.session_state.history_interact += "\n[Le programme s'est terminé immédiatement]\n"

# Afficher l'historique du terminal interactif
st.text_area("Terminal Interactif (C++) :", st.session_state.history_interact, height=300)

user_input_interact = st.text_input("Entrez une commande (pour le programme C++) :")

if st.button("Envoyer au programme C++"):
    if user_input_interact and 'child' in st.session_state:
        st.session_state.child.sendline(user_input_interact)
        st.session_state.history_interact += f"> {user_input_interact}\n"
        # Essayer de lire une réponse
        try:
            # On attend une sortie. On tente de lire ce que le process renvoie
            # On essaie un petit loop pour récupérer plus de données
            st.session_state.child.expect('.+', timeout=0.5)
            output = st.session_state.child.after
            st.session_state.history_interact += output
        except pexpect.exceptions.TIMEOUT:
            st.session_state.history_interact += "[Aucune sortie reçue]\n"
        except pexpect.exceptions.EOF:
            st.session_state.history_interact += "\n[Le programme s'est terminé]\n"
            del st.session_state.child

        # Re-rendre la page après l'envoi
        st.experimental_rerun()
