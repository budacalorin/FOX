#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include "include/bloc.h"
#include "include/init.h"
#include "include/scena_joc.h"
#include "include/scena_meniu.h"
#include "include/initializari.h"
#include <fstream>
#include <cstring>

using namespace std;
ifstream fin;
ofstream fout;


scena_meniu meniu_principal;
scena_joc joc_principal;

int stele[20];
void init()
{

    int i=1;
    unsigned int x;
    fin.open("scor.txt");

    fin >> tutorial;
    fin >> FPS;
    if (!FPS||FPS%10!=0)
        FPS=60;

    fin >> indice_rezolutie;
    if (indice_rezolutie==0)
        indice_rezolutie=1;

    fin >> audio;
    if (audio==0)
    {
        saritura.setVolume(0);
        atezirare.setVolume(0);
        music.setVolume(0);
        adaugare_bloc_sound.setVolume(0);
        sunet_coin.setVolume(0);
        music_cave.setVolume(0);
    }

     x=0;
    for (i=1; i<=19; i++)
    {
        int y=0;
        fin >>x>>y;
        meniu_principal.numar_stele[i]=joc_principal.numar_maxim_stele[i]=x;
        meniu_principal.coins[i]=joc_principal.coins[i]=y;
    }
    fin.close();

    background_texture.loadFromFile("img/endings/level_1.png");

    char y[3];
    y[0]='-';
    y[1]=abs(tutorial)+'0';
    y[2]=(char)NULL;
    if (tutorial<=0)
    {
        meniu_principal.level=tutorial;
        char s[30]= {0};
        strcpy(s,"img/endings/level_");
        strcat(s,y);
        strcat(s,".png");
        background_texture.loadFromFile(s);
    }
    else
        meniu_principal.level=1;
    meniu_principal.level_last_update=-4;

    textura_palete.loadFromFile("img/paleta_1.png");
    textura_palete.setRepeated(1);
    fox.loadFromFile("img/fox.png");

    p_background_texture=&background_texture;
    background.setTexture(p_background_texture);
    background.setSize(sf::Vector2f(1366*((float)1366/width),768*((float)768/hight)));

    meniu_principal.adaugare_optiune("PLAY");
    meniu_principal.adaugare_optiune("LEVELS");
    meniu_principal.adaugare_optiune("OPTIONS");
    meniu_principal.adaugare_optiune("EXIT");
    meniu_principal.adaugare_setup(720,480,0.9,0.7,3.9,3.9,0.79,0.79,-8,0,0);
    meniu_principal.adaugare_setup(1024,768,0.9,0.7,3.1,3.1,1,1,-13,0,0);
    meniu_principal.adaugare_setup(1280,960,0.9,0.7,2.65,2.65,1.11,1.11,-15,0,0);
    meniu_principal.adaugare_setup(1366,768,0.9,0.70,3,3,1,1,-13,0,0);
    meniu_principal.adaugare_setup(1680,1050,0.9,0.7,2.55,2.55,1.16,1.16,-16,0,0);
    meniu_principal.adaugare_setup(2732,1536,0.9,0.7,2.1,3,1.405,1.405,-25,0,0);

    meniu_principal.adaugare_fps_setup(20);
    meniu_principal.adaugare_fps_setup(30);
    meniu_principal.adaugare_fps_setup(40);
    meniu_principal.adaugare_fps_setup(50);
    meniu_principal.adaugare_fps_setup(60);
    meniu_principal.adaugare_fps_setup(80);
    meniu_principal.adaugare_fps_setup(100);
    meniu_principal.adaugare_fps_setup(120);
    meniu_principal.adaugare_fps_setup(150);
    meniu_principal.adaugare_fps_setup(200);
    meniu_principal.adaugare_fps_setup(240);

    meniu_principal.setup=4;
    meniu_principal.numar_levele=14;
    meniu_principal.tip_meniu=1;

    textura_masina.loadFromFile("img/masina_femeie_ufo.png");

    textura_bloc.loadFromFile("img/bloc_sky.png");
    bloc_blocat.loadFromFile("img/bloc_sky_blocat.png");
    textura_bloc_propulsie.loadFromFile("img/bloc_sky_propulsie.png");
    bloc_propulsie_blocat.loadFromFile("img/bloc_sky_propulsie_blocat.png");

    masina_animatie.loadFromFile("img/masina_femeie_ufo.png");

    p_textura_masina=&textura_masina;
    p_textura_blocuri_ramase=&numar_blocuri_ramase.textura;

    cer.setTexture(&textura_cer);
    cer.setSize(sf::Vector2f(width,hight));

    textura_stea.loadFromFile("img/stea.png");
    textura_stea_goala.loadFromFile("img/stea_goala.png");
    sf::Image imagine;

    music.openFromFile("audio/No More Magic.ogg");
    music_cave.openFromFile("audio/cave theme.ogg");
    music.setLoop(true);
    music_cave.setLoop(true);

    saritura_buffer.loadFromFile("audio/jump.wav");
    saritura.setBuffer(saritura_buffer);
    //saritura.setVolume(20);

    adaugare_bloc_buffer.loadFromFile("audio/masina.ogg");
    adaugare_bloc_sound.setBuffer(adaugare_bloc_buffer);

    aterizare_buffer.loadFromFile("audio/jumpland.wav");
    atezirare.setBuffer(aterizare_buffer);

    textura_coin.loadFromFile("img/coin.png");
    textura_single_coin.loadFromFile("img/single coin.png");
    textura_chenar_coin.loadFromFile("img/chenar coin.png");

    slash.loadFromFile("img/numere/slash.png");

    buffer_coin.loadFromFile("audio/coin luat.ogg");
    sunet_coin.setBuffer(buffer_coin);


    //window.setMouseCursorVisible(0);

    meniu_principal.init_setups(indice_rezolutie);

    string_font="DejaVuSans.ttf";
}

