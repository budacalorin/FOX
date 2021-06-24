#include "../include/scena_meniu.h"
#include <cstring>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../include/init.h"
using namespace std;

void schimbare_numar(int x,numar &numar_blocuri_ramas);

void scena_meniu::adaugare_optiune(const char *s)
{
    strcpy(optiuni[++numar_optiuni],s);
}

bool scena_meniu::input_tastatura()
{
    bool ret=0;
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type==sf::Event::KeyPressed)
        {
            if (event.key.code==sf::Keyboard::Up)
            {
                up=1;
                down=0;
                ret=1;
            }

            if (event.key.code==sf::Keyboard::Down)
            {
                up=0;
                down=1;
                ret=1;
            }

            if (event.key.code==sf::Keyboard::Enter)
            {
                enter=1;
                ret=1;
            }
            if (event.key.code==sf::Keyboard::Left)
            {
                left=1;
                right=0;
                ret=1;
            }
            if (event.key.code==sf::Keyboard::Right)
            {
                right=1;
                left=0;
                ret=1;
            }

        }

        if (event.type==sf::Event::Closed)
        {
            game_on=0;
        }
    }
    return ret;
}

void scena_meniu::deplasare()
{
    if (up==1)
    {
        optiune_curenta--;
        if (strstr(optiuni[optiune_curenta],"RESET")&&level<=0)
            optiune_curenta--;
        if (strstr(optiuni[optiune_curenta],"LEVELS")&&level<=0)
            optiune_curenta--;
        if(optiune_curenta==0)
            optiune_curenta=1;
        up=0;
    }
    if (down==1)
    {
        optiune_curenta++;
        if (strstr(optiuni[optiune_curenta],"LEVELS")&&level<=0)
            optiune_curenta++;
        if (strstr(optiuni[optiune_curenta],"RESET")&&level<=0)
            optiune_curenta++;
        if (optiune_curenta==numar_optiuni+1)
            optiune_curenta=numar_optiuni;
        down=0;
    }
    if (enter==1)
    {
        actiune();
        enter=0;
    }
    if (left)
    {

        if (level>1&&strstr(optiuni[optiune_curenta],"CHOSE"))
        {
            level--;
        }

        if (setup>1&&strstr(optiuni[optiune_curenta],"RESOLUTION"))
        {
            setup--;
        }

        if (indice_fps_curent>1&&strstr(optiuni[optiune_curenta],"FPS"))
        {
            indice_fps_curent--;
        }
        if (audio_status==1&&strstr(optiuni[optiune_curenta],"AUDIO"))
        {
            audio_status=0;
        }
        left=0;
    }
    if (right)
    {

        if (level<numar_levele&&strstr(optiuni[optiune_curenta],"CHOSE"))
        {
            level++;
        }

        if (setup<numar_setups&&strstr(optiuni[optiune_curenta],"RESOLUTION"))
        {
            setup++;
        }

        if (indice_fps_curent<numar_fps_setups&&strstr(optiuni[optiune_curenta],"FPS"))
        {
            indice_fps_curent++;
        }
        if (audio_status==0&&strstr(optiuni[optiune_curenta],"AUDIO"))
        {
            audio_status=1;
        }

        right=0;
    }
}

void scena_meniu::draw()
{
    window.clear();
    window.draw(background);
    afisare_optiuni();

    //if (tip_meniu==3)
    //draw_stele();

    window.display();
}

