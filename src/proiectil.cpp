#include "../include/proiectil.h"
#include "../include/init.h"
#include <cmath>
#include <iostream>
proiectil::proiectil()
{
    forma.setSize(sf::Vector2f(dimensiune_proiectile_x,dimensiune_proiectile_y));
    forma.setOrigin(sf::Vector2f(forma.getSize().x/2,forma.getSize().y/2));
    forma.setFillColor(sf::Color::Red);

}

void proiectil::setup(int x,int y,float viteza_Y,float viteza_X,int UNGHI, int COUNTDOWN,int indice_proprietaR)
{
    viteza_x=viteza_X;
    viteza_y=viteza_Y;
    unghi=UNGHI;
    countdown=COUNTDOWN;
    forma.setPosition(sf::Vector2f(x,y+5));
    indice_proprietar=indice_proprietaR;
    numar_victime=0;

}

void proiectil::miscare()
{
    if (countdown)
    {
        float sin_unghi=-sin(unghi*3.14/180);
        float cos_unghi=cos(unghi*3.14/180);
        //std::cout << sin_unghi<< " " << cos_unghi <<std::endl;
        forma.move(sf::Vector2f(cos_unghi*viteza_x,sin_unghi*viteza_y));
    }
    if (forma.getPosition().x>width+forma.getSize().x+10)
        forma.setPosition(sf::Vector2f(0-forma.getSize().x/2,forma.getPosition().y));
    if (forma.getPosition().x<0-forma.getSize().x-10)
        forma.setPosition(sf::Vector2f(width+forma.getSize().x/2,forma.getPosition().y));
    if (forma.getPosition().y>hight+forma.getSize().y+10)
        forma.setPosition(sf::Vector2f(forma.getPosition().x,0-forma.getSize().y/2));
    if (forma.getPosition().y<0-forma.getSize().y-10)
        forma.setPosition(sf::Vector2f(forma.getPosition().x,hight+forma.getSize().y/2));

}

void proiectil::test_colizune()
{
    if (countdown<=0)
        viteza_x=viteza_y=0;
    if (hp<=0)
        viteza_x=viteza_y=0;

}

bool proiectil::alive()
{
    if (viteza_x!=0)
        return 1;
    if (viteza_y!=0)
        return 1;
    return 0;

}

void proiectil::adaugare_victime(int x)
{
    victime[++numar_victime]=x;
}

bool proiectil::verificare_victima(int x)
{
    for (int i=1;i<=numar_victime;i++)
        if (victime[i]==x)
            return 1;
    return 0;
}

void proiectil::update_sprite()
{
    sprite.setPosition(forma.getPosition().x,forma.getPosition().y);
    indice_sprite++;
    //std::cout << sprite.getPosition().x << " " << sprite.getPosition().y << std::endl;
    if (indice_sprite>28)
        indice_sprite=1;
    sprite.setOrigin(sprite.getTexture()->getSize().x/2,sprite.getTexture()->getSize().y/2);
    sprite.setRotation(-unghi);
    sprite.setScale(sf::Vector2f((float)forma.getSize().x/187,(float)forma.getSize().y/108));

}
