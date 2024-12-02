

/**
 * @brief Classe représentant une grille de jeu de la vie
 * 
 * Cette classe gère une grille bidimensionnelle de cellules pour le jeu de la vie.
 * Elle fournit les méthodes nécessaires pour initialiser, afficher et faire évoluer
 * l'état des cellules selon les règles du jeu de la vie.
 * 
 * @class Grille
 * 
 * @var lignes 
 * Nombre de lignes de la grille
 * @var colonnes
 * Nombre de colonnes de la grille
 * @var cellules
 * Matrice de cellules représentant l'état de la grille
 * 
 * @fn Grille(int l, int c)
 * Constructeur créant une grille de dimensions l x c
 * @param l Nombre de lignes
 * @param c Nombre de colonnes
 * 
 * @fn void initialiser(const std::vector<std::vector<bool>> &etatInitial)
 * Initialise l'état des cellules de la grille
 * @param etatInitial Matrice de booléens définissant l'état initial des cellules
 * 
 * @fn void afficher() const
 * Affiche l'état actuel de la grille
 * 
 * @fn int compterVoisinsVivants(int x, int y) const
 * Compte le nombre de cellules vivantes autour d'une position donnée
 * @param x Position en x de la cellule
 * @param y Position en y de la cellule
 * @return Nombre de voisins vivants
 * 
 * @fn void mettreAJour()
 * Met à jour l'état de toutes les cellules selon les règles du jeu
 * 
 * @fn const std::vector<std::vector<Cellule>> &obtenirCellules() const
 * Retourne une référence constante vers la matrice de cellules
 * @return Référence vers la matrice de cellules
 * 
 * @fn int getLignes() const
 * Retourne le nombre de lignes de la grille
 * @return Nombre de lignes
 * 
 * @fn int getColonnes() const
 * Retourne le nombre de colonnes de la grille
 * @return Nombre de colonnes
 * 
 * @fn void ecrireDansFichier(const std::string &cheminFichier) const
 * Écrit l'état actuel de la grille dans un fichier
 * @param cheminFichier Chemin du fichier où sauvegarder la grille
 */
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