void scena_meniu::afisare_optiuni()
{
    int x_optiuni=15;
    int y_optiuni=hight-80*(float)hight/768-43*(float)hight/768;

    for (int i=numar_optiuni; i>=1; i--)
    {
        sf::Text text;
        if (!((strstr(optiuni[i],"LEVELS")||(strstr(optiuni[i],"RESET")))&&level<=0))
        {
            text.setString(optiuni[i]);

            if(strstr(optiuni[i],"CHOSE"))
            {
                char x[15]="LEVEL ";
                char n[3];
                if (level<0)
                {
                    n[0]='-';
                    n[1]='0'+abs(level);
                    n[2]=(char)NULL;
                }
                if (level>=0)
                {
                    n[0]='0'+level;
                    n[1]=(char)NULL;
                }
                if (level>=10)
                {
                    n[0]='0'+level/10;
                    n[1]='0'+level%10;
                    n[2]=(char)NULL;
                }

                strcat(x,n);
                text.setString(x);
            }
            if(strstr(optiuni[i],"RESOLUTION"))
            {
                char x[30]="RESOLUTION: ";
                char n[30]= {0};

                int p=1;
                int aux=setups[setup][0];
                while (aux)
                {
                    aux/=10;
                    p*=10;
                }
                p/=10;
                aux=setups[setup][0];
                int i=0;
                while (p!=0)
                {
                    n[i]=aux/p%10+'0';
                    p/=10;
                    i++;
                }
                n[i]='X';
                i++;
                p=1;
                aux=setups[setup][1];
                while (aux)
                {
                    aux/=10;
                    p*=10;
                }
                p/=10;
                aux=setups[setup][1];
                while (p!=0)
                {
                    n[i]=aux/p%10+'0';
                    p/=10;
                    i++;
                }
                n[i]=(char)NULL;

                strcat(x,n);
                text.setString(x);
            }
            if(strstr(optiuni[i],"FPS"))
            {
                char x[30]="FPS: ";
                char n[10]= {0};
                int aux=fps_setups[indice_fps_curent];
                int p=1;
                while (aux)
                {
                    p*=10;
                    aux/=10;
                }
                p/=10;
                aux=fps_setups[indice_fps_curent];
                int i=0;
                while (p)
                {
                    n[i]=aux/p%10+'0';
                    p/=10;
                    i++;
                }
                n[i]=(char)NULL;
                strcat(x,n);
                text.setString(x);
            }
            if(strstr(optiuni[i],"AUDIO"))
            {
                char x[30]="AUDIO: ";
                if (audio_status==0)
                    strcat(x,"OFF");
                else
                    strcat(x,"ON");
                text.setString(x);
            }
            text.setCharacterSize (80*(float)width/1366);
            text.setPosition(sf::Vector2f(x_optiuni,y_optiuni));
            sf::Font font;
            font.loadFromFile(string_font);
            text.setFont(font);
            //text.setOutlineColor(sf::Color::Black);
            //text.setOutlineThickness(1);
            y_optiuni-=90*(float)hight/768;

            text.setFillColor(culoare_text);


            if (optiune_curenta==i)
            {
                text.setCharacterSize (85*(float)width/1366);
                text.setFillColor(culoare_text_selectat);

            }
            window.draw(text);
        }
    }

}

void scena_meniu::update()
{
    bool x=input_tastatura();
    if (x)
    {
        deplasare();
        update_background();
        draw();
        level_last_update=level;
    }
}

