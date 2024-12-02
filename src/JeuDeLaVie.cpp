
/**
 * @brief Classe représentant le Jeu de la Vie de Conway
 * 
 * Cette classe implémente la logique du Jeu de la Vie, un automate cellulaire
 * inventé par John Conway. Elle gère une grille de cellules qui évoluent selon
 * des règles spécifiques.
 */

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

/**
 * @brief Constructeur de la classe JeuDeLaVie
 * @param lignes Nombre de lignes de la grille
 * @param colonnes Nombre de colonnes de la grille
 * @param iter Nombre d'itérations à exécuter
 */

JeuDeLaVie::JeuDeLaVie(int lignes, int colonnes, int iter) : grille(lignes, colonnes), iterations(iter) {}


/**
 * @brief Charge l'état initial depuis un fichier
 * @param nomFichier Chemin du fichier à charger
 * @param lignes Référence pour stocker le nombre de lignes
 * @param colonnes Référence pour stocker le nombre de colonnes
 * @return vector<vector<bool>> État initial de la grille
 */


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



/**
 * @brief Permet à l'utilisateur de saisir l'état initial de la grille
 * 
 * L'utilisateur doit entrer 0 pour une cellule morte et 1 pour une cellule vivante dans le fichier texte
 */

void JeuDeLaVie::saisirEtatInitial()
{
    vector<vector<bool>> etatInitial(grille.getLignes(), vector<bool>(grille.getColonnes())); // Initialisation de la grille avec des cellules mortes
    cout << "Saisissez l'état initial des cellules (0 pour morte, 1 pour vivante) :\n";
    for (int i = 0; i < grille.getLignes(); i++)
    {
        for (int j = 0; j < grille.getColonnes(); j++) // L'utilisateur doit entrer 0 pour une cellule morte et 1                                                  pour une cellule vivante
        {
            cout << "Cellule (" << i << ", " << j << ") : ";
            int valeur;
            cin >> valeur;
            etatInitial[i][j] = (valeur != 0);// Si la valeur est différente de 0, la cellule est vivante
        }
    }

    grille.initialiser(etatInitial);
    cout << "Grille initialisée avec succès !\n";
    grille.afficher();
}

/**
 * @brief Charge l'état initial depuis un fichier texte et initialise la grille
 * @param nomFichier Chemin du fichier contenant l'état initial
*/


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


/*
* @brief Exécute le jeu en mode console pour un nombre donné d'itérations
* @param nomFichierEntree Chemin du fichier d'entrée
*/

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



/**
 * @brief Exécute le jeu en mode graphique avec une interface SFML
 * @param tailleCellule Taille en pixels de chaque cellule
 */
void JeuDeLaVie::executerModeGraphique(int tailleCellule)
{
    InterfaceGraphique interfaceGraphique(*this, tailleCellule);
    interfaceGraphique.executer(iterations);
}
