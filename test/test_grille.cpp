#include <iostream>
#include "Grille.hpp"

void testGrille() {
    Grille grille(3, 3);
    std::vector<std::vector<bool>> etatInitial = {
        {0, 1, 0},
        {1, 1, 1},
        {0, 1, 0}
    };

    // Test d'initialisation
    grille.initialiser(etatInitial);
    auto cellules = grille.obtenirCellules();
    if (cellules[1][1].estVivante() && !cellules[0][0].estVivante()) {
        std::cout << "Test 1: Initialisation de la grille - Réussi\n";
    } else {
        std::cout << "Test 1: Initialisation de la grille - Échoué\n";
    }

    // Test de mise à jour
    grille.mettreAJour();
    cellules = grille.obtenirCellules();
    if (!cellules[0][1].estVivante() && cellules[1][1].estVivante()) {
        std::cout << "Test 2: Mise à jour de la grille - Réussi\n";
    } else {
        std::cout << "Test 2: Mise à jour de la grille - Échoué\n";
    }
}
