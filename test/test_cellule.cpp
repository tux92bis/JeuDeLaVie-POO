#include <iostream>
#include "Cellule.hpp"

void testCellule() {
    Cellule c;

    // Test de l'état initial
    if (!c.estVivante()) {
        std::cout << "Test 1: Cellule morte par défaut - Réussi\n";
    } else {
        std::cout << "Test 1: Cellule morte par défaut - Échoué\n";
    }

    // Test de la modification de l'état
    c.estVivante(true);
    if (c.estVivante()) {
        std::cout << "Test 2: Cellule vivante après setVivante(true) - Réussi\n";
    } else {
        std::cout << "Test 2: Cellule vivante après setVivante(true) - Échoué\n";
    }
}
