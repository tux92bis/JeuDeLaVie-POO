#include "InterfaceGraphique.hpp"
#include <thread>
#include <chrono>
#include <string>

using namespace std;

InterfaceGraphique::InterfaceGraphique(JeuDeLaVie& jeu, int taille)
    : jeu(jeu), tailleCellule(taille) {}

void InterfaceGraphique::executer(int iterations) {
    auto& grille = jeu.getGrille();

    // Calculer la taille de la fenêtre
    int largeurFenetre = grille.getColonnes() * tailleCellule;
    int hauteurFenetre = grille.getLignes() * tailleCellule;

    sf::RenderWindow fenetre(sf::VideoMode(largeurFenetre, hauteurFenetre), "Jeu de la Vie");

    int currentIteration = 0;
    while (fenetre.isOpen()) {
        sf::Event event;
        while (fenetre.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                fenetre.close(); // Fermer la fenêtre si l'utilisateur clique sur "fermer"
            }
        }

        // Dessiner la grille
        fenetre.clear();
        const auto& cellules = grille.obtenirCellules();
        for (int x = 0; x < grille.getLignes(); x++) {
            for (int y = 0; y < grille.getColonnes(); y++) {
                sf::RectangleShape rectangle(sf::Vector2f(tailleCellule, tailleCellule));
                rectangle.setPosition(y * tailleCellule, x * tailleCellule);

                rectangle.setFillColor(cellules[x][y].estVivante() ? sf::Color::Green : sf::Color::Black);
                rectangle.setOutlineThickness(1);
                rectangle.setOutlineColor(sf::Color::White);

                fenetre.draw(rectangle);
            }
        }
        fenetre.display();

        // Pause pour visualiser l'évolution
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Mettre à jour la grille pour la prochaine itération
        if (currentIteration < iterations) {
            grille.mettreAJour();
            currentIteration++;
        }

        // Après la dernière itération, rester en pause jusqu'à fermeture manuelle
        if (currentIteration >= iterations) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Pause pour observer le résultat
        }
    }
}