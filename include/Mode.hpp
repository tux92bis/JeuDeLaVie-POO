

/**
 * @brief Classe abstraite représentant un mode de jeu
 * 
 * Cette classe définit l'interface pour différents modes de jeu
 * qui peuvent être implémentés dans le Jeu de la Vie.
 */

/**
 * @brief Exécute le mode de jeu pour un nombre donné d'itérations
 * @param iterations Le nombre d'itérations à exécuter
 */
#pragma once

class mode
{
public:
    virtual void executer(int iterations) = 0; // Méthode pure virtuelle
    virtual ~mode() {}
};