sf::RenderWindow window(sf::VideoMode(width,hight),"",sf::Style::None);
sf::Clock ceas_general;

int main()
{
    init();
    sf::Clock ceas;
    sf::Clock c;
    int fps=0;
    timp_per_frame=1000000/FPS;


    if (audio)
        music.play();

    while (game_on)
    {
        int timp=ceas.getElapsedTime().asMicroseconds();

        if (scena_curenta==0)
        {
            joc_principal.copiere_date(meniu_principal.numar_stele,meniu_principal.coins);
            meniu_principal.update_background();
            meniu_principal.draw();
            scena_curenta=1;
        }
        if (scena_curenta==1)
        {
            meniu_principal.update();
        }
        if (scena_curenta==3)
        {

            joc_principal.reset(meniu_principal.level,meniu_principal.numar_stele,meniu_principal.coins);
            scena_curenta=2;
        }
        if (scena_curenta==2)
        {
            joc_principal.update();
            meniu_principal.level=joc_principal.level;
        }


        fps++;
        if (c.getElapsedTime().asMicroseconds()>=1000000)
        {

            c.restart();
            cout << fps << endl;
            fps=0;
        }

        if (ceas.getElapsedTime().asMicroseconds()-timp<timp_per_frame)
        {
            sf::sleep(sf::microseconds(timp_per_frame-(ceas.getElapsedTime().asMicroseconds()-timp)));
            ceas.restart();
        }
    }

        /*cout << "indice_rezolutie="<<indice_rezolutie<<endl;
        cout << "width="<<width<<endl;
        cout << "hight="<<hight<<endl;
        cout << "coeficient_frecare="<<coeficient_frecare<<endl;
        cout << "gravitatie="<<gravitatie<<endl;
        cout << "coeficient_propulsie="<<coeficient_propulsie<<endl;
        cout << "viteza_player="<<viteza_player<<endl;
        cout << "viteza_masina="<<viteza_masina<<endl;
        cout << "coeficient_saritura="<<coeficient_saritura<<endl;
        cout << "sprite_offset="<<sprite_offset<<endl;*/


    fout.open("scor.txt");
    tutorial=meniu_principal.level;
    fout << tutorial << endl;
    fout << FPS<<endl;
    fout <<indice_rezolutie<<endl;
    fout << audio << endl;

    for (int i=1; i<=19; i++)
    {
        fout <<meniu_principal.numar_stele[i]<<" "<<meniu_principal.coins[i] << endl;
    }
    return 0;
}
