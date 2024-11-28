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
    int lignes, colonnes;
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
    grille = Grille(lignes, colonnes); 
    grille.initialiser(etatInitial);

    cout << "Grille initialisée avec succès depuis le fichier " << nomFichier << " !\n";
    grille.afficher(); 
}

void JeuDeLaVie::executerModeConsole(const string &nomFichierEntree)
{
    for (int i = 0; i < iterations; i++)
    {
        cout << "Itération " << (i + 1) << " :\n";
        grille.afficher();
        grille.mettreAJour();
    }
    string fichierSortie = nomFichierEntree + "_out.txt";
    ofstream fichier(fichierSortie);
    if (!fichier)
    {
        throw runtime_error("Erreur : Impossible de créer le fichier " + fichierSortie);
    }
    fichier << grille.getLignes() << " " << grille.getColonnes() << "\n";
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

Grille &JeuDeLaVie::getGrille()
{
    return grille;
}

void JeuDeLaVie::executerModeGraphique(int tailleCellule)
{
    InterfaceGraphique interfaceGraphique(*this, tailleCellule);
    interfaceGraphique.executer(iterations);
}
