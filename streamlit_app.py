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
# Chemin vers le fichier .git/HEAD
head_file = os.path.join(clone_dir, '.git', 'HEAD')

# Vérifier si le fichier HEAD existe
if os.path.exists(head_file):
    with open(head_file, 'r') as f:
        head_ref = f.read().strip()
    st.write(f'Référence du HEAD : {head_ref}')
else:
    st.error('Le fichier .git/HEAD n\'existe pas.')
if head_ref.startswith('ref:'):
    branch_name = head_ref.partition('refs/heads/')[2]
    st.write(f'Branche courante : {branch_name}')
else:
    st.write(f'HEAD détaché sur le commit : {head_ref}')
# Définir les variables pour Git
git_dir = os.path.join(clone_dir, '.git')
work_tree = clone_dir

# Obtenir le hash du dernier commit
result = subprocess.run(['git', '--git-dir', git_dir, '--work-tree', work_tree, 'rev-parse', 'HEAD'], capture_output=True, text=True)

if result.returncode == 0:
    commit_hash = result.stdout.strip()
    st.write(f'Hash du dernier commit : {commit_hash}')
else:
    st.error('Erreur lors de la récupération du hash du dernier commit :')
    st.code(result.stderr)
# Chemin vers le fichier main.cpp
cpp_file_path = os.path.join(clone_dir, 'src', 'main.cpp')

# Nom de l'exécutable
executable_name = 'main_executable'

# Commande pour compiler le fichier C++
compile_command = ['g++', '-std=c++11', cpp_file_path, '-o', executable_name]

st.info('Compilation du code C++...')
compile_result = subprocess.run(compile_command, capture_output=True, text=True)

# Vérifier si la compilation a réussi
if compile_result.returncode == 0:
    st.success('Compilation réussie.')
else:
    st.error('Erreur lors de la compilation :')
    st.code(compile_result.stderr)
    st.stop()
# Vérifier si l'exécutable existe
if not os.path.exists(executable_name):
    st.error("L'exécutable n'a pas été créé.")
    st.stop()

# Commande pour exécuter l'exécutable
execute_command = ['./' + executable_name]

st.info('Exécution du programme...')
execute_result = subprocess.run(execute_command, capture_output=True, text=True)

# Vérifier si l'exécution a réussi
if execute_result.returncode == 0:
    st.success('Programme exécuté avec succès.')
    st.write('Sortie du programme :')
    st.code(execute_result.stdout)
else:
    st.error("Erreur lors de l'exécution du programme :")
    st.code(execute_result.stderr)
