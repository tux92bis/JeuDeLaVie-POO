#pragma once

#include "Mode.hpp"
#include "Grille.hpp"
#include <SFML/Graphics.hpp>
#include "JeuDeLaVie.hpp"

class InterfaceGraphique : public mode
{
private:
    JeuDeLaVie &jeu; // Relation directe avec JeuDeLaVie
    int tailleCellule;

public:
    InterfaceGraphique(JeuDeLaVie &jeu, int tailleCellule);
    void executer(int iterations) override;
};