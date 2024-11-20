#pragma once

#include "Grille.hpp"
#include <string>

class JeuDeLaVie
{
private:
    Grille grille;
    int iterations;
    std::vector<std::vector<bool>> chargerEtatDepuisFichier(const std::string &nomFichier, int &lignes, int &colonnes);

public:
    JeuDeLaVie(int lignes, int colonnes, int iter);
    void chargerEtatInitial(const std::string &nomFichier);
    void executerModeConsole(const std::string &nomFichierEntree);
    void executerModeGraphique(int tailleCellule);
    void saisirEtatInitial();
    Grille &getGrille();
};
