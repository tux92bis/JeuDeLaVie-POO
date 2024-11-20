#include "InterfaceGraphique.hpp"
#include <thread>
#include <chrono>
#include <string>

using namespace std;

InterfaceGraphique::InterfaceGraphique(Grille& g, int taille) : grille(g), tailleCellule(taille) {}

void InterfaceGraphique::afficher() {
    // Calculer la taille de la fenêtre
    int largeurFenetre = grille.getColonnes() * tailleCellule;
    int hauteurFenetre = grille.getLignes() * tailleCellule;

    sf::RenderWindow fenetre(sf::VideoMode(largeurFenetre, hauteurFenetre), "Jeu de la Vie");

    while (fenetre.isOpen()) {
        sf::Event event;
        while (fenetre.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                fenetre.close();
            }
        }

        fenetre.clear();

        // Dessiner les cellules
        const auto& cellules = grille.obtenirCellules();
        for (int i = 0; i < grille.getLignes(); i++) {
            for (int j = 0; j < grille.getColonnes(); j++) {
                sf::RectangleShape rectangle(sf::Vector2f(tailleCellule, tailleCellule));
                rectangle.setPosition(j * tailleCellule, i * tailleCellule);

                if (cellules[i][j].estVivante()) {
                    rectangle.setFillColor(sf::Color::Green);
                } else {
                    rectangle.setFillColor(sf::Color::Black);
                }

                fenetre.draw(rectangle);
            }
        }

        fenetre.display();

        // Mettre à jour la grille
        grille.mettreAJour();

        // Pause pour contrôler la vitesse
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}