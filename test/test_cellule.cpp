#include <iostream>
#include "Cellule.hpp"

void testEtatInitial()
{
    Cellule celluleMorte(false);
    Cellule celluleVivante(true);

    if (!celluleMorte.estVivante())
    {
        std::cout << "[OK] Test 1 : Cellule morte par défaut.\n";
    }
    else
    {
        std::cout << "[FAIL] Test 1 : Cellule morte par défaut.\n";
    }

    if (celluleVivante.estVivante())
    {
        std::cout << "[OK] Test 2 : Cellule vivante par défaut.\n";
    }
    else
    {
        std::cout << "[FAIL] Test 2 : Cellule vivante par défaut.\n";
    }
}

void testModificationEtat()
{
    Cellule cellule(false);

    cellule.definirEtat(true);
    if (cellule.estVivante())
    {
        std::cout << "[OK] Test 3 : Cellule devient vivante.\n";
    }
    else
    {
        std::cout << "[FAIL] Test 3 : Cellule devient vivante.\n";
    }

    cellule.definirEtat(false);
    if (!cellule.estVivante())
    {
        std::cout << "[OK] Test 4 : Cellule devient morte.\n";
    }
    else
    {
        std::cout << "[FAIL] Test 4 : Cellule devient morte.\n";
    }
}

int main()
{
    std::cout << "Tests pour la classe Cellule\n";
    testEtatInitial();
    testModificationEtat();
    return 0;
}