void scena_meniu::actiune()
{

    if (strstr(optiuni[optiune_curenta],"PLAY"))
    {
        scena_curenta=3;
        return;
    }
    if (strstr(optiuni[optiune_curenta],"EXIT"))
    {
        game_on=0;
        return;
    }
    if (strstr(optiuni[optiune_curenta],"RESET"))
    {
        optiune_curenta--;
        level=-3;
        for (int i=1; i<=19; i++)
            numar_stele[i]=coins[i]=0;
        for (int i=1; i<=numar_levele; i++)
        {
            char s[30]= {0};
            strcpy(s,"img/endings/level_");
            char n[3];
            if (i<=0)
            {
                n[0]='-';
                n[1]='0'+abs(i);
                n[2]=(char)NULL;
            }
            if (i>0)
            {
                n[0]='0'+abs(i);
                n[1]=(char)NULL;
            }
            if (i>=10)
            {
                n[0]='0'+i/10;
                n[1]='0'+i%10;
                n[2]=(char)NULL;
            }

            strcat(s,n);
            strcat(s,".png");

            char a[30]= {0};
            strcpy(a,"img/levels/level_");
            char o[3];
            if (i<=0)
            {
                o[0]='-';
                o[1]='0'+abs(i);
                o[2]=(char)NULL;
            }
            if (i>0)
            {
                o[0]='0'+abs(i);
                o[1]=(char)NULL;
            }
            if (i>=10)
            {
                o[0]='0'+i/10;
                o[1]='0'+i%10;
                o[2]=(char)NULL;
            }

            strcat(a,o);
            strcat(a,".png");
            sf::Image Imagine;
            Imagine.loadFromFile(a);
            Imagine.saveToFile(s);

        }

        return;
    }
    if (strstr(optiuni[optiune_curenta],"OPTIONS"))
    {
        reset();
        adaugare_optiune("RESOLUTION");
        adaugare_optiune("FPS");
        adaugare_optiune("AUDIO");
        adaugare_optiune("RESET");
        adaugare_optiune("BACK");
        optiune_curenta=5;
        tip_meniu=2;
        return;
    }
    if (strstr(optiuni[optiune_curenta],"BACK"))
    {
        reset();
        adaugare_optiune("PLAY");
        adaugare_optiune("LEVELS");
        adaugare_optiune("OPTIONS");
        adaugare_optiune("EXIT");
        if (tip_meniu==2)
            optiune_curenta=3;
        if (tip_meniu==3)
            optiune_curenta=2;
        tip_meniu=1;
        return;
    }
    if (strstr(optiuni[optiune_curenta],"CHOSE"))
    {
        reset();
        adaugare_optiune("PLAY");
        adaugare_optiune("LEVELS");
        adaugare_optiune("OPTIONS");
        adaugare_optiune("EXIT");
        if (tip_meniu==2)
            optiune_curenta=3;
        if (tip_meniu==3)
            optiune_curenta=2;
        tip_meniu=1;
        return;
    }
    if (strstr(optiuni[optiune_curenta],"RESOLUTION"))
    {
        indice_rezolutie=setup;
        width=setups[setup][0];
        hight=setups[setup][1];
        coeficient_frecare=setups[setup][2];
        gravitatie=setups[setup][3];
        coeficient_propulsie=setups[setup][4];
        viteza_player=setups[setup][5];
        viteza_masina=setups[setup][6];
        coeficient_saritura=setups[setup][7];
        sprite_offset=setups[setup][8];
        masina_offset_X=setups[setup][9];
        masina_offset_Y=setups[setup][10];


        window.create(sf::VideoMode(width,hight),"",sf::Style::None);
        background.setSize(sf::Vector2f(1366*((float)width/1366),768*((float)hight/768)));

        return;
    }

    if (strstr(optiuni[optiune_curenta],"FPS"))
    {
        FPS=fps_setups[indice_fps_curent];
        timp_per_frame=1000000/FPS;
        return;
    }
    if (strstr(optiuni[optiune_curenta],"AUDIO"))
    {
        if (audio_status==1&&audio==0)
        {
            music.play();
            saritura.setVolume(100);
            atezirare.setVolume(100);
            music.setVolume(100);
            adaugare_bloc_sound.setVolume(100);
            sunet_coin.setVolume(100);
            music_cave.setVolume(100);
        }
        if (audio_status==0&&audio==1)
        {
            music_cave.pause();
            music.pause();
            saritura.setVolume(0);
            atezirare.setVolume(0);
            music.setVolume(0);
            adaugare_bloc_sound.setVolume(0);
            sunet_coin.setVolume(0);

        }

        audio=audio_status;
        return;
    }
    if (strstr(optiuni[optiune_curenta],"LEVELS"))
    {
        reset();
        adaugare_optiune("CHOSE");
        adaugare_optiune("BACK");
        optiune_curenta=2;
        tip_meniu=3;
        return;
    }
}

void scena_meniu::update_background()
{
    if (level_last_update!=level)
    {
        char s[30]= {0};
        strcpy(s,"img/endings/level_");
        char n[3];
        if (level<=0)
        {
            n[0]='-';
            n[1]='0'+abs(level);
            n[2]=(char)NULL;
        }
        if (level>0)
        {
            n[0]='0'+abs(level);
            n[1]=(char)NULL;
        }
        if (level>=10)
        {
            n[0]='0'+level/10;
            n[1]='0'+level%10;
            n[2]=(char)NULL;
        }
        strcat(s,n);
        strcat(s,".png");
        background_texture.loadFromFile(s);





        int x_optiuni=15;
        int y_optiuni=hight-80*(float)hight/768-43*(float)hight/768;

        sf::Image img;
        img.loadFromFile(s);

        long sum=0;
        for (int q=x_optiuni; q<=x_optiuni+30; q++)
            for (int w=y_optiuni; w<=y_optiuni+30; w++)
            {
                sum+=img.getPixel(q,w).r+img.getPixel(q,w).g+img.getPixel(q,w).b;
                //cout << s<<endl;
            }
        if (sum<3*50*(30)*(30))
        {
            culoare_text=sf::Color(220,220,220);
            culoare_text_selectat=sf::Color(244, 217, 66);
        }
        else
        {
            culoare_text=sf::Color(220,220,220);
            culoare_text_selectat=sf::Color(244, 217, 66);
        }

        // background.setTexture(&background_texture);
    }
}

