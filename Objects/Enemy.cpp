#include "Enemy.hpp"
#include <math.h>       /* atan2 */
#include "../Game.hpp"


Enemy::Enemy(){
}

Enemy::Enemy(int x, int y) : VisibleObject(x,y){

}

Enemy::~Enemy(){
}

void Enemy::Update(sf::Time timeDelta)
{
  VisibleObject::Update(timeDelta);
  if(slowFactor !=1 )
  {
    if(slowClock.getElapsedTime().asSeconds() > slowDuration.asSeconds())
      this->slowFactor = 1;
  }
}

double Enemy::getAngle(sf::Vector2f pos, sf::Vector2f tar,sf::Time timeDelta)
{

  sf::Vector2f movementVector(tar.x - pos.x,
                        tar.y - pos.y);
  double angle = atan2(movementVector.y,movementVector.x) * 180 / PI;
  this->GetSprite().move(this->GetSpeed().x * timeDelta.asSeconds() *
                          slowFactor * cos(angle * PI / 180.0),
                        this->GetSpeed().y  * timeDelta.asSeconds() *
                          slowFactor * sin(angle * PI / 180.0));
  return angle;
}

double Enemy::updateBottomLeftPosition(sf::Time timeDelta, sf::Vector2f targetPos)
{
  sf::Vector2f enemyPos = this->GetBottomLeftPosition();
  return getAngle(enemyPos,targetPos,timeDelta);
}

//
void Enemy::updateGuardTarget()
{
  this->targetId++;
  if(this->targetId == guardingPoints.size())
    this->targetId = 0;
  _targetPosition = guardingPoints[targetId];
}

void Enemy::setSlowFactor(float factor, sf::Time duration)
{
  this->slowDuration = duration;
  this->slowFactor = factor;
  this->slowClock.restart();
}

bool Enemy::reachedTargetPosition(sf::Vector2f target,sf::Vector2f current)
{
  if(abs(target.x - current.x) < 2 && abs(target.y - current.y) < 2)
    return true;
  return false;
}

bool Enemy::isPlayerInRadius(int radius)
{
  sf::Vector2f playerPos = Game::getPlayersPosition();
  sf::Vector2f enemyPos = this->GetCenterPosition();
  sf::Vector2f distanceVector(playerPos.x - enemyPos.x,
                          playerPos.y - enemyPos.y);
  if(sqrt(pow(distanceVector.x,2) + pow(distanceVector.y,2)) <= radius)
  {
    return true;
  }
  return false;
}

void Enemy::damagePlayer()
{
  VisibleObject* target;
  //we could have a protected string Enemy that will change if a fury spell
  // is cast and in the target section
  target = Game::getVisibleObject("player");
  target->setDamage(this->power);
}

void Enemy::addGuardingPoint(sf::Vector2f point)
{
  this->guardingPoints.push_back(point);
}

bool Enemy::isDead()
{
  if(this->_enemyState == Enemy::Died)
    return true;
  return false;
}

void Enemy::SetPush(double angle, sf::Time time)
{
 this->_enemyState = Enemy::Pushed;
 this->pushedAngle = angle;
 this->pushDuration = time;
}

enum Enemy::EnemyState Enemy::getEnemyState()
{
  return this->_enemyState;
};
