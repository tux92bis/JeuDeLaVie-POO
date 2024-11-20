#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Cellule.hpp"

TEST_CASE("Cellule: Initialisation et état")
{
    Cellule c;

    SECTION("Cellule est morte par défaut")
    {
        REQUIRE(c.estVivante() == false);
    }

    SECTION("Modification de l'état de la cellule")
    {
        c.setVivante(true);
        REQUIRE(c.estVivante() == true);

        c.setVivante(false);
        REQUIRE(c.estVivante() == false);
    }
}
