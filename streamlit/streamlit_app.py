import streamlit as st
import requests
import zipfile
import io
import subprocess
import os

# URL pour télécharger l'archive ZIP du dépôt
zip_url = 'https://github.com/tux92bis/JeuDeLaVie-POO-CESI/archive/refs/heads/main.zip'

# Répertoire où le code source sera extrait
extract_dir = 'cloned_repo'

# Télécharger et extraire le code source
st.info('Téléchargement du code source...')
response = requests.get(zip_url)
if response.status_code == 200:
    st.success('Code source téléchargé avec succès.')
    with zipfile.ZipFile(io.BytesIO(response.content)) as zip_ref:
        zip_ref.extractall()
    # Renommer le dossier extrait pour correspondre à 'cloned_repo'
    extracted_folder_name = 'JeuDeLaVie-POO-CESI-main'
    if os.path.exists(extracted_folder_name):
        os.rename(extracted_folder_name, extract_dir)
else:
    st.error('Erreur lors du téléchargement du code source.')
    st.stop()

clone_dir_path = os.path.abspath(extract_dir)

# Fonction pour lister les fichiers et dossiers du dépôt
def list_repo_contents(startpath):
    st.write('Contenu du dépôt :')
    for root, dirs, files in os.walk(startpath):
        # Exclure le dossier .git si présent
        dirs[:] = [d for d in dirs if d != '.git']
        # Calculer le niveau de profondeur pour l'indentation
        level = root.replace(startpath, '').count(os.sep)
        indent = ' ' * 4 * level
        # Afficher le nom du dossier
        st.write(f"{indent}- **{os.path.basename(root)}/**")
        # Afficher les fichiers dans le dossier
        for file in files:
            file_indent = ' ' * 4 * (level + 1)
            st.write(f"{file_indent}- {file}")

# Afficher le contenu du dépôt
list_repo_contents(extract_dir)

# Chemin vers le makefile (notez que le nom est en minuscules 'makefile' ou 'Makefile')
makefile_path = os.path.join(clone_dir_path, 'makefile')
if not os.path.exists(makefile_path):
    makefile_path = os.path.join(clone_dir_path, 'Makefile')

# Vérifier si le makefile existe
if not os.path.exists(makefile_path):
    st.error("Le makefile n'existe pas dans le dépôt.")
    st.write(f"Chemin vérifié : {makefile_path}")
    st.stop()
else:
    st.success("Le makefile a été trouvé.")

# Afficher le contenu du makefile
with open(makefile_path, 'r') as f:
    makefile_content = f.read()
st.write('Contenu du makefile :')
st.code(makefile_content)

# Exécuter la commande make et afficher les messages
st.info('Compilation du code C++ avec le makefile...')
make_result = subprocess.run(['make', '-C', clone_dir_path], capture_output=True, text=True)
st.write('Messages de compilation :')
st.code(make_result.stdout + '\n' + make_result.stderr)

# Vérifier si la compilation a réussi
if make_result.returncode == 0:
    st.success('Compilation réussie.')
else:
    st.error('Erreur lors de la compilation.')
    st.stop()

# Rechercher l'exécutable généré dans le répertoire 'bin/'
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

# Exécuter l'exécutable
st.info(f'Exécution du programme : {executable_path}')
execute_result = subprocess.run([executable_path], capture_output=True, text=True)

# Vérifier si l'exécution a réussi
if execute_result.returncode == 0:
    st.success('Programme exécuté avec succès.')
    st.write('Sortie du programme :')
    st.code(execute_result.stdout)
else:
    st.error("Erreur lors de l'exécution du programme :")
    st.code(execute_result.stderr)
