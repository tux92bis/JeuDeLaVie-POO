

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
    : jeu(jeu), tailleCellule(taille) {}

void InterfaceGraphique::executer(int iterations)
{
    auto &grille = jeu.getGrille();
    int largeurFenetre = grille.getColonnes() * tailleCellule;
    int hauteurFenetre = grille.getLignes() * tailleCellule;

    sf::RenderWindow fenetre(sf::VideoMode(largeurFenetre, hauteurFenetre), "Jeu de la Vie");

    int currentIteration = 0;
    while (fenetre.isOpen())
    {
        sf::Event event;
        while (fenetre.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                fenetre.close();
            }
        }
        fenetre.clear();
        const auto &cellules = grille.obtenirCellules();
        for (int x = 0; x < grille.getLignes(); x++)
        {
            for (int y = 0; y < grille.getColonnes(); y++)
            {
                sf::RectangleShape rectangle(sf::Vector2f(tailleCellule, tailleCellule));
                rectangle.setPosition(y * tailleCellule, x * tailleCellule);

                rectangle.setFillColor(cellules[x][y].estVivante() ? sf::Color::White : sf::Color::Black);
                fenetre.draw(rectangle);
            }
        }
        fenetre.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (currentIteration < iterations)
        {
            grille.mettreAJour();
            currentIteration++;
        }
    }
}