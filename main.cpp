#include "JeuDeLaVie.hpp"
#include "ModeConsole.hpp"
#include "InterfaceGraphique.hpp"
#include <iostream>
#include <string>
#include "Mode.hpp"

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

    mode* mode = nullptr; // Pointeur polymorphique
    if (choixMode == 1) {
        mode = new ModeConsole(jeu, cheminFichier);
    } else if (choixMode == 2) {
        mode = new InterfaceGraphique(jeu, 20); // Taille des cellules
    } else {
        cerr << "Choix invalide !" << endl;
        return EXIT_FAILURE;
    }

    mode->executer(iterations);

    delete mode; // Libérer la mémoire
    return 0;
}