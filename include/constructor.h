#ifndef CONSTRUCTOR_H
#define CONSTRUCTOR_H
#include "SFML/Graphics.hpp"
#include "../include/bloc.h"
#include "../include/animatie.h"

class constructor
{
    public:
        sf::RectangleShape forma;
        sf::Sprite sprite;
        int blocuri_ramase[10];
        int tip_bloc=1;
        animatie animatia;
        bool dreapta=1;

        constructor();

        void set_position(int x,int y);
        void deplasare();
        void miscare(bloc blocuri[],int numar_blocuri,float dt);

    private:
};

#endif // CONSTRUCTOR_H
