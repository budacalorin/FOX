#include "morisca.h"
#include <iostream>
using namespace std;


void morisca::setup(int numar_palete,sf::Vector2f pozitie_centru,sf::Vector2f dimensiune_palete,sf::Texture *textura,int switch_timE)
{
    switch_time=switch_timE;
    this->numar_palete=numar_palete;
    for (int i=1;i<=numar_palete;i++)
    {
        palete[i].setOrigin(dimensiune_palete.x/2,dimensiune_palete.y);
        palete[i].setPosition(pozitie_centru);
        palete[i].setSize(dimensiune_palete);
        palete[i].setTexture(textura);
        palete[i].setRotation((float)360/numar_palete*i);
        //cout << (float)360/numar_palete*i << endl;

    }
}

void morisca::update(int delta_time)
{
    total_time+=delta_time;
    if (total_time>=switch_time)
    {
        for (int i=1;i<=numar_palete;i++)
        {
            palete[i].rotate((float)5*((float)total_time/switch_time));
        }
        total_time=0;
    }
}
