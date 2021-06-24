#include "../include/scena_joc.h"
#include "../include/init.h"
#include "../include/time.h"
#include <fstream>
#include <iostream>
#include "scena_pop_up.h"
void coliziune_verticala(sf::RectangleShape &forma,bloc blocuri[],int numar_blocuri,bloc blocuri_masina[],int numar_blocuri_masina,float &viteza_y);
void coliziune_orizontala(sf::RectangleShape &forma,bloc blocuri[],int numar_blocuri,bloc blocuri_masina[],int numar_blocuri_masina,float &viteza_x);
void adaugare_particula(int x,int y,float viteza_x,float viteza_y,int unghi,int countdown,particula particule[],int &numar_particule,sf::Color culoare=sf::Color(100,100,100));
void schimbare_numar(int x,numar &numar_blocuri_ramas);
void scena_joc::update()
{
    sf::Mouse mouse;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        playerul.forma.setPosition(sf::Vector2f(mouse.getPosition(window).x,mouse.getPosition(window).y));

    int x;
    x=update_cooldown();
    update_moristi(x);
    update_turn();
    playerul.deplasare(blocuri,numar_blocuri,blocuri_masina,numar_blocuri_masina,x);
    bool moare=0;

    moare=playerul.miscare(blocuri,numar_blocuri,blocuri_masina,numar_blocuri_masina,x);
    coliziuni(moare);
    if (moare)
    {
        for (int i=1; i<=100; i++)
        {
            adaugare_particula(playerul.forma.getPosition().x+rand()%(int)playerul.forma.getSize().x,playerul.forma.getPosition().y+rand()%(int)playerul.forma.getSize().y,2.f*((float)(rand()%100)/100),2.f*((float)(rand()%100)/100),rand()%180,200000,particule,numar_particule,sf::Color(rand()%255,0,0));
        }
        revive();
    }
    update_playeri(x);

    masina.miscare(blocuri,numar_blocuri,x);
    actiune_masina();
    space_apasat=0;

    update_events();
    update_animatii(x);
    update_stele();
    update_particule(x);
    update_coin(x);
    update_plasma_guns(x);
    update_blocuri();

    draw();
    verificare_final();
}

void scena_joc::draw()
{
    window.clear();

    window.draw(cer);
    window.draw(background);
    draw_blocuri();
    draw_numar_blocuri_ramase();
    draw_stele();
    draw_particule();
    draw_morisci();
    draw_coin();
    draw_plasma_guns();

    if ((e_in_camera_coin==1||level<6)&&e_coin==1&&coin_colectat==0)
    {
        window.draw(coinul.forma);
    }

    window.draw(masina.sprite);
    //window.draw(masina.forma);

    window.draw(playerul.sprite);
    //window.draw(playerul.forma);

    /*sf::View view;
    view.setSize(sf::Vector2f(width,hight));
    view.setCenter(playerul.forma.getPosition().x+playerul.forma.getSize().x/2,playerul.forma.getPosition().y+playerul.forma.getSize().y);
    window.setView(view);*/

    window.display();
}

void scena_joc::draw_blocuri()
{

    for (int i=1; i<=numar_blocuri; i++)
    {
        if (blocuri[i].alive==1)
        {
            window.draw(blocuri[i].forma);
        }
    }
    for (int i=1; i<=numar_blocuri_masina; i++)
    {
        if (blocuri_masina[i].alive)
        {
            window.draw(blocuri_masina[i].forma);
        }
    }

}

