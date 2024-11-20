#pragma once

#include "JeuDeLaVie.hpp"
#include <string>

class ModeConsole {
private:
    JeuDeLaVie& jeu;
    std::string nomFichierEntree;

public:
    ModeConsole(JeuDeLaVie& jeu, const std::string& nomFichierEntree);
    void executer(int iterations);
};

