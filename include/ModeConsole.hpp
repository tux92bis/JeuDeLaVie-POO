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