void scena_joc::update_playeri(float dt)
{
    if (playerul.forma.getPosition().y+playerul.forma.getSize().y>=hight+20)
        revive();
    if (playerul.forma.getPosition().y<=-playerul.forma.getSize().y&&level!=11)
        revive();
    if (playerul.forma.getPosition().x<=-playerul.forma.getSize().x)
        revive();

    if (playerul.forma.getPosition().x+playerul.forma.getSize().x>=width+20||(playerul.forma.getPosition().y<=-playerul.forma.getSize().x&&level==11))
    {
        if (level>=1)
            if (numar_maxim_stele[level]<=numar_stele[level])
            {
                numar_maxim_stele[level]=numar_stele[level];
            }

        if (level>0)
            pop_up_stele();

        if (level>=1&&numar_maxim_stele[level]<=numar_stele[level])
        {
            for (int i=1; i<=2; i++)
            {
                window.clear();

                window.draw(cer);
                window.draw(background);
                draw_blocuri();
                draw_numar_blocuri_ramase();
                draw_stele();
                draw_particule();
                draw_morisci();
                draw_coin();
                draw_plasma_guns();

                if ((e_in_camera_coin==1||level<6)&&e_coin==1&&coin_colectat==0)
                {
                    window.draw(coinul.forma);
                }

                //window.display();
            }



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

            sf::Texture t;
            t.create(width,hight);
            //sf::sleep(sf::seconds(1));

            t.update(window);
            sf::Image Imagine=t.copyToImage();


            Imagine.saveToFile(s);
        }

        numar_level++;
        reset_level();
        init_levels(numar_level);
        playerul.forma.setPosition(pozitie_player);
        masina.forma.setPosition(pozitie_masina);

    }

    sf::Color culoare(40,40,40);
    if (playerul.sta_jos==0&&playerul.viteza_y==0)
    {
        if (playerul.viteza_x>1.5||(playerul.viteza_x<0&&playerul.viteza_x>=-1.5))
        {
            //culoare=imagine.getPixel(playerul.forma.getPosition().x,playerul.forma.getPosition().y+playerul.forma.getSize().y+1);

            particule_timer+=dt/(16666*1);
            for (int i=1; i<=particule_timer; i++)
            {
                adaugare_particula(playerul.forma.getPosition().x,playerul.forma.getPosition().y+playerul.forma.getSize().y,2.f*((float)(rand()%100)/100),1.f*((float)(rand()%100)/100),rand()%45+180-45,100000,particule,numar_particule,culoare);
            }
            particule_timer-=(int)particule_timer;
        }
        if (playerul.viteza_x<-1.5||(playerul.viteza_x>0&&playerul.viteza_x<=1.5))
        {
            //culoare=imagine.getPixel(playerul.forma.getPosition().x+playerul.forma.getSize().x+1,playerul.forma.getPosition().y+playerul.forma.getSize().y+1);
            particule_timer+=dt/(16666*1);
            for (int i=1; i<=particule_timer; i++)
            {
                particule_timer-=1;
                adaugare_particula(playerul.forma.getPosition().x+playerul.forma.getSize().x,playerul.forma.getPosition().y+playerul.forma.getSize().y,2.f*((float)(rand()%100)/100),1.f*((float)(rand()%100)/100),rand()%45,100000,particule,numar_particule,culoare);
            }
            particule_timer-=(int)particule_timer;
        }
    }

    if (atezirare.getStatus()==2&&aterizat==0)
    {
        //culoare=imagine.getPixel(playerul.forma.getPosition().x,playerul.forma.getPosition().y+playerul.forma.getSize().y+1);
        for (int i=1; i<=30; i++)
            adaugare_particula(playerul.forma.getPosition().x+rand()%(int)playerul.forma.getSize().x,playerul.forma.getPosition().y+playerul.forma.getSize().y,float(rand()%50)/100,(float)(rand()%50)/100,rand()%180,300000,particule,numar_particule,culoare);
        aterizat=1;
    }
    if (atezirare.getStatus()==0)
        aterizat=0;

    if (level==-3)
    {
        if (playerul.forma.getPosition().x>=0&&trigers[1]==1)
        {
            if (trigers[3]==1)
            {
                trigers[3]=0;
            }
            else
            {
                if (trigers[2]==1)
                {
                    trigers[2]=0;
                }
                else
                {
                    trigers[1]=0;
                    char txt[]= {"MOVE WITH  W A S D  AND WITH THE ARROWS"};
                    pop_up_text(txt);

                }
            }
        }
    }
    if (level==-2)
    {
        if (playerul.forma.getPosition().x>=1000&&trigers[1]==1)
        {
            trigers[1]=0;
            char txt[]= {"PRESS DOWN TO CROUCH"};
            pop_up_text(txt);

        }
    }
    if (level==-1)
    {
        if (playerul.forma.getPosition().x>=0&&trigers[1]==1)
        {
            if (trigers[3]==1)
            {
                trigers[3]=0;
            }
            else
            {
                if (trigers[2]==1)
                {
                    trigers[2]=0;
                }
                else
                {
                    trigers[1]=0;
                    char txt[]= {"YOU CAN RESET BY PRESSING R"};
                    pop_up_text(txt);

                }
            }

        }
    }
    if (level==0)
    {
        if (playerul.forma.getPosition().x>=0&&trigers[1]==1)
        {
            if (trigers[3]==1)
            {
                trigers[3]=0;
            }
            else
            {
                if (trigers[2]==1)
                {
                    trigers[2]=0;
                }
                else
                {
                    trigers[1]=0;
                    char txt[]= {"YOU CAN PRESS SPACEBAR TO PLACE BLOCKS"};
                    pop_up_text(txt);
                    char txt2[]= {"YOU CAN ERASE BLOCKS BY PRESSING SPACE AGAIN"};
                    pop_up_text(txt2);
                }
            }
        }
        if (playerul.forma.getPosition().x>=400&&trigers[4]==1)
        {
            trigers[4]=0;
            char txt[]= {"HOLD SHIFT TO ENHANCE PRECISION"};
            pop_up_text(txt);
        }
        if (playerul.forma.getPosition().x>=600&&trigers[5]==1)
        {
            trigers[5]=0;
            char txt[]= {"YOU CAN CHANGE THE BLOCK TYPE BY PRESSING 1 AND 2"};
            pop_up_text(txt);
        }
    }
    if (level==1)
    {
        if (playerul.forma.getPosition().x>=0&&trigers[1]==1)
        {
            if (trigers[3]==1)
            {
                trigers[3]=0;
            }
            else
            {
                if (trigers[5]==1)
                {
                    trigers[5]=0;
                }
                else
                {
                    if (trigers[2]==1)
                    {
                        trigers[2]=0;
                    }
                    else
                    {
                        trigers[1]=0;
                        char txt[]= {"HELP FOX REACH THE END BY PLACING BLOCKS"};
                        pop_up_text(txt);
                        char txt2[]= {"GET AS MANY STARS AS POSSIBLE, USE AS FEW BLOCKS AS POSSIBLE"};
                        pop_up_text(txt2);

                    }
                }
            }
        }
    }
    if (level==14)
    {
        if (playerul.forma.getPosition().x>(float)width*3/4)
        {
            char txt[]={"CONGRATULATIONS, YOU WON!"};
            pop_up_text(txt);
            scena_curenta=0;
        }
    }
}

void scena_joc::verificare_final()
{
}

void scena_joc::reset(int x,unsigned int numar_stele_meniu[20],bool coinuri[20])
{
    numar_playeri=1;
    numar_blocuri=0;

    scale_x=(float)width/1366;
    scale_y=(float)hight/768;
    numar_blocuri_masina=0;
    numar_particule=0;
    numar_morisci=0;
    turn=1;
    playerul.set_positioin(0,0);
    playerul.set_size((32*(float)width/1366),60*((float)hight/768));
    playerul.set_texture(NULL);
    playerul.forma.setFillColor(sf::Color::Blue);
    playerul.setTaste(sf::Keyboard::Up,sf::Keyboard::Down,sf::Keyboard::Left,sf::Keyboard::Right,sf::Keyboard::Space);
    playerul.alive=1;
    playerul.sta_jos=0;
    playerul.hp=hp_player;
    playerul.forma.setTexture(NULL);
    playerul.sprite.setTexture(fox);
    playerul.sprite.setScale(((float)width/1366),((float)hight/768));

    masina.animatia.setup(&masina_animatie,sf::Vector2u(8,2),50000);
    masina.animatia.curent_image.y=1;
    playerul.animatia.setup(&fox,sf::Vector2u(8,6),100000);

    //masina.animatia.image_count.x=1;
    //masina.animatia.curent_image.x=0;

    coin_scor.animatia.setup(&textura_coin,sf::Vector2u(61,1),50000);
    coin_scor.setup(sf::Vector2f(width-(154-24)*((float)width/1366),(160+24)*((float)hight/768)));
    coin_scor.forma.setSize(sf::Vector2f(40*scale_x,40*scale_y));

    numar_level=x;

    masina.sprite.setTexture(textura_masina);
    masina.sprite.setOrigin(7,7);
    masina.sprite.setScale(1*((float)width/1366),1*((float)hight/768));
    masina.forma.setSize(sf::Vector2f(constructor_x*scale_x,constructor_y*scale_y));


    for (int i=1; i<=19; i++)
    {
        int x=numar_stele_meniu[i];
        numar_stele[i]=x;
        numar_maxim_stele[i]=x;

        coins[i]=coinuri[i];
    }

    timp_precedent=ceas_general.getElapsedTime().asMicroseconds();
    init_levels(numar_level);
    masina.forma.setPosition(pozitie_masina);
    revive();
}

