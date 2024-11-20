#pragma once

#include "Grille.hpp"
#include <SFML/Graphics.hpp>

class InterfaceGraphique
{
private:
    Grille &grille;
    int tailleCellule;

public:
    InterfaceGraphique(Grille &g, int taille);
    void afficher();
};
