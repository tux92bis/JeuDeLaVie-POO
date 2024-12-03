

/**
 * @brief Interface graphique pour le Jeu de la Vie utilisant SFML
 * @file InterfaceGraphique.cpp
 * 
 * Ce fichier implémente l'affichage graphique du Jeu de la Vie.
 * Il utilise la bibliothèque SFML pour créer une fenêtre et afficher
 * la grille de cellules avec un rafraîchissement périodique.
 * 
 * !!! Installer SFML pour compiler le programme !!!
 * 
 * @class InterfaceGraphique
 * 
 * Le constructeur:
 * @param jeu Référence vers l'instance du Jeu de la Vie à afficher
 * @param taille Taille en pixels de chaque cellule dans la fenêtre
 * 
 * La méthode executer:
 * @param iterations Nombre d'itérations à exécuter (-1 pour infini)
 * 
 * Fonctionnalités:
 * - Crée une fenêtre SFML dimensionnée selon la taille de la grille
 * - Affiche les cellules vivantes en blanc et mortes en noir
 * - Met à jour l'affichage toutes les 100ms
 * - Permet de fermer la fenêtre avec le bouton de fermeture
 * - Continue l'exécution jusqu'au nombre d'itérations spécifié
 */
#include "InterfaceGraphique.hpp"
#include <thread>
#include <chrono>
#include <string>

using namespace std;

InterfaceGraphique::InterfaceGraphique(JeuDeLaVie &jeu, int taille) 
    : jeu(jeu), tailleCellule(taille) {} // Initialisation des attributs

void InterfaceGraphique::executer(int iterations) 
{
    auto &grille = jeu.getGrille(); // Récupération de la grille du jeu
    int largeurFenetre = grille.getColonnes() * tailleCellule; // Calcul de la largeur de la fenêtre
    int hauteurFenetre = grille.getLignes() * tailleCellule; // Calcul de la hauteur de la fenêtre

    sf::RenderWindow fenetre(sf::VideoMode(largeurFenetre, hauteurFenetre), "Jeu de la Vie"); // Création de la fenêtre SFML

    int currentIteration = 0; // Initialisation du compteur d'itérations
    while (fenetre.isOpen()) // Boucle principale de la fenêtre
    {
        sf::Event event; // Gestion des événements
        while (fenetre.pollEvent(event)) // Boucle de gestion des événements
        {
            if (event.type == sf::Event::Closed) // Si l'utilisateur ferme la fenêtre
            {
                fenetre.close();  // Fermeture de la fenêtre
            }
        }
        fenetre.clear(); // Effacement de la fenêtre
        const auto &cellules = grille.obtenirCellules(); // Récupération des cellules de la grille
        for (int x = 0; x < grille.getLignes(); x++) // Parcours de la grille
        {
            for (int y = 0; y < grille.getColonnes(); y++)
            {
                sf::RectangleShape rectangle(sf::Vector2f(tailleCellule, tailleCellule)); // Création d'un rectangle SFML
                rectangle.setPosition(y * tailleCellule, x * tailleCellule); // Positionnement du rectangle

                rectangle.setFillColor(cellules[x][y].estVivante() ? sf::Color::White : sf::Color::Black); // Couleur du rectangle
                fenetre.draw(rectangle); // Affichage du rectangle
            }
        }
        fenetre.display(); // Affichage de la fenêtre
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Pause de 100ms
        if (currentIteration < iterations) // Si le nombre d'itérations n'est pas atteint
        {
            grille.mettreAJour(); // Mise à jour de la grille
            currentIteration++; // Incrémentation du compteur d'itérations
        }
    }
}