void scena_joc::update_events()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type==sf::Event::KeyPressed)
        {
            if (event.key.code==sf::Keyboard::Enter)
                is_turn_pressed=1;
            if (event.key.code==sf::Keyboard::R)
                is_r_pressed=1;
            if (event.key.code==sf::Keyboard::Num1&&numar_blocuri_disponibile_masinei[1]!=0)
                masina.tip_bloc=1;
            if (event.key.code==sf::Keyboard::Num2&&numar_blocuri_disponibile_masinei[2]!=0)
                masina.tip_bloc=2;
            if (event.key.code==sf::Keyboard::Escape)
                scena_curenta=0;


            /* if (event.key.code==sf::Keyboard::Q)
                 coeficient_propulsie+=0.01;
             if (event.key.code==sf::Keyboard::A)
                 coeficient_propulsie-=0.01;

             if (event.key.code==sf::Keyboard::W)
                 coeficient_saritura+=0.01;
             if (event.key.code==sf::Keyboard::S)
                 coeficient_saritura-=0.01;

             if (event.key.code==sf::Keyboard::E)
                 viteza_player+=0.05;
             if (event.key.code==sf::Keyboard::D)
                 viteza_player-=0.05;

             if (event.key.code==sf::Keyboard::T)
                 viteza_masina+=0.05;
             if (event.key.code==sf::Keyboard::G)
                 viteza_masina-=0.05;

             if (event.key.code==sf::Keyboard::Y)
                 sprite_offset+=1;
             if (event.key.code==sf::Keyboard::H)
                 sprite_offset-=1;*/
        }
        if (event.type==sf::Event::KeyReleased)
        {
            if (event.key.code==sf::Keyboard::Space)
                space_apasat=1;
        }

        if (event.type==sf::Event::Closed)
            game_on=0;



    }
}

void scena_joc::adaugare_bloc(int x,int y,int size_x,int size_y,sf::Color culoare,bool killer)
{
    blocuri[++numar_blocuri].forma.setPosition(sf::Vector2f(x,y));
    blocuri[numar_blocuri].forma.setSize(sf::Vector2f(size_x,size_y));
    blocuri[numar_blocuri].forma.setFillColor(culoare);
    blocuri[numar_blocuri].alive=1;
    blocuri[numar_blocuri].killer=killer;
}

void scena_joc::adaugare_bloc_masina(int x,int y,int size_x,int size_y,sf::Color culoare,int tip)
{

    blocuri_masina[++numar_blocuri_masina].forma.setPosition(sf::Vector2f(x,y));
    blocuri_masina[numar_blocuri_masina].forma.setSize(sf::Vector2f(size_x,size_y));
    blocuri_masina[numar_blocuri_masina].alive=1;
    blocuri_masina[numar_blocuri_masina].tip=tip;
    if (tip==1)
    {
        blocuri_masina[numar_blocuri_masina].propulsie=0;
        blocuri_masina[numar_blocuri_masina].forma.setTexture(&textura_bloc);
    }
    if (tip==2)
    {
        blocuri_masina[numar_blocuri_masina].propulsie=20;
        blocuri_masina[numar_blocuri_masina].forma.setTexture(&textura_bloc_propulsie);
    }
    int ok=0;
    for (int i=1; i<numar_blocuri_masina; i++)
    {
        if (blocuri_masina[i].forma.getGlobalBounds().intersects(blocuri_masina[numar_blocuri_masina].forma.getGlobalBounds())&&blocuri_masina[i].alive==1&&blocuri_masina[i].blocat==0)
        {
            if (ok==0)
            {
                masina.blocuri_ramase[tip]++;
            }
            ok=1;
            masina.blocuri_ramase[blocuri_masina[i].tip]++;
            blocuri_masina[i].alive=0;
            blocuri_masina[numar_blocuri_masina].alive=0;
        }
    }
    /*sf::Texture textura;
    textura.create(width,hight);
    textura.update(window);
    imagine=textura.copyToImage();*/

}

void scena_joc::draw_particule()
{
    for (int i=1; i<=numar_particule; i++)
        if (particule[i].alive())
        {
            window.draw(particule[i].forma);
        }
}

void scena_joc::update_particule(float x)
{
    for (int i=1; i<=numar_particule; i++)
    {
        if (particule[i].viteza_x!=0||particule[i].viteza_y!=0)
        {
            particule[i].miscare(x);
            particule[i].test_colizune();
        }
    }
}

int scena_joc::update_cooldown()
{
    int timp=ceas_general.getElapsedTime().asMicroseconds();

    for (int i=1; i<=numar_particule; i++)
    {
        if (particule[i].viteza_x!=0||particule[i].viteza_y!=0)
        {
            particule[i].countdown-=timp-timp_precedent;
            if (particule[i].countdown<=0)
                particule[i].countdown=0;
        }
    }
    playerul.cooldown_saritura-=timp-timp_precedent;
    if (playerul.cooldown_saritura<0)
        playerul.cooldown_saritura=0;

    playerul.marja_eroare_saritura-=timp-timp_precedent;
    if (playerul.marja_eroare_saritura<0)
        playerul.marja_eroare_saritura=0;
    int aux=timp_precedent;
    timp_precedent=timp;
    return timp-aux;
}

void scena_joc::actualizare_animatii(int timp_trecut)
{
}

void scena_joc::coliziuni(bool &moare)
{
    coliziune_palete(moare);
}

void scena_joc::actiune_masina()
{
    if (space_apasat)
    {
        adaugare_bloc_sound.play();
        if (
            ((masina.tip_bloc==1&&masina.blocuri_ramase[1])
             ||
             (masina.tip_bloc==2&&masina.blocuri_ramase[2])
            )
            &&
            (verificare_pozitie_bloc(sf::Vector2f(masina.forma.getPosition().x,masina.forma.getPosition().y))==0)
        )

        {
            adaugare_bloc_masina(masina.forma.getPosition().x,masina.forma.getPosition().y,masina.forma.getSize().x,masina.forma.getSize().y,sf::Color::Black,masina.tip_bloc);

            masina.blocuri_ramase[masina.tip_bloc]--;
        }
        else
        {
            for (int i=1; i<=numar_blocuri_masina; i++)
            {
                if (blocuri_masina[i].alive==1&&blocuri_masina[i].forma.getGlobalBounds().intersects(masina.sprite.getGlobalBounds()))
                {
                    blocuri_masina[i].alive=0;
                    if (blocuri_masina[i].propulsie==0)
                        masina.blocuri_ramase[1]++;
                    else
                        masina.blocuri_ramase[2]++;
                }
            }
        }
        space_apasat=0;
    }
}

void scena_joc::revive()
{
    playerul.forma.setPosition(pozitie_player);
    playerul.viteza_x=playerul.viteza_y=0;
    if (level==12)
        playerul.viteza_y=-putere_saritura*coeficient_saritura;
    for (int i=1; i<=numar_blocuri_masina; i++)
    {
        if (blocuri_masina[i].blocat==1)
        {
            blocuri_masina[i].blocat=0;
            if (blocuri_masina[i].tip==1)
                blocuri_masina[i].forma.setTexture(&textura_bloc);
            if (blocuri_masina[i].tip==2)
                blocuri_masina[i].forma.setTexture(&textura_bloc_propulsie);

        }
    }
}

void scena_joc::reset_level()
{
    for (int i=1; i<=numar_blocuri_masina; i++)
    {
        if (blocuri_masina[i].blocat==1)
        {
            blocuri_masina[i].blocat=0;
            if (blocuri_masina[i].tip==1)
                blocuri_masina[i].forma.setTexture(&textura_bloc);
            if (blocuri_masina[i].tip==2)
                blocuri_masina[i].forma.setTexture(&textura_bloc_propulsie);

        }
    }
    numar_blocuri_masina=0;
    masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1];
    masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2];
    space_apasat=0;
    revive();
}

