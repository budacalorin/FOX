#include "scena_pop_up.h"
#include <iostream>
using namespace std;
void scena_pop_up::init_stele_pop_up(const sf::Image imagine,int numar_stele)
{
    fundal=imagine;
    tip=1;
    stele=numar_stele;
    timp_precedent=0;
    blur.setFillColor(sf::Color(100,100,100,0));
}

void scena_pop_up::init_text(const sf::Image imagine,char txt[350])
{
    fundal=imagine;
    strcpy(text,txt);
    if (strlen(text)>60&&strstr(text,"\n")==0)
    {
        char s[100]= {0};
        unsigned int l=strlen(text);
        char *p=strtok(text," ");
        bool ok=1;
        while (p)
        {
            strcat(s,p);
            strcat(s," ");
            if (ok==1&&strlen(s)>=l/2)
            {
                ok=0;
                strcat(s,"\n");
            }
            p=strtok(NULL," ");
        }
        strcpy(text,s);
    }
    tip=2;
    blur.setFillColor(sf::Color(100,100,100,0));
    timp_precedent=0;
}


void scena_pop_up::update(int dt)
{
    //
    //cout << dt << endl;
    countdown-=dt;
    timp_precedent+=dt;
    if (blur.getFillColor().a<150)
    {
        while (timp_precedent>fade_time/15)
        {

            if (blur.getFillColor().a+10<=255)
                blur.setFillColor(sf::Color(blur.getFillColor().r,blur.getFillColor().g,blur.getFillColor().b,blur.getFillColor().a+10));
            else
                blur.setFillColor(sf::Color(blur.getFillColor().r,blur.getFillColor().g,blur.getFillColor().b,255));
            timp_precedent-=fade_time/15;
        }
    }
    if (countdown<=0)
    {

        countdown=0;
    }
    draw();
}

void scena_pop_up::draw()
{

    window.clear();
    blur.setSize(sf::Vector2f(width,hight));
    sf::RectangleShape Fundal;
    sf::Texture textura;
    textura.loadFromImage(fundal);
    Fundal.setTexture(&textura);
    Fundal.setSize(sf::Vector2f(width,hight));
    window.draw(Fundal);
    window.draw(blur);
    if (blur.getFillColor().a>=150)
    {
        if (tip==1)
        {
            int x,y;
            x=500*((float)width/1366);
            y=490*((float)hight/768);

            sf::RectangleShape stea;
            stea.setPosition(sf::Vector2f(x,y));
            stea.setSize(sf::Vector2f(100*((float)hight/768),100*((float)hight/768)));
            stea.setTexture(&textura_stea);
            int i;
            for ( i=1; i<=stele; i++)
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
            sf::Text press_key;
            press_key.setCharacterSize(40);
            press_key.setString("PRESS ENTER TO CONTINUE");
            press_key.setPosition(sf::Vector2f(355*((float)width/1366),590*((float)hight/768)));
            sf::Font font;
            font.loadFromFile(string_font);
            press_key.setFont(font);
            press_key.setOutlineColor(sf::Color::Black);
            press_key.setOutlineThickness(2);
            window.draw(press_key);
        }
        if (tip==2)
        {
            sf::Text textul;
            textul.setCharacterSize(40);
            textul.setString(text);
            if (strlen(text)>50)
                textul.setPosition(sf::Vector2f(((width/2)-(strlen(text))/2*12)*((float)width/1366),400*((float)hight/768)));
            else
                textul.setPosition(sf::Vector2f(((width/2)-(strlen(text))*12)*((float)width/1366),400*((float)hight/768)));



            sf::Font font;
            font.loadFromFile(string_font);
            textul.setFont(font);
            textul.setOutlineColor(sf::Color::Black);
            textul.setOutlineThickness(2);
            textul.setStyle(sf::Text::Style::Bold);

            //cout << endl <<textul.getGlobalBounds().width <<endl;
            std::string w=textul.getString();
            int l=w.size();
            int aux=l;
            for (int i=0;i<aux;i++)
            {
                if (w[i]==' ')
                    l--;
            }

            if (l>width/textul.getCharacterSize())
            {
                l=w.size();
                int numar_impartiri=l/(width/textul.getCharacterSize());
                if (l%(width/textul.getCharacterSize())!=0)
                    numar_impartiri++;
                int sectiune_curenta=1;
                int sectiuni=1;
                for (int i=0; i<l; i++)
                {
                    if (w[i]==' ')
                    {
                        if (i>=l/numar_impartiri*sectiune_curenta&&sectiuni<numar_impartiri)
                        {
                            w[i]='\n';
                            sectiune_curenta++;
                            sectiuni++;
                        }
                    }
                }
                textul.setString(w);
            }

            int a=textul.getGlobalBounds().left-(width-textul.getGlobalBounds().width-textul.getGlobalBounds().left);

            while (abs(a)>10)
            {
                textul.move(-5*(a/abs(a)),0);
                a=textul.getGlobalBounds().left-(width-textul.getGlobalBounds().width-textul.getGlobalBounds().left);
            }
            sf::Text press_key;
            press_key.setCharacterSize(40);
            press_key.setString("PRESS ENTER TO CONTINUE");
            press_key.setPosition(sf::Vector2f(355*((float)width/1366),590*((float)hight/768)));
            press_key.setFont(font);
            press_key.setOutlineColor(sf::Color::Black);
            press_key.setOutlineThickness(2);
            a=press_key.getGlobalBounds().left-(width-press_key.getGlobalBounds().width-press_key.getGlobalBounds().left);

            while (abs(a)>10)
            {
                press_key.move(-5*(a/abs(a)),0);
                a=press_key.getGlobalBounds().left-(width-press_key.getGlobalBounds().width-press_key.getGlobalBounds().left);
            }
            while (press_key.getGlobalBounds().intersects(textul.getGlobalBounds()))
                textul.move(sf::Vector2f(0,-10));

            window.draw(textul);
            window.draw(press_key);
        }
    }

    window.display();
}

void scena_pop_up::fade_out()
{
    int timp;
    int final_frame=0;
    int timp_precedent=0;
    while (blur.getFillColor().a>0)
    {
        timp=ceas_general.getElapsedTime().asMicroseconds();
        timp_precedent+=final_frame;
        while (timp_precedent>fade_time/15)
        {
            if (blur.getFillColor().a-10>=0)
                blur.setFillColor(sf::Color(blur.getFillColor().r,blur.getFillColor().g,blur.getFillColor().b,blur.getFillColor().a-10));
            else
                blur.setFillColor(sf::Color(blur.getFillColor().r,blur.getFillColor().g,blur.getFillColor().b,0));
            timp_precedent-=fade_time/15;
        }
        draw();
        final_frame=ceas_general.getElapsedTime().asMicroseconds()-timp;

    }
}
