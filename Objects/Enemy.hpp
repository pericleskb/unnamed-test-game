#pragma once
#include "VisibleObject.hpp"
#include <list>


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
  virtual void SetPush(double angle,sf::Time time);
  enum EnemyState getEnemyState();

 protected:

  EnemyState _enemyState;

  std::vector<sf::Vector2f> guardingPoints;
  std::list<sf::Vector2i> pathPoints;

  float slowFactor = 1;
  sf::Clock slowClock;
  sf::Time slowDuration;

  sf::Vector2f _guardPosition;
  sf::Vector2f _targetPosition;

  double pushedAngle;
  sf::Time pushDuration;

  virtual bool reachedTargetPosition(sf::Vector2f,sf::Vector2f);
  double updateBottomLeftPosition(sf::Time timeDelta, sf::Vector2f targetPos);
  virtual bool isPlayerInRadius(int);
  virtual void damagePlayer();

 private:
  double getAngle(sf::Vector2f pos, sf::Vector2f tar, sf::Time timeDelta);

};
