#include "Border.hpp"

Border::Border(float x, float y, float width, float height)
{
  this->shape.setPosition(sf::Vector2f(x,y));
  this->shape.setSize(sf::Vector2f(width,height));
}
