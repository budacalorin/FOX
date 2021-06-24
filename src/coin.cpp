#include "coin.h"
#include "init.h"

coin::coin()
{

}

void coin::setup(sf::Vector2f pozitie)
{
    animatia.setup(&textura_coin,sf::Vector2u(61,1),50000);
    forma.setSize(sf::Vector2f(48,48));
    forma.setOrigin(sf::Vector2f(24,24));
    forma.setTexture(&textura_coin);
    forma.setFillColor(sf::Color::White);
     forma.setPosition(pozitie);

}
