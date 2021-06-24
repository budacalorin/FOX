#include "../include/player.h"
#include "../include/init.h"
#include <SFML/Audio.hpp>
#include <iostream>
void coliziune_verticala(sf::RectangleShape &forma,bloc blocuri[],int numar_blocuri,bloc blocuri_masina[],int numar_blocuri_masina,float &viteza_y);
void coliziune_orizontala(sf::RectangleShape &forma,bloc blocuri[],int numar_blocuri,bloc blocuri_masina[],int numar_blocuri_masina,float &viteza_x);
bool coliziune_player(sf::RectangleShape forma,bloc blocuri[],int numar_blocuri,bloc blocuri_masina[],int numar_blocuri_masina);

void player::set_positioin(int x,int y)
{
    forma.setPosition(sf::Vector2f(x,y));
    forma.setFillColor(sf::Color::Black);
    sprite.setOrigin(sf::Vector2f(50,50));
    animatia.curent_image.x=animatia.curent_image.y=0;


}

void player::set_texture(sf::Texture *texture)
{
    textura=texture;
}

void player::set_size(int x,int y)
{
    forma.setSize(sf::Vector2f(x,y));
}

bool player::miscare(bloc blocuri[],int numar_blocuri,bloc blocuri_masina[],int numar_blocuri_masina,float dt)
{
    bool moare=0;

    if (viteza_x<=0.5&&viteza_x>=-0.5&&sta_jos==0)
        viteza_x=0;
    if (viteza_y<0.01&&viteza_y>-0.01)
        viteza_y=0;
    float frecare=0.3*coeficient_frecare;
    if (viteza_x>frecare)
        viteza_x-=frecare;
    else if (viteza_x<-frecare)
        viteza_x+=frecare;
    else
        viteza_x=0;
    //cout << viteza_y << endl;
    float g=gravitatie*(float)dt/16666;
    viteza_y+=g;
    float inaltime=(dt/16666)*(((15*15)/(g))-((g/2)*((15/(g)*((15+(g))/(g))))));
    //cout << inaltime <<endl;
    //cout << 324-forma.getPosition().y << endl<<endl;
    //cout << viteza_x << " " << viteza_y << endl<<endl;
    //cout << forma.getPosition().x << " " << forma.getPosition().y << endl<<endl;


    if (viteza_y>20*((float)width/1366))
        viteza_y=20*((float)width/1366);

    forma.move(sf::Vector2f(viteza_x*(float)dt/16666*(153/inaltime),0));

    coliziune_orizontala(forma,blocuri,numar_blocuri,blocuri_masina,numar_blocuri_masina,viteza_x);
    for (int i=1; i<=numar_blocuri; i++)
        if (blocuri[i].forma.getGlobalBounds().intersects(forma.getGlobalBounds())&&blocuri[i].alive==1&&blocuri[i].killer==1)
        {
            moare=1;
        }

    forma.move(sf::Vector2f(0,viteza_y*(float)dt/16666*(153/inaltime)));
    float propulsie=0;
    for (int i=1; i<=numar_blocuri_masina; i++)
    {
        if (blocuri_masina[i].forma.getGlobalBounds().intersects(forma.getGlobalBounds())&&blocuri_masina[i].alive==1&&blocuri_masina[i].propulsie!=0&&forma.getPosition().y<blocuri_masina[i].forma.getPosition().y&&!sf::Keyboard::isKeyPressed(jos))
        {
            propulsie=blocuri_masina[i].propulsie;
            blocuri_masina[i].blocat=1;
            blocuri_masina[i].forma.setTexture(&bloc_propulsie_blocat);

        }
    }

    coliziune_verticala(forma,blocuri,numar_blocuri,blocuri_masina,numar_blocuri_masina,viteza_y);
    for (int i=1; i<=numar_blocuri; i++)
        if (blocuri[i].forma.getGlobalBounds().intersects(forma.getGlobalBounds())&&blocuri[i].alive==1&&blocuri[i].killer==1)
        {
            moare=1;
        }
    if (propulsie!=0)
        viteza_y=-propulsie*coeficient_propulsie;

    if (sta_jos==1&&!sf::Keyboard::isKeyPressed(jos))
    {
        forma.move(sf::Vector2f(0,-25*((float)hight/768)));
        forma.setSize(sf::Vector2f(forma.getSize().x,forma.getSize().y+25*((float)hight/768)));
        if (coliziune_player(forma,blocuri,numar_blocuri,blocuri_masina,numar_blocuri_masina)==0)
        {
            sta_jos=0;
        }
        else
        {
            forma.move(sf::Vector2f(0,25*((float)hight/768)));
            forma.setSize(sf::Vector2f(forma.getSize().x,forma.getSize().y-25*((float)hight/768)));
        }
    }
    return moare;
}

void player::setTaste (sf::Keyboard::Key su,sf::Keyboard::Key jo,sf::Keyboard::Key st,sf::Keyboard::Key dr,sf::Keyboard::Key act)
{
    action=act;
    sus=su;
    jos=jo;
    stanga=st;
    dreapta=dr;
}

void player::deplasare(bloc blocuri[],int numar_blocuri,bloc blocuri_masina[],int numar_blocuri_masina,float dt)
{
    int ok=1;
    int propulsie=0;
    forma.move(0,1);
    for (int i=1; i<=numar_blocuri; i++)
    {
        if (forma.getGlobalBounds().intersects(blocuri[i].forma.getGlobalBounds())&&blocuri[i].alive&&blocuri[i].killer==0)
        {
            ok=0;
            break;
        }
    }
    for (int i=1; i<=numar_blocuri_masina; i++)
    {
        if (forma.getGlobalBounds().intersects(blocuri_masina[i].forma.getGlobalBounds())&&blocuri_masina[i].alive)
        {
            blocuri_masina[i].blocat=1;
            if (blocuri_masina[i].tip==1)
                blocuri_masina[i].forma.setTexture(&bloc_blocat);
            if (blocuri_masina[i].tip==2)
                blocuri_masina[i].forma.setTexture(&bloc_propulsie_blocat);

            if (blocuri_masina[i].propulsie!=0)
                propulsie=blocuri_masina[i].propulsie;
            ok=0;
            break;
        }
    }
    forma.move(0,-1);
    if (ok==0)
        marja_eroare_saritura=50000;
    if (marja_eroare_saritura>0)
        ok=0;
    if (sf::Keyboard::isKeyPressed(sus))
    {
        if (ok==0&&cooldown_saritura==0)
        {
            viteza_y=-putere_saritura*coeficient_saritura;
                saritura.play();
            cooldown_saritura=300000;
        }
        if (propulsie!=0)
            viteza_y=-propulsie*coeficient_propulsie;
        sare=0;
    }
    if (sf::Keyboard::isKeyPressed(jos))
    {
        if (ok==0&&sta_jos==0)
        {
            sta_jos=1;
            forma.setSize(sf::Vector2f(forma.getSize().x,forma.getSize().y-25*((float)hight/768)));
            forma.move(sf::Vector2f(0,25*((float)hight/768)));
        }
    }
    if (sf::Keyboard::isKeyPressed(dreapta))
    {
        if (sta_jos)
            viteza_x=1*((float)width/1366);
        else
            viteza_x=viteza_player*((float)width/1366);
        orientare_dreapta=1;
    }
    if (sf::Keyboard::isKeyPressed(stanga))
    {
        if (sta_jos)
            viteza_x=-1*((float)width/1366);
        else
            viteza_x=-viteza_player*((float)width/1366);
        orientare_dreapta=0;
    }

    if (sf::Keyboard::isKeyPressed(action))
    {

    }
}
