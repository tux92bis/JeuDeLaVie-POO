
/**
 * @brief Classe représentant une cellule dans le jeu de la vie.
 *
 * Cette classe permet de gérer l'état d'une cellule (vivante ou morte)
 * dans une grille du jeu de la vie.
 */

#include "Cellule.hpp"

/**
 * @brief Constructeur de la classe Cellule
 * @param etat État initial de la cellule (true = vivante, false = morte)
 */

Cellule::Cellule(bool etat) : vivante(etat) {}

/**
 * @brief Vérifie si la cellule est vivante
 * @return true si la cellule est vivante, false sinon
 */

bool Cellule::estVivante() const
{
    return vivante;
}

/**
 * @brief Modifie l'état de la cellule
 * @param etat Nouvel état de la cellule (true = vivante, false = morte)
 */

void Cellule::definirEtat(bool etat)
{
    vivante = etat;
}
