#pragma once

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>

using namespace std;

struct Cell
{
    bool valeur;
    float couleur;
};

class JeuDeLaVie
{
private:
    int TX, TY = 0;
    Cell **PLAN = nullptr;
    Cell **MIROIR = nullptr;
    int compte_voisins(int x, int y);
    void creation_matrice(void);

public:
    void renitialiser(void);
    void detruire(void);
    void calculer(void);
    void copier(void);
    void afficher(void);
};