void scena_joc::update_turn()
{
    if (is_turn_pressed&&(numar_blocuri_disponibile_masinei[1]!=0||numar_blocuri_disponibile_masinei[2]!=0))
    {
        if (turn == 2)
        {
            turn=1;
        }
        else if (turn == 1)
        {
            turn=2;
        }
        is_turn_pressed=0;
    }
    if (is_r_pressed)
    {
        reset_level();
        is_r_pressed=0;
    }

}

void scena_joc::init_levels(int x)
{
    for (int i=0; i<=19; i++)
        trigers[i]=1;
    turn=1;
    level=x;
    numar_blocuri=0;
    numar_morisci=0;
    numar_plasma_guns=0;
    e_coin=0;
    e_in_camera_coin=0;
    coin_colectat=coins[level];

    if (x<6)
    {
        textura_bloc.loadFromFile("img/bloc_sky.png");
        textura_bloc_propulsie.loadFromFile("img/bloc_sky_propulsie.png");
        bloc_blocat.loadFromFile("img/bloc_sky_blocat.png");
        bloc_propulsie_blocat.loadFromFile("img/bloc_sky_propulsie_blocat.png");

        if (music.getStatus()==0)
        {
            music.play();
            music_cave.stop();
        }
    }
    if (x>5)
    {
        if (music_cave.getStatus()==0)
        {
            music_cave.play();
            music.stop();
        }
    }
    if (x>9)
    {
        textura_bloc.loadFromFile("img/bloc_cave.png");
        textura_bloc_propulsie.loadFromFile("img/bloc_cave_propulsie.png");
        bloc_blocat.loadFromFile("img/bloc_cave_blocat.png");
        bloc_propulsie_blocat.loadFromFile("img/bloc_cave_propulsie_blocat.png");
    }

    switch (x)
    {
    case -3:
    {
        pozitie_player.x=0*scale_x;
        pozitie_player.y=(447-60)*scale_y;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=0;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=0;
        masina.tip_bloc=1;

        adaugare_chenar();
        init_hitboxuri(-3);
        background_texture.loadFromFile("img/levels/level_-3.png");
        break;
    }
    case -2:
    {
        pozitie_player.x=0*scale_x;
        pozitie_player.y=(447-60)*scale_y;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=0;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=0;
        masina.tip_bloc=1;

        adaugare_chenar();
        init_hitboxuri(-2);

        background_texture.loadFromFile("img/levels/level_-2.png");
        break;
    }
    case -1:
    {
        pozitie_player.x=0*scale_x;
        pozitie_player.y=(447-60)*scale_y;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=0;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=0;
        masina.tip_bloc=1;

        adaugare_chenar();
        init_hitboxuri(-1);


        background_texture.loadFromFile("img/levels/level_-1.png");
        break;
    }
    case 0:
    {
        pozitie_player.x=0*scale_x;
        pozitie_player.y=(447-60)*scale_y;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=99;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=99;
        masina.tip_bloc=1;

        adaugare_chenar();

        init_hitboxuri(0);

        background_texture.loadFromFile("img/levels/level_-0.png");
        break;
    }
    case 1:
    {
        pozitie_player.x=0*scale_x;
        pozitie_player.y=(511-60)*scale_y;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=7;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=1;
        masina.tip_bloc=1;

        adaugare_chenar();
        init_hitboxuri(1);


        adaugare_coin(sf::Vector2f(0*scale_x,0*scale_y),sf::Vector2f(0*scale_x,0*scale_y),sf::Vector2f(1366/2*scale_x,32*scale_y));
        background_texture.loadFromFile("img/levels/level_1.png");
        break;
    }
    case 2:
    {
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=7;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=0;
        masina.tip_bloc=1;

        pozitie_player.x=0*scale_x;
        pozitie_player.y=(511-60)*scale_y;
        pozitie_masina.x=0*scale_x;
        pozitie_masina.y=(511-60)*scale_y;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;

        adaugare_chenar();
        init_hitboxuri(2);
        adaugare_morisca(sf::Vector2f(160*scale_x,290*scale_y),3,sf::Vector2f(7*scale_x,40*scale_y),100000,&textura_palete);
        adaugare_coin(sf::Vector2f(0*scale_x,0*scale_y),sf::Vector2f(0*scale_x,0*scale_y),sf::Vector2f(280*scale_x,744*scale_y));
        background_texture.loadFromFile("img/levels/level_2.png");
        break;
    }
    case 3:
    {
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=7;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=0;
        masina.tip_bloc=1;
        pozitie_player.x=0*scale_x;
        pozitie_player.y=(414-60)*scale_y;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;
        adaugare_chenar();
        init_hitboxuri(3);

        adaugare_coin(sf::Vector2f(0*scale_x,0*scale_y),sf::Vector2f(0*scale_x,0*scale_y),sf::Vector2f(751*scale_x,320*scale_y));
        background_texture.loadFromFile("img/levels/level_3.png");
        break;
    }
    case 4:
    {
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=6;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=3;
        masina.tip_bloc=1;
        pozitie_player.x=0*scale_x;
        pozitie_player.y=(415-60)*scale_y;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;
        adaugare_chenar();
        init_hitboxuri(4);
        adaugare_coin(sf::Vector2f(0*scale_x,0*scale_y),sf::Vector2f(0*scale_x,0*scale_y),sf::Vector2f(1366/2*scale_x,32*scale_y));
        background_texture.loadFromFile("img/levels/level_4.png");
        break;
    }
    case 5:
    {
        pozitie_player.x=0*scale_x;
        pozitie_player.y=(415-60)*scale_y;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=5;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=2;
        masina.tip_bloc=1;
        adaugare_chenar();
        init_hitboxuri(5);
        adaugare_morisca(sf::Vector2f(404*scale_x,400*scale_y),7,sf::Vector2f(5*scale_x,100*scale_y),50000,&textura_palete);
        adaugare_morisca(sf::Vector2f(985*scale_x,400*scale_y),7,sf::Vector2f(5*scale_x,100*scale_y),50000,&textura_palete);
        adaugare_coin(sf::Vector2f(0*scale_x,0*scale_y),sf::Vector2f(0*scale_x,0*scale_y),sf::Vector2f(900*scale_x,32*scale_y));

        background_texture.loadFromFile("img/levels/level_5.png");
        break;
    }
    case 6:
    {
        pozitie_player.x=0*scale_x;
        pozitie_player.y=(607-60)*scale_y;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=10;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=3;
        adaugare_chenar();
        init_hitboxuri(6);
        background_texture.loadFromFile("img/levels/level_6.png");


        adaugare_coin(sf::Vector2f(1216,64),sf::Vector2f(130,160),sf::Vector2f(1290,120));


        break;
    }
    case 7:
    {
        pozitie_player.x=0*scale_x;
        pozitie_player.y=(447-60)*scale_y;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=0;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=12;
        masina.tip_bloc=2;
        adaugare_chenar();
        init_hitboxuri(7);

        adaugare_coin(sf::Vector2f(576*scale_x,576*scale_y),sf::Vector2f(224*scale_x,128*scale_y),sf::Vector2f(688*scale_x,638*scale_y));

        adaugare_morisca(sf::Vector2f(400*scale_x,400*scale_y),5,sf::Vector2f(10*scale_x,140*scale_y),50000,&textura_palete);

        background_texture.loadFromFile("img/levels/level_7.png");
        break;
    }
    case 8:
    {
        pozitie_player.x=0*scale_x;
        pozitie_player.y=(511-60)*scale_y;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=15;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=5;
        masina.tip_bloc=1;
        adaugare_chenar();

        init_hitboxuri(8);

        adaugare_coin(sf::Vector2f(1152*scale_x,32*scale_y),sf::Vector2f(182*scale_x,161*scale_y),sf::Vector2f(1295*scale_x,80*scale_y));


        background_texture.loadFromFile("img/levels/level_8.png");
        break;
    }
    case 9:
    {
        pozitie_player.x=0*scale_x;
        pozitie_player.y=(575-60)*scale_y;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=15;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=5;
        masina.tip_bloc=1;

        init_hitboxuri(9);

        adaugare_coin(sf::Vector2f(160*scale_x,32*scale_y),sf::Vector2f(224*scale_x,64*scale_y),sf::Vector2f(270*scale_x,63*scale_y));


        background_texture.loadFromFile("img/levels/level_9.png");
        break;
    }
    case 10:
    {
        pozitie_player.x=0*scale_x;
        pozitie_player.y=(607-60)*scale_y;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=15;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=5;
        masina.tip_bloc=1;
        adaugare_bloc(-50,-30,30,hight+60,sf::Color::White,0);

        init_hitboxuri(10);

        adaugare_coin(sf::Vector2f(640*scale_x,32*scale_y),sf::Vector2f(96*scale_x,96*scale_y),sf::Vector2f(688*scale_x,80*scale_y));

        background_texture.loadFromFile("img/levels/level_10.png");
        break;
    }
    case 11:
    {
        pozitie_player.x=0*scale_x;
        pozitie_player.y=(607-60)*scale_y;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=15;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=5;
        masina.tip_bloc=1;
        adaugare_bloc(-50,-30,30,hight+60,sf::Color::White,0);

        init_hitboxuri(11);


        adaugare_coin(sf::Vector2f(350*scale_x,64*scale_y),sf::Vector2f(384*scale_x,384*scale_y),sf::Vector2f(222*scale_x,148*scale_y));

        background_texture.loadFromFile("img/levels/level_11.png");
        break;
    }
    case 12:
    {
        pozitie_player.x=890*scale_x;
        pozitie_player.y=(760-60)*scale_y;
        playerul.viteza_y=-putere_saritura*coeficient_saritura;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=15;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=5;
        masina.tip_bloc=1;
        adaugare_bloc(-50,-30,30,hight+60,sf::Color::White,0);
        adaugare_chenar();
        init_hitboxuri(12);
        adaugare_bloc(669,190,60,5,sf::Color::Transparent,1);

        adaugare_coin(sf::Vector2f(1216*scale_x,32*scale_y),sf::Vector2f(128*scale_x,96*scale_y),sf::Vector2f(1312*scale_x,73*scale_y));

        background_texture.loadFromFile("img/levels/level_12.png");
        break;
    }
    case 13:
    {
        pozitie_player.x=0*scale_x;
        pozitie_player.y=(383-60)*scale_y;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=15;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=5;
        masina.tip_bloc=1;
        adaugare_bloc(-50,-30,30,hight+60,sf::Color::White,0);
        adaugare_chenar();
        init_hitboxuri(13);


        adaugare_coin(sf::Vector2f(132*scale_x,32*scale_y),sf::Vector2f(192*scale_x,96*scale_y),sf::Vector2f(335*scale_x,80*scale_y));

        background_texture.loadFromFile("img/levels/level_13.png");
        break;
    }
    case 14:
    {
        pozitie_player.x=0*scale_x;
        pozitie_player.y=(735-60)*scale_y;
        pozitie_masina.x=pozitie_player.x;
        pozitie_masina.y=pozitie_player.y;
        masina.blocuri_ramase[1]=numar_blocuri_disponibile_masinei[1]=0;
        masina.blocuri_ramase[2]=numar_blocuri_disponibile_masinei[2]=0;
        masina.tip_bloc=1;
        adaugare_bloc(-50,-30,30,hight+60,sf::Color::White,0);
        adaugare_chenar();
        init_hitboxuri(14);

        background_texture.loadFromFile("img/levels/level_14.png");
        break;

    }
    default:
    {
        scena_curenta=0;
        level--;
        break;
    }
    if (numar_blocuri_disponibile_masinei[2]!=0)
        masina.tip_bloc=2;
    if (numar_blocuri_disponibile_masinei[1]!=0)
        masina.tip_bloc=1;
    }

}

