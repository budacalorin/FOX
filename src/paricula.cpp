#include "../include/particula.h"
#include "../include/init.h"
#include <cmath>
#include <iostream>
particula::particula()
{
    forma.setSize(sf::Vector2f(2*((float)width/1366),2*((float)hight/768)));
    forma.setFillColor(sf::Color(200,200,200));
}

void particula::setup(int x,int y,float viteza_Y,float viteza_X,int UNGHI, int COUNTDOWN,sf::Color culoare)
{
    viteza_x=viteza_X;
    viteza_y=viteza_Y;
    unghi=UNGHI;
    countdown=COUNTDOWN;
    forma.setPosition(sf::Vector2f(x,y));
    forma.setFillColor(culoare);

}

void particula::miscare(float x)
{

    if (countdown)
    {
        sf::Color culoare=forma.getFillColor();
        culoare.a-=x/1000;
        forma.setFillColor(culoare);
        float sin_unghi=-sin(unghi*3.14/180);
        float cos_unghi=cos(unghi*3.14/180);
        forma.move(sf::Vector2f(cos_unghi*viteza_x*((float)x/16666),sin_unghi*viteza_y*((float)x/16666)));
    }
}

void particula::test_colizune()
{
    if (countdown<=0)
        viteza_x=viteza_y=0;
    if (hp<=0)
        viteza_x=viteza_y=0;
}


bool particula::alive()
{
    if (viteza_x!=0)
        return 1;
    if (viteza_y!=0)
        return 1;
    return 0;

}
