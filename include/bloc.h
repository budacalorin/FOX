#ifndef BLOC
#define BLOC
#include <SFML/Graphics.hpp>
class bloc{
public:
    sf::RectangleShape forma;
    bool alive;
    int tip;
    float propulsie=0;
    bool killer=0;
    bool blocat=0;
};

#endif // BLOC
