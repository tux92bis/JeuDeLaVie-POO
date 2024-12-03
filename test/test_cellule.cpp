

/**
 * @file test_cellule.cpp
 * @brief Fichier de tests pour la classe Cellule
 * @details Ce fichier contient les tests unitaires pour vérifier le bon fonctionnement de la classe Cellule
 *
 * Les tests vérifient:
 * - L'initialisation correcte de l'état des cellules (vivante/morte)
 * - La modification de l'état d'une cellule
 */
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


/**
 * @fn void testModificationEtat() 
 * @brief Test la modification de l'état d'une cellule
 * @details Vérifie qu'une cellule peut changer d'état correctement (de morte à vivante et vice versa)
 *
*/
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


/** 

 * @fn int main()
 * @brief Point d'entrée du programme de test
 * @return 0 si tous les tests se sont bien déroulés

**/
int main()
{
    std::cout << "Tests pour la classe Cellule\n";
    testEtatInitial();
    testModificationEtat();
    return 0;
}
