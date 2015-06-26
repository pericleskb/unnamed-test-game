#pragma once
#include <SFML/Graphics/RectangleShape.hpp>

class Border
{
public:

    Border(float x, float y, float width, float height);
    ~Border();

private:
    sf::RectangleShape shape;

};
