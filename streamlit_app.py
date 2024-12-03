import streamlit as st
import subprocess
import os

# URL du dépôt GitHub
repo_url = 'https://github.com/tux92bis/JeuDeLaVie-POO-CESI.git'

# Répertoire où le dépôt sera cloné
clone_dir = 'cloned_repo'

# Cloner le dépôt si ce n'est pas déjà fait
if not os.path.exists(clone_dir):
    st.info('Clonage du dépôt GitHub...')
    result = subprocess.run(['git', 'clone', repo_url, clone_dir], capture_output=True, text=True)
    if result.returncode == 0:
        st.success('Dépôt cloné avec succès.')
    else:
        st.error('Erreur lors du clonage du dépôt :')
        st.code(result.stderr)
        st.stop()

# Fonction pour lister les fichiers du dépôt
def list_repo_files(startpath):
    files_list = []
    for root, dirs, files in os.walk(startpath):
        for file in files:
            # Obtenir le chemin relatif pour un affichage propre
            relative_path = os.path.relpath(os.path.join(root, file), startpath)
            files_list.append(relative_path)
    return files_list

# Afficher la liste des fichiers
st.title('Contenu du dépôt GitHub')
files = list_repo_files(clone_dir)
for file in sorted(files):
    st.write(f'- {file}')

# Optionnel : Afficher le contenu des fichiers
st.header('Afficher le contenu des fichiers')
selected_file = st.selectbox('Sélectionnez un fichier pour voir son contenu', files)
file_path = os.path.join(clone_dir, selected_file)
try:
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()
    st.code(content, language='python' if selected_file.endswith('.py') else '')
except Exception as e:
    st.error(f'Impossible de lire le fichier : {e}')
