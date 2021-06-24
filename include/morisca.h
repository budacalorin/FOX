#ifndef MORISCA_H
#define MORISCA_H
#include <SFML/Graphics.hpp>

class morisca
{
    public:

        sf::RectangleShape palete[11];
        int numar_palete;
        int switch_time;
        int total_time;


        void update(int delta_time);

        void setup(int numar_palete,sf::Vector2f pozitie_centru,sf::Vector2f dimensiune_palete,sf::Texture *textura,int switch_timE);


};

#endif // MORISCA_H
