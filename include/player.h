#ifndef PLAYER
#define PLAYER

#include <SFML/Graphics.hpp>
#include "bloc.h"
#include "../include/animatie.h"

using namespace std;


class player
{
public:
    sf::Sprite sprite;
    sf::RectangleShape forma;
    float viteza_x,viteza_y;
    float pixel_miscare;
    bool orientare_dreapta=1;
    bool sta_jos=0;
    int hp=200;
    sf::Texture *textura;
    sf::Keyboard::Key sus,jos,stanga,dreapta,action;
    bool sare;
    bool alive;
    animatie animatia;
    int cooldown_saritura=0;
    int marja_eroare_saritura=0;

    void set_positioin(int x,int y);

    void set_texture(sf::Texture *texture);

    void setTaste (sf::Keyboard::Key su,sf::Keyboard::Key jo,sf::Keyboard::Key st,sf::Keyboard::Key dr,sf::Keyboard::Key act);

    void set_size(int x,int y);

    bool miscare(bloc blocuri[],int numar_blocuri,bloc blocuri_masina[],int numar_blocuri_masina,float dt);

    void deplasare(bloc blocuri[],int numar_blocuri,bloc blocuri_masina[],int numar_blocuri_masina,float dt);
};

#endif