void scena_meniu::draw_stele()
{
    if (level>0)
    {
        int x,y;
        x=355*((float)width/1366);
        y=490*((float)hight/768);

        sf::RectangleShape stea;
        stea.setPosition(sf::Vector2f(x,y));
        stea.setSize(sf::Vector2f(40*((float)hight/768),40*((float)hight/768)));
        stea.setTexture(&textura_stea);
        unsigned int i;
        for ( i=1; i<=numar_stele[level]; i++)
        {
            window.draw(stea);
            stea.move(sf::Vector2f(stea.getSize().x+1,0));
        }
        stea.setTexture(&textura_stea_goala);

        for (int j=i; j<=3; j++)
        {
            window.draw(stea);
            stea.move(sf::Vector2f(stea.getSize().x+1,0));
        }
        int s=0;
        for (int i=1; i<=19; i++)
            s+=numar_stele[i];
        sf::RectangleShape text,text2;

        stea.setTexture(&textura_stea);
        stea.setPosition(sf::Vector2f(width-120*((float)width/1366),10*((float)hight/768)));
        stea.setSize(sf::Vector2f(100*((float)width/1366),100*((float)hight/768)));
        window.draw(stea);
        schimbare_numar(s,numar_blocuri_ramase);
        if (s<=9)
            text.setPosition(sf::Vector2f(width-90*((float)width/1366),100*((float)hight/768)));
        else
            text.setPosition(sf::Vector2f(width-80*((float)width/1366),100*((float)hight/768)));
        text.setSize(sf::Vector2f(40*((float)width/1366),40*((float)hight/768)));
        text2.setPosition(sf::Vector2f(width-100*((float)width/1366),100*((float)hight/768)));
        text2.setSize(sf::Vector2f(40*((float)width/1366),40*((float)hight/768)));
        text.setFillColor(sf::Color::White);
        text.setTexture(&numar_blocuri_ramase.textura);
        text2.setTexture(&numar_blocuri_ramase.zece);
        window.draw(text);
        if (s>=10)
            window.draw(text2);
    }
}

void scena_meniu::reset()
{
    numar_optiuni=0;

}

void scena_meniu::adaugare_setup (int x,int y,float forta_frecare,float gravitatia,float viteza_playerul,float viteza_masinei,float propulsie,float coeficient_sarituri,int sprite_offsett,int masina_offset_x,int masina_offset_y)
{

    setups[++numar_setups][0]=x;
    setups[numar_setups][1]=y;
    setups[numar_setups][2]=forta_frecare;
    setups[numar_setups][3]=gravitatia;
    setups[numar_setups][4]=propulsie;
    setups[numar_setups][5]=viteza_playerul;
    setups[numar_setups][6]=viteza_masinei;
    setups[numar_setups][7]=coeficient_sarituri;
    setups[numar_setups][8]=sprite_offsett;
    setups[numar_setups][9]=masina_offset_x;
    setups[numar_setups][10]=masina_offset_y;
}

void scena_meniu::init_setups(int ind)
{
    int ok=0;
    if (width!=setups[ind][0]||hight!=setups[ind][1])
        ok=1;
    width=setups[ind][0];
    hight=setups[ind][1];
    coeficient_frecare=setups[ind][2];
    gravitatie=setups[ind][3];
    coeficient_propulsie=setups[ind][4];
    viteza_player=setups[ind][5];
    viteza_masina=setups[ind][6];
    coeficient_saritura=setups[ind][7];
    sprite_offset=setups[ind][8];
    masina_offset_X=setups[ind][9];
    masina_offset_Y=setups[ind][10];
    setup=ind;
    if (ok)
    {
        window.create(sf::VideoMode(width,hight),"",sf::Style::None);
        background.setSize(sf::Vector2f(1366*((float)width/1366),768*((float)hight/768)));
    }
    for (int i=1; i<=numar_fps_setups; i++)
        if (fps_setups[i]==FPS)
        {
            indice_fps_curent=i;
            break;
        }
    audio_status=audio;
}

void scena_meniu::adaugare_fps_setup(int x)
{
    fps_setups[++numar_fps_setups]=x;
}
