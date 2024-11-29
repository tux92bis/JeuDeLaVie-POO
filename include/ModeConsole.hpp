

/**
 * @brief Classe représentant le mode d'exécution console du Jeu de la Vie
 * 
 * Cette classe gère l'exécution du jeu de la vie en mode console,
 * permettant de lire les configurations depuis un fichier et d'afficher
 * les résultats dans la console.
 * 
 * @see mode
 * @see JeuDeLaVie
 */
#pragma once
#include "Mode.hpp"
#include "JeuDeLaVie.hpp"
#include <string>

class ModeConsole : public mode
{
private:
    JeuDeLaVie &jeu;
    std::string nomFichierEntree;

public:
    ModeConsole(JeuDeLaVie &jeu, const std::string &nomFichierEntree);
    void executer(int iterations) override;
};
