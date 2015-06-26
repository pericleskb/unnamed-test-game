#pragma once
#include <iostream>
#include "Enemy.hpp"

#define FULL_HP 3;
#define SLOW_FACTOR 1;

class EnemyOrc: public Enemy
{
public:
  EnemyOrc(int x, int y);
  ~EnemyOrc();

  void Update(sf::Time elapsedTime);
  void resetObject();

private:

  int chaseRadius = 200;
  int attackRadius = 50;

  void checkIfHit();

  Animation orc_attack_up,orc_attack_down,orc_attack_right,orc_attack_left,
            orc_up,orc_down,orc_right,orc_left,orc_dying;

  std::vector<Animation*> animations;

  std::string animationIDs[9] = {
    "orc_attack_up","orc_attack_left","orc_attack_down","orc_attack_right",
    "orc_up","orc_left","orc_down","orc_right","orc_dying"};
};
