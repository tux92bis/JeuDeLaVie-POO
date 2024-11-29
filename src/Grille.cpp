

/**
 * @file Grille.cpp
 * @brief Implémentation de la classe Grille pour le Jeu de la Vie.
 *
 * Ce fichier contient les définitions des méthodes de la classe Grille,
 * qui représente une grille de cellules pour le Jeu de la Vie de Conway.
 *
 * @author Gabriel
 * @date 2023
 */

#include "Grille.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/**
 * @brief Constructeur de la classe Grille.
 *
 * Initialise une grille avec un nombre donné de lignes et de colonnes.
 *
 * @param l Nombre de lignes de la grille.
 * @param c Nombre de colonnes de la grille.
 */

Grille::Grille(int l, int c) : lignes(l), colonnes(c), cellules(l, vector<Cellule>(c)) {}

/**
 * @brief Initialise la grille avec un état initial donné.
 *
 * @param etatInitial Un vecteur 2D de booléens représentant l'état initial des cellules.
 */

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

/**
 * @brief Affiche la grille dans la console.
 *
 * Affiche chaque cellule comme "1" si elle est vivante et "0" si elle est morte.
 */
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

/**
 * @brief Compte le nombre de voisins vivants d'une cellule donnée.
 *
 * @param x Coordonnée x de la cellule.
 * @param y Coordonnée y de la cellule.
 * @return int Le nombre de voisins vivants.
 */

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

/**
 * @brief Met à jour la grille à la génération suivante selon les règles du Jeu de la Vie.
 *
 * Applique les règles de survie, de naissance et de mort des cellules pour générer la nouvelle génération.
 */

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

/**
 * @brief Obtient les cellules de la grille.
 *
 * @return const vector<vector<Cellule>>& Une référence constante au vecteur 2D de cellules.
 */

const vector<vector<Cellule>> &Grille::obtenirCellules() const
{
    return cellules;
}

/**
 * @brief Obtient le nombre de lignes de la grille.
 *
 * @return int Le nombre de lignes.
 */
int Grille::getLignes() const
{
    return lignes;
}

/**
 * @brief Obtient le nombre de colonnes de la grille.
 *
 * @return int Le nombre de colonnes.
 */

int Grille::getColonnes() const
{
    return colonnes;
}
