#include "JeuDeLaVie.hpp"
#include "Affichage.hpp"
#include <unistd.h>

int main()
{
    JeuDeLaVie A;
    A.renitialiser();
    A.afficher();
    return 1;
}