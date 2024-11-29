/**
 * @class InterfaceGraphique
 * @brief Classe représentant l'interface graphique du Jeu de la Vie.
 * 
 * Cette classe hérite de la classe Mode et permet d'exécuter le Jeu de la Vie
 * avec une interface graphique utilisant la bibliothèque SFML.
 * 
 * @file InterfaceGraphique.hpp
 * 
 * @see Mode
 * @see Grille
 * @see JeuDeLaVie
 */

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