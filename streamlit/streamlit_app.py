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
else:
    st.info('Le dépôt GitHub a déjà été cloné.')

# Fonction pour lister les fichiers et dossiers du dépôt
def list_repo_contents(startpath):
    st.write('Contenu du dépôt cloné :')
    for root, dirs, files in os.walk(startpath):
        # Exclure le dossier .git si vous ne souhaitez pas l'afficher
        dirs[:] = [d for d in dirs if d != '.git']
        # Calculer le niveau de profondeur pour l'indentation
        level = root.replace(startpath, '').count(os.sep)
        indent = ' ' * 4 * level
        # Afficher le nom du dossier
        st.write(f"{indent}- **{os.path.basename(root)}/**")
        # Afficher les fichiers dans le dossier
        for file in files:
            filepath = os.path.join(root, file)
            file_indent = ' ' * 4 * (level + 1)
            st.write(f"{file_indent}- {file}")

# Appeler la fonction pour afficher le contenu du dépôt
list_repo_contents(clone_dir)
