

/**
 * @brief Classe représentant le Jeu de la Vie de Conway
 * 
 * Cette classe implémente la logique du Jeu de la Vie, permettant de:
 * - Charger un état initial depuis un fichier
 * - Exécuter le jeu en mode console ou graphique
 * - Saisir manuellement un état initial
 * - Gérer l'évolution des cellules selon les règles du jeu
 */
#pragma once

#include "Grille.hpp"
#include <string>

class JeuDeLaVie
{
private:
    Grille grille;
    int iterations;
    std::vector<std::vector<bool>> chargerEtatDepuisFichier(const std::string &nomFichier, int &lignes, int &colonnes); // Méthode privée pour charger l'état initial depuis un fichier

public:
    JeuDeLaVie(int lignes, int colonnes, int iter);
    void chargerEtatInitial(const std::string &nomFichier);
    void executerModeConsole(const std::string &nomFichierEntree);
    void executerModeGraphique(int tailleCellule);
    void saisirEtatInitial();
    Grille &getGrille();
};
