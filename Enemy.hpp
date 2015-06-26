#pragma once
#include "VisibleObject.hpp"

#define PI 3.14159265

class Enemy: public VisibleObject
{
 public:

  Enemy();
  Enemy(int x, int y);
  ~Enemy();

  virtual void Update(sf::Time timeDelta);
  virtual void updateGuardTarget();

  virtual void setSlowFactor(float factor, sf::Time duration);
  virtual void addGuardingPoint(sf::Vector2f);

  int targetId = 0;
  bool isDead();
 protected:

  enum EnemyState { Guard, Chase, Attack, Dying, Died };
  EnemyState _enemyState;

  //sf::Vector2f guardingPoint;
  std::vector<sf::Vector2f> guardingPoints;

  //if repeated can be made into a struct
  float slowFactor = 1;
  sf::Clock slowClock;
  sf::Time slowDuration;

  sf::Vector2f _guardPosition;
  sf::Vector2f _targetPosition;

  //check if target reached
  virtual bool reachedTargetPosition(sf::Vector2f,sf::Vector2f);
  virtual double updatePosition(sf::Time elapsedTime, sf::Vector2f targetPos);
  virtual bool isPlayerInRadius(int);
  virtual void damagePlayer();

};
