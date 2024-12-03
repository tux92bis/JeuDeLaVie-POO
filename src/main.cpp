/**
 * @file main.cpp
 * @brief Point d'entrée principal du Jeu de la Vie
 * 
 * Ce programme implémente le Jeu de la Vie de Conway avec deux modes d'affichage possibles :
 * console ou graphique. Il permet à l'utilisateur de :
 * - Spécifier le nombre d'itérations à exécuter
 * - Charger un fichier contenant les dimensions et l'état initial de la grille
 * - Choisir entre le mode console et le mode graphique
 * 
 * Le programme utilise un design pattern Strategy pour gérer les différents modes d'affichage
 * à travers une interface commune (classe Mode).
 * 
 * @note La libération de la mémoire est gérée automatiquement à la fin de l'exécution
 * 
 * @return 0 en cas de succès, EXIT_FAILURE en cas d'erreur
 */


#include "JeuDeLaVie.hpp"
#include "ModeConsole.hpp"
#include "InterfaceGraphique.hpp"
#include <iostream>
#include <string>
#include "Mode.hpp"

using namespace std;

int main()
{
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

    mode *mode = nullptr; // Pointeur polymorphique
    if (choixMode == 1)
    {
        mode = new ModeConsole(jeu, cheminFichier); // Allocation dynamique
    }
    else if (choixMode == 2)
    {
        mode = new InterfaceGraphique(jeu, 20); // Taille des cellules
    }
    else
    {
        cerr << "Choix invalide !" << endl; // Affichage d'une erreur en cas de choix invalide
        return EXIT_FAILURE;
    }

    mode->executer(iterations); 

    delete mode; // Libérer la mémoire
    return 0;
}