#ifndef COIN_H
#define COIN_H
#include "animatie.h"
#include <SFML/Graphics.hpp>


class coin
{
public:
    animatie animatia;
    sf::RectangleShape forma;

    coin();
    void setup(sf::Vector2f pozitie);

};

#endif // COIN_H
