#include "Grille.hpp"
#include <iostream>
#include <fstream>
#include <string>


using namespace std;

Grille::Grille(int l, int c) : lignes(l), colonnes(c), cellules(l, vector<Cellule>(c)) {}

void Grille::initialiser(const vector<vector<bool>> &etatInitial)
{
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            cellules[i][j].definirEtat(etatInitial[i][j]);
        }
    }
}

void Grille::afficher() const
{
    for (const auto &ligne : cellules)
    {
        for (const auto &cellule : ligne)
        {
            cout << (cellule.estVivante() ? "1 " : "0 ");
        }
        cout << endl;
    }
    cout << endl;
}

int Grille::compterVoisinsVivants(int x, int y) const
{
    int voisinsVivants = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;
            int nx = x + i, ny = y + j;
            if (nx >= 0 && ny >= 0 && nx < lignes && ny < colonnes && cellules[nx][ny].estVivante())
            {
                voisinsVivants++;
            }
        }
    }
    return voisinsVivants;
}

void Grille::mettreAJour()
{
    vector<vector<Cellule>> nouvelleGeneration = cellules;

    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            int voisinsVivants = compterVoisinsVivants(i, j);

            if (cellules[i][j].estVivante())
            {
                nouvelleGeneration[i][j].definirEtat(voisinsVivants == 2 || voisinsVivants == 3);
            }
            else
            {
                nouvelleGeneration[i][j].definirEtat(voisinsVivants == 3);
            }
        }
    }

    cellules = move(nouvelleGeneration);
}

const vector<vector<Cellule>> &Grille::obtenirCellules() const
{
    return cellules;
}

int Grille::getLignes() const {
    return lignes;
}

int Grille::getColonnes() const {
    return colonnes;
}

