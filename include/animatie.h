#ifndef ANIMATIE_H
#define ANIMATIE_H
#include <SFML/Graphics.hpp>

class animatie
{
public:
    sf::Vector2u image_count;
    sf::Vector2u curent_image;

    float total_time;
    float switch_time;
    int stai=0;

    sf::IntRect uv_rect;


    animatie();

    void update(int &row,float delta_time,bool stanga,int stay);

    void setup(sf::Texture* texture,sf::Vector2u image_count,float switch_time);

};

#endif // ANIMATIE_H
