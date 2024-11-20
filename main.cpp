#include "JeuDeLaVie.hpp"
#include <iostream>
#include <string>
#include "ModeConsole.hpp"

using namespace std;

int main() {
    int iterations;
    string cheminFichier;
    cout << "Entrez le nombre d'itérations : ";
    cin >> iterations;
    cout << "Entrez le chemin du fichier contenant les dimensions et l'état initial : ";
    cin >> cheminFichier;
    JeuDeLaVie jeu(0, 0, iterations); 
    jeu.chargerEtatInitial(cheminFichier);
    int choixMode;
    cout << "Choisissez un mode d'exécution :\n1. Mode Console\n2. Mode Graphique\nVotre choix : ";
    cin >> choixMode;
    if (choixMode == 1) {
        ModeConsole modeConsole(jeu, cheminFichier);
        modeConsole.executer(iterations);
    } else if (choixMode == 2) {
        jeu.executerModeGraphique(20); 
    } else {
        cerr << "Choix invalide !" << endl;
        return EXIT_FAILURE;
    }

    return 0;
}