void scena_joc::draw_numar_blocuri_ramase()
{
    sf::Sprite bloc_normal;
    sf::RectangleShape chenar;
    float scale_x=1,scale_y=1;
    chenar.setSize(sf::Vector2f((36*((float)width/1366)),36*((float)hight/768)));
    chenar.setFillColor(sf::Color::Transparent);
    chenar.setOutlineColor(sf::Color::White);
    chenar.setOutlineThickness(2);
    bloc_normal.setTexture(textura_bloc);
    bloc_normal.setScale(sf::Vector2f(((float)width/1366),((float)hight/768)));

    int distanta=1;
    int dimensiune=50;

    if (numar_blocuri_disponibile_masinei[1]!=0)
    {
        if (masina.blocuri_ramase[1]<10)
        {
            sf::RectangleShape text;
            text.setPosition(sf::Vector2f(width-(90+dimensiune+distanta)*((float)width/1366),100*((float)hight/768)));
            text.setSize(sf::Vector2f(20*((float)width/1366),20*((float)hight/768)));
            schimbare_numar(masina.blocuri_ramase[1],numar_blocuri_ramase);
            text.setFillColor(sf::Color::White);
            text.setTexture(&numar_blocuri_ramase.textura);
            bloc_normal.setPosition(sf::Vector2f(width-(100+dimensiune+distanta)*((float)width/1366),65*((float)hight/768)));
            if (masina.tip_bloc==1)
            {
                chenar.setPosition(sf::Vector2f(bloc_normal.getPosition().x-(2)*((float)width/1366),bloc_normal.getPosition().y-(2)*((float)hight/768)));
                window.draw(chenar);
            }
            window.draw(bloc_normal);
            window.draw(text);
        }
        if (masina.blocuri_ramase[1]>=10)
        {
            sf::RectangleShape text;
            sf::RectangleShape text2;
            text.setPosition(sf::Vector2f(width-(90+dimensiune+distanta)*((float)width/1366),100*((float)hight/768)));
            text.setSize(sf::Vector2f(20*((float)width/1366),20*((float)hight/768)));
            text2.setPosition(sf::Vector2f(width-(100+dimensiune+distanta)*((float)width/1366),100*((float)hight/768)));
            text2.setSize(sf::Vector2f(20*((float)width/1366),20*((float)hight/768)));
            schimbare_numar(masina.blocuri_ramase[1],numar_blocuri_ramase);
            text.setFillColor(sf::Color::White);
            text.setTexture(&numar_blocuri_ramase.textura);
            text2.setTexture(&numar_blocuri_ramase.zece);
            bloc_normal.setPosition(sf::Vector2f(width-(100+dimensiune+distanta)*((float)width/1366),65*((float)hight/768)));
            if (masina.tip_bloc==1)
            {
                chenar.setPosition(sf::Vector2f(bloc_normal.getPosition().x-(2)*scale_x,bloc_normal.getPosition().y-(2)*scale_y));
                window.draw(chenar);
            }
            window.draw(bloc_normal);
            window.draw(text);
            window.draw(text2);
        }
    }
    bloc_normal.setTexture(textura_bloc_propulsie);
    if (numar_blocuri_disponibile_masinei[2]!=0)
    {
        if (masina.blocuri_ramase[2]<10)
        {
            sf::RectangleShape text;
            text.setPosition(sf::Vector2f(width-(100)*((float)width/1366),100*((float)hight/768)));
            text.setSize(sf::Vector2f(20*((float)width/1366),20*((float)hight/768)));
            schimbare_numar(masina.blocuri_ramase[2],numar_blocuri_ramase);
            text.setFillColor(sf::Color::White);
            text.setTexture(&numar_blocuri_ramase.textura);
            bloc_normal.setPosition(sf::Vector2f(width-(110)*((float)width/1366),65*((float)hight/768)));
            if (masina.tip_bloc==2)
            {

                chenar.setPosition(sf::Vector2f(bloc_normal.getPosition().x-(2)*((float)width/1366),bloc_normal.getPosition().y-(2)*((float)hight/768)));
                window.draw(chenar);
            }
            window.draw(bloc_normal);
            window.draw(text);
        }
        if (masina.blocuri_ramase[2]>=10)
        {
            sf::RectangleShape text;
            sf::RectangleShape text2;
            text.setPosition(sf::Vector2f(width-(100)*((float)width/1366),100*((float)hight/768)));
            text.setSize(sf::Vector2f(20*scale_x,20*((float)hight/768)));
            text2.setPosition(sf::Vector2f(width-(110)*((float)width/1366),100*((float)hight/768)));
            text2.setSize(sf::Vector2f(20*((float)width/1366),20*((float)hight/768)));
            schimbare_numar(masina.blocuri_ramase[2],numar_blocuri_ramase);
            text.setFillColor(sf::Color::White);
            text.setTexture(&numar_blocuri_ramase.textura);
            text2.setTexture(&numar_blocuri_ramase.zece);
            bloc_normal.setPosition(sf::Vector2f(width-(110)*((float)width/1366),65*((float)hight/768)));
            if (masina.tip_bloc==2)
            {
                chenar.setPosition(sf::Vector2f(bloc_normal.getPosition().x-(2)*((float)width/1366),bloc_normal.getPosition().y-(2)*((float)hight/768)));
                window.draw(chenar);
            }
            window.draw(bloc_normal);
            window.draw(text);
            window.draw(text2);
        }
    }
}

