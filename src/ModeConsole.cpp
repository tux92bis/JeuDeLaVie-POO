

/**
 * @brief Mode d'exécution du Jeu de la Vie en console
 * @file ModeConsole.cpp
 *
 * Cette classe permet d'exécuter le Jeu de la Vie en mode console.
 * Elle gère l'affichage des itérations et l'écriture de l'état final dans un fichier.
 */

#include "ModeConsole.hpp"
#include <iostream>
#include <fstream>

using namespace std;

/**
 * @brief Constructeur de la classe ModeConsole
 * @param jeu Instance du Jeu de la Vie à exécuter
 * @param nomFichierEntree Nom du fichier d'entrée contenant l'état initial
 */

ModeConsole::ModeConsole(JeuDeLaVie &jeu, const string &nomFichierEntree)
    : jeu(jeu), nomFichierEntree(nomFichierEntree) {}

/**
 * @brief Exécute le Jeu de la Vie pour un nombre donné d'itérations
 * @param iterations Nombre d'itérations à effectuer
 *
 * Cette méthode:
 * - Affiche l'état de la grille à chaque itération
 * - Met à jour la grille entre chaque itération
 * - Écrit l'état final dans un fichier de sortie (nomFichierEntree + "_out.txt")
 *
 * @throw runtime_error Si le fichier de sortie ne peut pas être créé
 */

void ModeConsole::executer(int iterations)
{
    for (int i = 0; i < iterations; i++)
    {
        cout << "Itération " << (i + 1) << " :\n";
        jeu.getGrille().afficher();

        // Mettre à jour la grille sauf après la dernière itération
        if (i < iterations - 1)
        {
            jeu.getGrille().mettreAJour();
        }
    }

    // Écrire l'état final dans un fichier
    string fichierSortie = nomFichierEntree + "_out.txt";
    ofstream fichier(fichierSortie);
    if (!fichier)
    {
        throw runtime_error("Erreur : Impossible de créer le fichier " + fichierSortie);
    }

    // Écrire les dimensions
    const auto &grille = jeu.getGrille();
    fichier << grille.getLignes() << " " << grille.getColonnes() << "\n";

    // Écrire l'état final de la grille
    const auto &cellules = grille.obtenirCellules();
    for (const auto &ligne : cellules)
    {
        for (const auto &cellule : ligne)
        {
            fichier << (cellule.estVivante() ? "1 " : "0 ");
        }
        fichier << "\n";
    }

    fichier.close();
    cout << "L'état final a été écrit dans : " << fichierSortie << "\n";
}
