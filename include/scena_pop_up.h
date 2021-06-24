#ifndef SCENA_POP_UP_H
#define SCENA_POP_UP_H
#include <SFML/Graphics.hpp>
#include <cstring>
#include "init.h"
class scena_pop_up
{
public:
    int tip;//1-stele
    int timp_precedent;
    sf::Image fundal;
    int stele;
    int fade_time=500000;
    char text[350];
    sf::RectangleShape blur;


    int countdown;

    void init_stele_pop_up(const sf::Image imagine,int numar_stele);

    void init_text(const sf::Image imagine, char txt[350]);

    void update(int dt);

    void fade_out();


    void draw();
};

#endif // SCENA_POP_UP_H
