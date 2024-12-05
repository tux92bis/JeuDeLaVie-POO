import streamlit as st
import subprocess
import os
import shutil

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

# Écrire le nouveau Makefile avec des tabulations correctes
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
make_result = subprocess.run(['make', '-C', clone_dir_path], capture_output=True, text=True)
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

# Simulation de terminal
st.title("Simulation de terminal pour le programme C++")

# Initialiser l'historique des commandes
if 'history' not in st.session_state:
    st.session_state.history = []

# Champ de saisie pour la commande
command = st.text_input("Entrez une commande pour le programme :", key='command_input')

# Lorsque l'utilisateur appuie sur Entrée
if command:
    # Ajouter la commande à l'historique
    st.session_state.history.append(f"> {command}")

    # **IMPORTANT** : Sécurité
    # Ne pas exécuter de commandes système arbitraires
    # Ici, nous exécutons uniquement l'exécutable avec les arguments fournis
    try:
        # Construire la liste des arguments
        args = command.strip().split()
        cmd = [executable_path] + args

        # Exécuter l'exécutable avec les arguments
        result = subprocess.run(cmd, capture_output=True, text=True)

        # Ajouter la sortie à l'historique
        output = result.stdout + result.stderr
        st.session_state.history.append(output)
    except Exception as e:
        st.session_state.history.append(f"Erreur lors de l'exécution : {e}")

    # Effacer le champ de saisie
    st.session_state.command_input = ''

# Afficher l'historique des commandes
st.text_area("Terminal", value="\n".join(st.session_state.history), height=300)
