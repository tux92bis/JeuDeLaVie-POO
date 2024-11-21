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
