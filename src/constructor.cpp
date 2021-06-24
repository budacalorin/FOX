#include "constructor.h"
#include "init.h"
void coliziune_orizontala(sf::RectangleShape &forma,bloc blocuri[],int numar_blocuri,bloc blocuri_masina[],int numar_blocuri_masina,float &viteza_x);
void coliziune_verticala(sf::RectangleShape &forma,bloc blocuri[],int numar_blocuri,bloc blocuri_masina[],int numar_blocuri_masina,float &viteza_y);


constructor::constructor()
{
    forma.setSize(sf::Vector2f(constructor_x,constructor_y));
    forma.setFillColor(sf::Color::Red);
    animatia.curent_image.x=0;
    animatia.curent_image.y=0;
}

void constructor::set_position(int x,int y)
{
    forma.setPosition(sf::Vector2f(x,y));
}
void constructor::deplasare()
{
}
void constructor::miscare(bloc blocuri[],int numar_blocuri,float dt)
{
    bool x=sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
    float pixeli_per_frame=viteza_masina;
    if (x==1)
        pixeli_per_frame=1;
    pixeli_per_frame*=(float)dt/16666;
    sf::Vector2f v;
    v.x=v.y=0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        v.x=pixeli_per_frame;
        if (dreapta==0)
        {
            dreapta=1;
        }
    }else
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        v.x=-pixeli_per_frame;
        if (dreapta==1)
        {
            dreapta=0;

        }
    }
    forma.move(v);
    coliziune_orizontala(forma,blocuri,numar_blocuri,blocuri,0,v.x);
    v.x=0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        v.y=-pixeli_per_frame;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        v.y=pixeli_per_frame;
    forma.move(v);
    coliziune_verticala(forma,blocuri,numar_blocuri,blocuri,0,v.y);
    v.y=0;
    sprite.setPosition(forma.getPosition().x-27*((float)width/1366)+masina_offset_X,forma.getPosition().y-43*((float)hight/768)+masina_offset_Y);
}


