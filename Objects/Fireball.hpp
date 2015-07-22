#pragma once
#include "VisibleObject.hpp"

class Fireball: public VisibleObject
{
public:
  Fireball(int x, int y,int direction);
  ~Fireball();

  void Update(sf::Time elapsedTime);
	virtual sf::Rect<float> GetBoundingRect() const;
	virtual sf::Rect<float> GetBorderBoundingRect() const;
private:

    Animation fireball_left,fireball_up,fireball_right,fireball_down;
  //  std::vector<Animation*> animations;
    std::string animationIDs[4] =
        {"fireball_left","fireball_up", "fireball_right","fireball_down"};
};
