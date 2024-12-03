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

# Vérifier si le Makefile existe
makefile_path = os.path.join(clone_dir, '.git/HEAD/makefile')
if not os.path.exists(makefile_path):
    st.error('Le Makefile n\'existe pas dans le dépôt.')
    st.stop()

# Exécuter la commande make
st.info('Compilation du code C++ avec le Makefile...')
make_result = subprocess.run(['make', '-C', clone_dir], capture_output=True, text=True)

# Vérifier si la compilation a réussi
if make_result.returncode == 0:
    st.success('Compilation réussie.')
else:
    st.error('Erreur lors de la compilation :')
    st.code(make_result.stderr)
    st.stop()

# Chemin vers l'exécutable (vérifiez le nom de l'exécutable généré par le Makefile)
executable_path = os.path.join(clone_dir, 'bin', 'jeu_de_la_vie')

# Vérifier si l'exécutable existe
if not os.path.exists(executable_path):
    st.error("L'exécutable n'a pas été créé.")
    st.stop()

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
