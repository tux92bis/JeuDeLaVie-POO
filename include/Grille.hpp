#pragma once

#include <string>
#include "Cellule.hpp"
#include <vector>

class Grille
{
private:
    int lignes, colonnes;
    std::vector<std::vector<Cellule>> cellules;

public:
    Grille(int l, int c);
    void initialiser(const std::vector<std::vector<bool>> &etatInitial);
    void afficher() const;
    int compterVoisinsVivants(int x, int y) const;
    void mettreAJour();
    const std::vector<std::vector<Cellule>> &obtenirCellules() const;
    int getLignes() const;
    int getColonnes() const;
    void ecrireDansFichier(const std::string &cheminFichier) const;
};
