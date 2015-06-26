#include <SFML/Graphics.hpp>
#include "Collision.hpp"
#include <iostream>

namespace Collision
{
  bool checkRecToRecCollision(sf::Rect<float> rect1, sf::Rect<float> rect2)
  {
    if( rect1.left < rect2.left + rect2.width &&
       rect1.left + rect1.width > rect2.left &&
   rect1.top < rect2.top + rect2.height &&
   rect1.height + rect1.top > rect2.top) {
    /*std::cout<<rect1.left <<" < " << rect2.left <<"+"<< rect2.width <<" &&\n" <<
       rect1.left + rect1.width<< " > " << rect2.left <<" &&\n" <<
   rect1.top <<" < "<<rect2.top + rect2.height <<" &&\n" <<
   rect1.height + rect1.top <<" > "<< rect2.top<<std::endl;*/
    return true;
   }
   return false;
  }
}
