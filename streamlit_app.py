import streamlit as st
st.title("salut les loulou")
st.code("//ceci est du c++")

import subprocess
import os

# Chemin vers le fichier C++ que vous souhaitez compiler
cpp_file_path = '/src/main.cpp'

# Chemin où vous voulez placer l'exécutable
output_directory = '/'

# Nom de l'exécutable
executable_name = 'jeux de la vie'

# Commande pour compiler le fichier C++
compile_command = ['g++', cpp_file_path, '-o', os.path.join(output_directory, executable_name)]

# Exécution de la commande de compilation
compilation = subprocess.run(compile_command, capture_output=True, text=True)

# Vérification du succès de la compilation
if compilation.returncode == 0:
    st.print('Compilation réussie.')

    # Commande pour exécuter le programme compilé
    execution = subprocess.run([os.path.join(output_directory, executable_name)], capture_output=True, text=True)

    # Affichage de la sortie du programme
    st.text('Sortie du programme :')
    st.print(execution.stdout)
else:
    st.text('Erreur lors de la compilation :')
    st.print(compilation.stderr)

