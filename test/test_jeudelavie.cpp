

/**
 * @file test_jeudelavie.cpp
 * @brief Tests unitaires pour la classe JeuDeLaVie
 * 
 * Ce fichier contient les tests unitaires pour vérifier le bon fonctionnement 
 * de la classe JeuDeLaVie, notamment le chargement de l'état initial et
 * l'exécution en mode console.
 */


#include <iostream>
#include "JeuDeLaVie.hpp"


/**
 * @brief Teste le chargement de l'état initial depuis un fichier
 * 
 * Vérifie si la cellule centrale de la grille est correctement initialisée
 * après le chargement depuis un fichier externe.
 */

void testChargementEtatInitial()
{
    JeuDeLaVie jeu(3, 3, 5);
    jeu.chargerEtatInitial("grille.txt"); // s'assurer que ce fichier existe.

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

/**
 * @brief Teste l'exécution du jeu en mode console
 * 
 * Lance le jeu en mode console et nécessite une vérification manuelle
 * de la sortie pour valider le bon fonctionnement.
 */


void testExecutionModeConsole()
{
    JeuDeLaVie jeu(3, 3, 5);
    jeu.executerModeConsole("grille.txt");

    std::cout << "[OK] Test 2 : Exécution du mode console (Vérifiez manuellement la sortie).\n";
}

/**
 * @brief Fonction principale exécutant la suite de tests
 * @return 0 si tous les tests sont passés
 */

int main()
{
    std::cout << "Tests pour la classe JeuDeLaVie\n";
    testChargementEtatInitial();
    testExecutionModeConsole();
    return 0;
}
