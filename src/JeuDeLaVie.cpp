#include "JeuDeLaVie.hpp"
#include "InterfaceGraphique.hpp"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <filesystem>
#include <chrono>
#include <stdexcept>
#include <sstream>

using namespace std;

JeuDeLaVie::JeuDeLaVie(int lignes, int colonnes, int iter) : grille(lignes, colonnes), iterations(iter) {}

vector<vector<bool>> JeuDeLaVie::chargerEtatDepuisFichier(const string &nomFichier, int &lignes, int &colonnes)
{
    ifstream fichier(nomFichier);
    if (!fichier)
    {
        cerr << "Erreur : Impossible d'ouvrir le fichier " << nomFichier << endl;
        exit(EXIT_FAILURE);
    }

    fichier >> lignes >> colonnes;

    vector<vector<bool>> etatInitial(lignes, vector<bool>(colonnes));
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            int valeur;
            fichier >> valeur;
            etatInitial[i][j] = (valeur != 0);
        }
    }

    return etatInitial;
}

void JeuDeLaVie::saisirEtatInitial()
{
    // État initial
    vector<vector<bool>> etatInitial(grille.getLignes(), vector<bool>(grille.getColonnes()));

    cout << "Saisissez l'état initial des cellules (0 pour morte, 1 pour vivante) :\n";
    for (int i = 0; i < grille.getLignes(); i++)
    {
        for (int j = 0; j < grille.getColonnes(); j++)
        {
            cout << "Cellule (" << i << ", " << j << ") : ";
            int valeur;
            cin >> valeur;
            etatInitial[i][j] = (valeur != 0);
        }
    }

    grille.initialiser(etatInitial);
    cout << "Grille initialisée avec succès !\n";
    grille.afficher();
}

void JeuDeLaVie::chargerEtatInitial(const string &nomFichier)
{
    ifstream fichier(nomFichier);
    if (!fichier)
    {
        cerr << "Erreur : Impossible d'ouvrir le fichier " << nomFichier << endl;
        exit(EXIT_FAILURE);
    }

    // Lire les dimensions
    int lignes, colonnes;
    fichier >> lignes >> colonnes;

    // Lire l'état initial
    vector<vector<bool>> etatInitial(lignes, vector<bool>(colonnes));
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            int valeur;
            fichier >> valeur;
            etatInitial[i][j] = (valeur != 0);
        }
    }

    // Initialiser la grille avec les dimensions et l'état initial
    grille = Grille(lignes, colonnes); // Recrée une grille avec les bonnes dimensions
    grille.initialiser(etatInitial);

    cout << "Grille initialisée avec succès depuis le fichier " << nomFichier << " !\n";
    grille.afficher(); // Afficher la grille après initialisation
}

void JeuDeLaVie::executerModeConsole(const std::string& nomFichierEntree) {
    // Effectuer les itérations
    for (int i = 0; i < iterations; i++) {
        std::cout << "Itération " << (i + 1) << " :\n";
        grille.afficher();
        grille.mettreAJour();
    }

    // Construire le nom du fichier de sortie
    std::string fichierSortie = nomFichierEntree + "_out.txt";

    // Écrire l'état final dans le fichier
    std::ofstream fichier(fichierSortie);
    if (!fichier) {
        throw std::runtime_error("Erreur : Impossible de créer le fichier " + fichierSortie);
    }

    // Écrire les dimensions
    fichier << grille.getLignes() << " " << grille.getColonnes() << "\n";

    // Écrire l'état final de la grille
    const auto& cellules = grille.obtenirCellules();
    for (const auto& ligne : cellules) {
        for (const auto& cellule : ligne) {
            fichier << (cellule.estVivante() ? "1 " : "0 ");
        }
        fichier << "\n";
    }
    fichier.close();

    std::cout << "L'état final a été écrit dans : " << fichierSortie << "\n";
}

Grille& JeuDeLaVie::getGrille() {
    return grille;
}



void JeuDeLaVie::executerModeGraphique(int tailleCellule)
{
    InterfaceGraphique interfaceGraphique(*this, tailleCellule);
    interfaceGraphique.executer(iterations);

}
