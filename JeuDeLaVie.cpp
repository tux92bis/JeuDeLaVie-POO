#include "JeuDeLaVie.hpp"

void JeuDeLaVie::renitialiser()
{
    detruire();
    creation_matrice();
}


void JeuDeLaVie::creation_matrice()
{
    FILE *fichier = fopen("matrice.txt", "r");
    if (fichier == nullptr)
    {
        cerr << "Erreur lors de l'ouverture du fichier" << endl;
        exit(1);
    }
    fscanf(fichier, "%d %d", &TX, &TY);
    PLAN = new Cell *[TY];
    MIROIR = new Cell *[TY];
    for (int i = 0; i < TY; i++)
    {
        PLAN[i] = new Cell[TX];
        MIROIR[i] = new Cell[TX];
    }
    for (int y = 0; y < TY; y++)
    {
        for (int x = 0; x < TX; x++)
        {
            fscanf(fichier, "%d", &PLAN[y][x].valeur);
            PLAN[y][x].couleur = 0.0f;
        }
    }
    fclose(fichier);
}



void JeuDeLaVie::detruire()
{
    if (PLAN == nullptr && MIROIR == nullptr)
    {
        return;
    }
    for (int i = 0; i < TY; i++)
    {
        delete[] PLAN[i];
        delete[] MIROIR[i];
    }
    delete[] PLAN;
    delete[] MIROIR;
}

void JeuDeLaVie::calculer()
{
    int nb_voisins = 0;
    for (int y = 0; y < TY; y++)
    {
        for (int x = 0; x < TX; x++)
        {
            nb_voisins = compte_voisins(x, y);
            if (nb_voisins < 2 || nb_voisins > 3)
            {
                MIROIR[y][x].valeur = 0;
                MIROIR[y][x].couleur = PLAN[y][x].couleur;
            }
            else
            {
                MIROIR[y][x].valeur = 1;
                if (PLAN[y][x].couleur == 0)
                    MIROIR[y][x].couleur = 0.7f;
                else if (PLAN[y][x].couleur < 15)
                MIROIR[y][x].couleur = PLAN[y][x].couleur + 0.1f;
            }
        }
    }
}



int JeuDeLaVie::compte_voisins(int x, int y)
{
    int nb = 0;
    int yn, ys, xe, xo;
    yn = (y - 1 + TY) % TY;
    ys = (y + 1) % TY;
    xe = (x + 1) % TX;
    xo = (x - 1 + TX) % TX;

    if (PLAN[yn][xo].valeur == 1)
        nb++;
    if (PLAN[yn][x].valeur == 1)
        nb++;
    if (PLAN[yn][xe].valeur == 1)
        nb++;
    if (PLAN[y][xo].valeur == 1)
        nb++;
    if (PLAN[y][xe].valeur == 1)
        nb++;
    if (PLAN[ys][xo].valeur == 1)
        nb++;
    if (PLAN[ys][x].valeur == 1)
        nb++;
    if (PLAN[ys][xe].valeur == 1)
        nb++;
    return nb;
}

void JeuDeLaVie::copier()
{
    for (int i = 0; i < TX; i++)
    {
        memcpy(PLAN[i], MIROIR[i], sizeof(Cell) * TX);
    }
}
    
void JeuDeLaVie::afficher()
{
    // Génère la configuration de la matrice dans un fichier texte

    
}