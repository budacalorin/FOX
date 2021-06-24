#include <SFML/Graphics.hpp>
#include "../include/player.h"
#include "../include/proiectil.h"
#include "../include/particula.h"
#include "../include/init.h"
#include <cmath>
#include <cstring>
#include <iostream>

bool coliziune_player(sf::RectangleShape forma,bloc blocuri[],int numar_blocuri,bloc blocuri_masina[],int numar_blocuri_masina)
{
    for (int i=1; i<=numar_blocuri; i++)
        if (forma.getGlobalBounds().intersects(blocuri[i].forma.getGlobalBounds())&&blocuri[i].alive&&blocuri[i].killer==0)
        {
            return 1;
        }
    for (int i=1; i<=numar_blocuri_masina; i++)
        if (forma.getGlobalBounds().intersects(blocuri_masina[i].forma.getGlobalBounds())&&blocuri_masina[i].alive&&blocuri_masina[i].killer==0)
        {
            return 1;
        }
    return 0;
}


void coliziune_orizontala(sf::RectangleShape &forma,bloc blocuri[],int numar_blocuri,bloc blocuri_masina[],int numar_blocuri_masina,float &viteza_x)
{
    int ok=1;
    while (coliziune_player(forma,blocuri,numar_blocuri,blocuri_masina,numar_blocuri_masina)&&viteza_x)
    {
        if (ok)
        {
            ok=0;
            forma.move(sf::Vector2f(-(forma.getPosition().x-(int)forma.getPosition().x),0));
        }
        else
        {
            forma.move(sf::Vector2f(-viteza_x/abs(viteza_x),0));
        }
    }
    if (ok==0)
    {
        viteza_x=0;
    }
}

void coliziune_verticala(sf::RectangleShape &forma,bloc blocuri[],int numar_blocuri,bloc blocuri_masina[],int numar_blocuri_masina,float &viteza_y)
{
    int ok=1;
    while (coliziune_player(forma,blocuri,numar_blocuri,blocuri_masina,numar_blocuri_masina)&&viteza_y)
    {
        if (ok)
        {
            ok=0;
            forma.move(sf::Vector2f(0,-(forma.getPosition().y-(int)forma.getPosition().y)));
        }
        else
        {
            forma.move(sf::Vector2f(0,-viteza_y/abs(viteza_y)));
        }
    }
    if (ok==0)
    {
        if (viteza_y>5)
            atezirare.play();
        viteza_y=0;
    }
}

void schimbare_numar(int x,numar &numar_blocuri_ramas)
{
    if (numar_blocuri_ramas.valoare!=x)
    {
        numar_blocuri_ramas.valoare=x;
        //cout << x <<endl;
        char s[20]={0};
        strcpy(s,"img/numere/");
        char aux[2];
        aux[0]=x%10+'0';
        aux[1]=(char)NULL;
        strcat(s,aux);
        strcat(s,".png");
        numar_blocuri_ramas.textura.loadFromFile(s);
        if (x>=10)
        {
            //cout << x<< endl;
            strcpy(s,"img/numere/");
            aux[0]=x/10+'0';
            strcat(s,aux);
            strcat(s,".png");
            numar_blocuri_ramas.zece.loadFromFile(s);
        }

    }

}

void adaugare_particula(int x,int y,float viteza_x,float viteza_y,int unghi,int countdown,particula particule[],int &numar_particule,sf::Color culoare=sf::Color(100,100,100))
{
    for (int i=1; i<=numar_particule; i++)
    {
        if (particule[i].alive()==0)
        {
            particule[i].setup(x,y,viteza_x,viteza_y,unghi,countdown,culoare);
            particule[i].hp=50;
            return;
        }
    }
    particule[++numar_particule].setup(x,y,viteza_x,viteza_y,unghi,countdown,culoare);
    particule[numar_particule].hp=50;
}

