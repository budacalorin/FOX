#ifndef PLASMA_GUN_H
#define PLASMA_GUN_H
#include <SFML/Graphics.hpp>
#include "init.h"

class plasma_gun
{
public:
    sf::RectangleShape forma;
    int cooldown_tragere;
    int curent_time;
    int indice_bloc;
    int cooldown_plasma;
    sf::Vector2f directie;
    int unghi;
    int stare;
};

#endif // PLASMA_GUN_H
