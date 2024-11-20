#include "ModeConsole.hpp"
#include <iostream>
#include <fstream>

using namespace std;

ModeConsole::ModeConsole(JeuDeLaVie &jeu, const string &nomFichierEntree)
    : jeu(jeu), nomFichierEntree(nomFichierEntree) {}

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
