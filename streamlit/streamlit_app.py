import streamlit as st
import subprocess
import os

# URL du dépôt GitHub
repo_url = 'https://github.com/tux92bis/JeuDeLaVie-POO-CESI.git'

# Répertoire où le dépôt sera cloné
clone_dir = 'cloned_repo'

# Chemin absolu du répertoire cloné
clone_dir_path = os.path.abspath(clone_dir)

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

# Chemin vers le makefile (notez que le nom est en minuscules 'makefile')
makefile_path = os.path.join(clone_dir_path, 'makefile')

# Vérifier si le makefile existe
if not os.path.exists(makefile_path):
    st.error("Le makefile n'existe pas dans le dépôt.")
    st.write(f"Chemin vérifié : {makefile_path}")
    st.stop()
else:
    st.success("Le makefile a été trouvé.")

# Exécuter la commande make
st.info('Compilation du code C++ avec le makefile...')
make_result = subprocess.run(['make', '-C', clone_dir_path], capture_output=True, text=True)

# Vérifier si la compilation a réussi
if make_result.returncode == 0:
    st.success('Compilation réussie.')
else:
    st.error('Erreur lors de la compilation :')
    st.code(make_result.stdout + '\n' + make_result.stderr)
    st.stop()

# Chemin vers l'exécutable (vérifiez le nom de l'exécutable généré par le makefile)
executable_name = 'jeu_de_la_vie'  # Assurez-vous que c'est le bon nom
executable_path = os.path.join(clone_dir_path, executable_name)

# Vérifier si l'exécutable existe
if not os.path.exists(executable_path):
    st.error("L'exécutable n'a pas été créé.")
    st.write(f"Chemin vérifié : {executable_path}")
    st.stop()
else:
    st.success("L'exécutable a été créé avec succès.")

# Rendre l'exécutable exécutable
subprocess.run(['chmod', '+x', executable_path])

# Exécuter l'exécutable
st.info('Exécution du programme...')
execute_result = subprocess.run([executable_path], capture_output=True, text=True)

# Vérifier si l'exécution a réussi
if execute_result.returncode == 0:
    st.success('Programme exécuté avec succès.')
    st.write('Sortie du programme :')
    st.code(execute_result.stdout)
else:
    st.error("Erreur lors de l'exécution du programme :")
    st.code(execute_result.stderr)
