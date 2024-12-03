


/**
 * @file test_grille.cpp
 * @brief Fichier de tests unitaires pour la classe Grille
 * 
 * Ce fichier contient les tests pour vérifier le bon fonctionnement 
 * de la classe Grille qui implémente le Jeu de la Vie.
 *
 * @test testInitialisation()
 * Teste l'initialisation correcte d'une grille 3x3 avec un motif spécifique.
 * Vérifie que les cellules sont correctement positionnées selon l'état initial.

 * 
 * Le motif de test utilisé est :
 *   0 1 0
 *   1 1 1
 *   0 1 0
 */
#include <iostream>
#include "Grille.hpp"

void testInitialisation()
{
    Grille grille(3, 3);
    std::vector<std::vector<bool>> etatInitial = {
        {0, 1, 0},
        {1, 1, 1},
        {0, 1, 0}};

    grille.initialiser(etatInitial);

    const auto &cellules = grille.obtenirCellules();
    if (cellules[1][1].estVivante() && !cellules[0][0].estVivante())
    {
        std::cout << "[OK] Test 1 : Initialisation de la grille.\n";
    }
    else
    {
        std::cout << "[FAIL] Test 1 : Initialisation de la grille.\n";
    }
}
/**
 * @brief test testMiseAJour() 
 * Teste la mise à jour des cellules selon les règles du Jeu de la Vie.
 * Vérifie que l'état de la grille évolue correctement après une itération.
*/

void testMiseAJour()
{
    Grille grille(3, 3);
    std::vector<std::vector<bool>> etatInitial = {
        {0, 1, 0},
        {1, 1, 1},
        {0, 1, 0}};

    grille.initialiser(etatInitial);
    grille.mettreAJour();

    const auto &cellules = grille.obtenirCellules();
    if (!cellules[0][1].estVivante() && cellules[1][1].estVivante())
    {
        std::cout << "[OK] Test 2 : Mise à jour de la grille.\n";
    }
    else
    {
        std::cout << "[FAIL] Test 2 : Mise à jour de la grille.\n";
    }
}

int main()
{
    std::cout << "Tests pour la classe Grille\n";
    testInitialisation();
    testMiseAJour();
    return 0;
}
