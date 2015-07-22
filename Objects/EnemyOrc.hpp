#pragma once
#include <iostream>
#include "Enemy.hpp"

#define SLOW_FACTOR 1;

class EnemyOrc: public Enemy
{
public:
  EnemyOrc(int x, int y);
  ~EnemyOrc();
  void Draw(sf::RenderWindow & window);
  void Update(sf::Time elapsedTime);
  void resetObject();
  void SetPush(double angle,sf::Time time);
  void setDamage(int);
private:
  //for testing
  sf::CircleShape viewRadiusCircle;

  void checkIfHit();

  Animation orc_attack_up,orc_attack_down,orc_attack_right,orc_attack_left,
            orc_up,orc_down,orc_right,orc_left,orc_dying;

  std::vector<Animation*> animations;

  std::string animationIDs[9] = {
    "orc_attack_up","orc_attack_left","orc_attack_down","orc_attack_right",
    "orc_up","orc_left","orc_down","orc_right","orc_dying"};
};