void scena_joc::update_animatii(int x)
{
    int row=masina.animatia.curent_image.y;

    if (space_apasat==1)//&&abs(masina.forma.getPosition().x-pozitie_masina.x)>abs(playerul.forma.getPosition().x-pozitie_player.x)+playerul.forma.getSize().x)
    {
        row=0;
        masina.animatia.curent_image.x=0;
    }
    else if (masina.animatia.curent_image.x==7&&x+masina.animatia.total_time>=masina.animatia.switch_time)
    {
        row=masina.animatia.curent_image.y=1;
    }



    masina.animatia.update(row,x,masina.dreapta,0);
    masina.sprite.setTextureRect(masina.animatia.uv_rect);


    row=playerul.animatia.curent_image.y;
    int h=0;
    if ((playerul.viteza_x>0.8||playerul.viteza_x<-0.8)&&!sf::Keyboard::isKeyPressed(playerul.sus))
    {
        row=2;
        playerul.animatia.image_count.x=4;
    }
    else
    {
        row=0;
        playerul.animatia.image_count.x=8;

    }
    if (playerul.viteza_y>0)
    {
        row=4;
        playerul.animatia.image_count.x=1;
    }
    if (playerul.viteza_y<0)
    {
        row=5;
        playerul.animatia.image_count.x=1;
    }
    if (playerul.sta_jos)
    {
        h=sprite_offset;
        row=1;
        playerul.animatia.image_count.x=6;
    }

    playerul.animatia.update(row,x,playerul.orientare_dreapta,0);
    playerul.sprite.setTextureRect(playerul.animatia.uv_rect);
    playerul.sprite.setPosition(sf::Vector2f(playerul.forma.getPosition().x+playerul.forma.getSize().x/2,playerul.forma.getPosition().y+playerul.forma.getSize().y/2+h));

    if (coin_colectat==0&&(e_in_camera_coin==1||level<6))
    {
        int row=0;
        coinul.animatia.update(row,x,1,0);
        coinul.forma.setTextureRect(coinul.animatia.uv_rect);
    }
    if (e_coin==1)
    {
        int row=0;
        coin_scor.animatia.update(row,x,1,0);
        coin_scor.forma.setTextureRect(coin_scor.animatia.uv_rect);
    }


}

void scena_joc::adaugare_chenar()
{
    adaugare_bloc(-50,-30,30,hight+60,sf::Color::White,0);

    adaugare_bloc(-30,-50,width+60,30,sf::Color::White,0);

    //adaugare_bloc(-30,hight+20,width+60,30,sf::Color::White,0);

    adaugare_bloc(width+20,-30,30,hight+60,sf::Color::White,0);
}

void scena_joc::adaugare_morisca(sf::Vector2f pozitie,int numar_palete,sf::Vector2f dimensiune,int switch_time,sf::Texture *textura)
{
    morisci[++numar_morisci].setup(numar_palete,pozitie,dimensiune,textura,switch_time);
}

void scena_joc::update_moristi(int x)
{
    for (int i=1; i<=numar_morisci; i++)
    {
        morisci[i].update(x);
    }
}

void scena_joc::draw_morisci()
{
    for (int i=1; i<=numar_morisci; i++)
    {
        for (int j=1; j<=morisci[i].numar_palete; j++)
        {
            window.draw(morisci[i].palete[j]);
        }
    }
}

