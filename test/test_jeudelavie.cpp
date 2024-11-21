#include <iostream>
#include "JeuDeLaVie.hpp"

void testChargementEtatInitial()
{
    JeuDeLaVie jeu(3, 3, 5);
    jeu.chargerEtatInitial("grille.txt"); // Assurez-vous que ce fichier existe.

    const auto &grille = jeu.getGrille();
    const auto &cellules = grille.obtenirCellules();

    if (cellules[1][1].estVivante())
    {
        std::cout << "[OK] Test 1 : Chargement de l'état initial depuis un fichier.\n";
    }
    else
    {
        std::cout << "[FAIL] Test 1 : Chargement de l'état initial depuis un fichier.\n";
    }
}

void testExecutionModeConsole()
{
    JeuDeLaVie jeu(3, 3, 5);
    jeu.executerModeConsole("grille.txt");

    std::cout << "[OK] Test 2 : Exécution du mode console (Vérifiez manuellement la sortie).\n";
}

int main()
{
    std::cout << "Tests pour la classe JeuDeLaVie\n";
    testChargementEtatInitial();
    testExecutionModeConsole();
    return 0;
}
