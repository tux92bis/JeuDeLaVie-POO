

/**
 * @brief Classe représentant une cellule dans le jeu de la vie
 *
 * Cette classe gère l'état d'une cellule (vivante ou morte) dans une grille du jeu de la vie.
 * Une cellule peut être soit vivante (true) soit morte (false).
 */

#pragma once

/**
 * @brief Attribut privé indiquant si la cellule est vivante
 */
// vivante

/**
 * @brief Constructeur de la classe Cellule
 * @param etat État initial de la cellule (false par défaut)
 */
// Cellule(bool etat)

/**
 * @brief Vérifie si la cellule est vivante
 * @return true si la cellule est vivante, false sinon
 */
// estVivante()

/**
 * @brief Modifie l'état de la cellule
 * @param etat Nouvel état de la cellule
 */
// definirEtat(bool etat)

class Cellule 
{
private:
    bool vivante;

public:
    Cellule(bool etat = false);
    bool estVivante() const;
    void definirEtat(bool etat);
};