void scena_joc::coliziune_palete(bool &moare)
{
    for (int i=1; i<=numar_morisci; i++)
    {
        for (int j=1; j<=morisci[i].numar_palete; j++)
        {
            if (playerul.forma.getGlobalBounds().intersects(morisci[i].palete[j].getGlobalBounds()))
            {
                moare=1;
            }
            for (int k=1; k<=numar_blocuri_masina; k++)
                if (blocuri_masina[k].forma.getGlobalBounds().intersects(morisci[i].palete[j].getGlobalBounds())&&blocuri_masina[k].alive)
                {
                    blocuri_masina[k].alive=0;
                    masina.blocuri_ramase[blocuri_masina[k].tip]++;
                }
        }
    }
}

void scena_joc::update_stele()
{
    int s=0,n=0;
    for (int i=1; i<=2; i++)
        s+=masina.blocuri_ramase[i];
    for (int i=1; i<=2; i++)
        n+=numar_blocuri_disponibile_masinei[i];
    s=n-s;

    //cout << s << endl;
    if (level>=1)
    {
        switch (level)
        {
        case 1:
        {
            int x=0;
            if (s<=4)
                x++;
            if (s<=5)
                x++;
            if (s<=6)
                x++;
            numar_stele[1]=x;
            break;
        }
        case 2:
        {
            int x=0;
            if (s<=4)
                x++;
            if (s<=5)
                x++;
            if (s<=7)
                x++;
            numar_stele[2]=x;
            break;
        }
        case 3:
        {
            int x=0;
            if (s<=4)
                x++;
            if (s<=5)
                x++;
            if (s<=7)
                x++;
            numar_stele[3]=x;
            break;
        }
        case 4:
        {
            int x=0;
            if (s<=2)
                x++;
            if (s<=4)
                x++;
            if (s<=5)
                x++;
            numar_stele[4]=x;
            break;
        }
        case 5:
        {
            int x=0;
            if (s<=4)
                x++;
            if (s<=5)
                x++;
            if (s<=7)
                x++;
            numar_stele[5]=x;
            break;
        }
        case 6:
        {
            int x=0;
            if (s<=6)
                x++;
            if (s<=7)
                x++;
            if (s<=8)
                x++;
            numar_stele[6]=x;
            break;
        }
        case 7:
        {
            int x=0;
            if (s<=4)
                x++;
            if (s<=5)
                x++;
            if (s<=6)
                x++;
            numar_stele[7]=x;
            break;
        }
        case 8:
        {
            int x=0;
            if (s<=10)
                x++;
            if (s<=14)
                x++;
            if (s<=16)
                x++;
            numar_stele[8]=x;
            break;
        }
        case 9:
        {
            int x=0;
            if (s<=10)
                x++;
            if (s<=14)
                x++;
            if (s<=16)
                x++;
            numar_stele[8]=x;
            break;
        }
        }
    }
}

void scena_joc::draw_stele()
{
    if ((numar_blocuri_disponibile_masinei[1]!=0||numar_blocuri_disponibile_masinei[2]!=0)&&level>=1)
    {
        int x,y;
        x=width-154*((float)width/1366);
        y=120*((float)hight/768);

        sf::RectangleShape stea;
        stea.setPosition(sf::Vector2f(x,y));
        stea.setSize(sf::Vector2f(40*((float)width/1366),40*((float)hight/768)));
        stea.setTexture(&textura_stea);

        unsigned int i;
        for (i=1; i<=numar_stele[level]; i++)
        {
            window.draw(stea);
            stea.move(sf::Vector2f(stea.getSize().x+1,0));
        }

        stea.setTexture(&textura_stea_goala);
        for (i=i; i<=3; i++)
        {
            window.draw(stea);
            stea.move(sf::Vector2f(stea.getSize().x+1,0));
        }
    }


}

void scena_joc::copiere_date(unsigned int x[20],bool coinuri[20])
{
    for (int i=1; i<=19; i++)
    {
        x[i]=numar_maxim_stele[i];
    }
    for (int i=1; i<=19; i++)
    {
        coinuri[i]=coins[i];
    }
}

bool scena_joc::verificare_pozitie_bloc(sf::Vector2f pozitie)
{
    sf::RectangleShape spawn;
    spawn.setPosition(sf::Vector2f(pozitie_player.x,pozitie_player.y));
    spawn.setSize(playerul.forma.getSize());
    sf::RectangleShape bloc;
    bloc.setPosition(pozitie);
    bloc.setSize(sf::Vector2f(32*scale_x,32*scale_y));
    return spawn.getGlobalBounds().intersects(bloc.getGlobalBounds())||playerul.forma.getGlobalBounds().intersects(bloc.getGlobalBounds());
}

void scena_joc::pop_up_stele()
{
    scena_pop_up pop_up;
    sf::Texture textura;
    textura.create(width,hight);
    textura.update(window);
    sf::Image img=textura.copyToImage();
    pop_up.init_stele_pop_up(img,numar_stele[level]);
    pop_up.draw();
    sf::sleep(sf::seconds(0.4));
    sf::Event event;
    int ok=2;
    while (window.pollEvent(event));

    int final_frame=0;
    int timp=ceas_general.getElapsedTime().asMicroseconds();
    int sleep=0;
    while (ok)
    {
        timp=ceas_general.getElapsedTime().asMicroseconds();
        sleep+=final_frame;
        sf::Event event;
        if (sleep<=500000)
            while (window.pollEvent(event));
        else
            sleep=500001;
        while (window.pollEvent(event))
        {
            if (event.type==sf::Event::KeyReleased&&event.key.code==sf::Keyboard::Enter)
                ok=0;
        }
        pop_up.update(final_frame);
        final_frame=ceas_general.getElapsedTime().asMicroseconds()-timp;
    }
    pop_up.fade_out();
    timp_precedent=ceas_general.getElapsedTime().asMicroseconds();
}

void scena_joc::pop_up_text(char txt[350])
{

    cout <<endl<<strlen(txt)<<endl;
    scena_pop_up pop_up;
    sf::Texture textura;
    textura.create(width,hight);
    textura.update(window);
    sf::Image image=textura.copyToImage();
    pop_up.init_text(image,txt);
    sf::Event event;
    int ok=2;
    while (window.pollEvent(event));

    int final_frame=0;
    int timp=ceas_general.getElapsedTime().asMicroseconds();
    int sleep=0;
    while (ok)
    {
        timp=ceas_general.getElapsedTime().asMicroseconds();
        sleep+=final_frame;
        sf::Event event;
        if (sleep<=500000)
            while (window.pollEvent(event));
        else
            sleep=500001;
        while (window.pollEvent(event))
        {
            if (event.type==sf::Event::KeyReleased&&event.key.code==sf::Keyboard::Enter)
                ok=0;
        }
        pop_up.update(final_frame);
        final_frame=ceas_general.getElapsedTime().asMicroseconds()-timp;
    }
    pop_up.fade_out();
    timp_precedent=ceas_general.getElapsedTime().asMicroseconds();
}

