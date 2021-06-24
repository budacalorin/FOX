#ifndef PARTICULA
#define PARTICULA

#include <SFML/Graphics.hpp>

class particula{
public:
    sf::RectangleShape forma;
    float viteza_x,viteza_y;
    int countdown;
    float hp=50;
    int unghi;

    particula();

    void setup(int x,int y,float viteza_Y,float viteza_X,int UNGHI, int COUNTDOWN,sf::Color culoare);

    void miscare(float x);

    void test_colizune();

    bool alive();

};

#endif // PARTICULA