void scena_joc::init_hitboxuri(int x)
{
    char s[40];
    strcpy(s,"data/hitbox/");
    char n[3];
    if (x<=0)
    {
        n[0]='-';
        n[1]='0'+abs(x);
        n[2]=(char)NULL;
    }
    if (x>0)
    {
        n[0]='0'+abs(x);
        n[1]=(char)NULL;
    }
    if (x>=10)
    {
        n[0]='0'+x/10;
        n[1]='0'+x%10;
        n[2]=(char)NULL;
    }
    strcat(s,n);
    strcat(s,".txt");
    ifstream r(s);
    int X,y,L,l,o;
    while (r >> X >> y >> L >> l>>o)
    {
        if (o==0)
            adaugare_bloc(X*scale_x,y*scale_y,L*scale_x,l*scale_y,sf::Color::Transparent,o);
        else
            adaugare_bloc(X*scale_x,y*scale_y,L*scale_x,l*scale_y,sf::Color::Transparent,o);

    }
}

void scena_joc::adaugare_coin(sf::Vector2f camera_position,sf::Vector2f camera_size,sf::Vector2f coin_position)
{
    camera_coin.setPosition(camera_position);
    camera_coin.setSize(camera_size);
    coinul.setup(coin_position);
    e_coin=1;
    e_in_camera_coin=0;

}

void scena_joc::update_coin(int dt)
{
    if (e_coin==1)
    {
        if (level>5)
        {
            bool e_inauntru=playerul.forma.getGlobalBounds().intersects(camera_coin.getGlobalBounds());
            if (e_inauntru==1&&e_in_camera_coin==0)
            {
                e_in_camera_coin=1;
                char s[40]= {0};
                strcpy(s,"img/levels/level_");
                char x[4];
                if (level<10)
                {
                    x[0]='0'+level;
                    x[1]=(char)NULL;
                }
                if (level>=10)
                {
                    x[0]='0'+level/10;
                    x[1]='0'+level%10;
                    x[2]=(char)NULL;
                }
                strcat(s,x);
                strcat(s,"_2.png");
                background_texture.loadFromFile(s);
                background.setTexture(&background_texture);
            }
            if (e_inauntru==0&&e_in_camera_coin==1)
            {
                e_in_camera_coin=0;
                char s[40]= {0};
                strcpy(s,"img/levels/level_");
                char x[4];
                if (level<10)
                {
                    x[0]='0'+level;
                    x[1]=(char)NULL;
                }
                if (level>=10)
                {
                    x[0]='0'+level/10;
                    x[1]='0'+level%10;
                    x[2]=(char)NULL;
                }
                strcat(s,x);
                strcat(s,".png");
                background_texture.loadFromFile(s);
                background.setTexture(&background_texture);
            }
        }
        if (playerul.forma.getGlobalBounds().intersects(coinul.forma.getGlobalBounds())==1&&coin_colectat==0)
        {
            coin_colectat=1;
            coins[level]=1;
            sunet_coin.play();
        }
    }
}

void scena_joc::draw_coin()
{
    if (e_coin)
    {

        int x=width-154*((float)width/1366);
        int y=160*((float)hight/768);
        window.draw(coin_scor.forma);
        if (coin_colectat==1)
            schimbare_numar(1,numar_blocuri_ramase);
        else
            schimbare_numar(0,numar_blocuri_ramase);
        sf::RectangleShape text;
        text.setTexture(&numar_blocuri_ramase.textura);
        text.setSize(sf::Vector2f(20*scale_x,20*scale_y));
        text.setPosition(sf::Vector2f((x+38)*scale_x,(y+10)*scale_y));
        window.draw(text);
        text.setTexture(&slash);
        text.move(sf::Vector2f(10*scale_x,0));
        window.draw(text);
        schimbare_numar(1,numar_blocuri_ramase);
        text.setTexture(&numar_blocuri_ramase.textura);
        text.move(sf::Vector2f(10*scale_x,0));
        window.draw(text);
    }
}

void scena_joc::adaugare_plasma_gun(sf::Vector2f pozitie,sf::Vector2f dimenisune,int unghi,int cooldown_tragere,int cooldown_plasma)
{
    plasma_guns[++numar_plasma_guns].forma.setSize(dimenisune);
    plasma_guns[numar_plasma_guns].forma.setOrigin(sf::Vector2f(dimenisune.x/2,dimenisune.y/2));
    plasma_guns[numar_plasma_guns].forma.setPosition(pozitie);
    plasma_guns[numar_plasma_guns].unghi=-abs(unghi);
    plasma_guns[numar_plasma_guns].directie=sf::Vector2f(sin(unghi),cos(unghi));
    plasma_guns[numar_plasma_guns].cooldown_tragere=cooldown_tragere;
    plasma_guns[numar_plasma_guns].cooldown_plasma=cooldown_plasma;
    plasma_guns[numar_plasma_guns].curent_time=cooldown_tragere;
    plasma_guns[numar_plasma_guns].stare=1;
}


void scena_joc::update_plasma_guns(int dt)
{
    for (int i=1; i<=numar_plasma_guns; i++)
    {
        plasma_guns[i].curent_time-=dt;
        if (plasma_guns[i].curent_time<=0)
        {
            if (plasma_guns[i].stare==1)
            {
                plasma_guns[i].stare=2;
                sf::Vector2f pozitie=plasma_guns[i].forma.getPosition();
                sf::Vector2f dimensiune=plasma_guns[i].forma.getSize();
                //sf::Vector2f directie=plasma_guns[i].directie;
                adaugare_bloc(pozitie.x,pozitie.y,width*2,dimensiune.y/2,sf::Color::Red,1);
                blocuri[numar_blocuri].forma.setRotation(plasma_guns[i].unghi);
                blocuri[numar_blocuri].forma.setOrigin(sf::Vector2f(0,blocuri[numar_blocuri].forma.getSize().y/2));
                plasma_guns[i].indice_bloc=numar_blocuri;
                plasma_guns[i].curent_time=plasma_guns[i].cooldown_plasma;
            }
            else
            {
                plasma_guns[i].stare=1;
                blocuri[plasma_guns[i].indice_bloc].alive=0;
                plasma_guns[i].curent_time=plasma_guns[i].cooldown_tragere;
            }
        }
    }

}
void scena_joc::draw_plasma_guns()
{
    for (int i=1; i<=numar_plasma_guns; i++)
    {
        window.draw(plasma_guns[i].forma);
    }
}

void scena_joc::update_blocuri()
{
    for (int i=1; i<=numar_blocuri; i++)
        for (int j=1; j<=numar_blocuri_masina; j++)
            if (blocuri[i].forma.getGlobalBounds().intersects(blocuri_masina[j].forma.getGlobalBounds())&&blocuri[i].alive==1&&blocuri[i].killer==1&&blocuri_masina[j].alive==1)
            {
                blocuri_masina[j].alive=0;
                masina.blocuri_ramase[blocuri_masina[j].tip]++;
            }
}
