#include "EnemyOrc.hpp"
#include <assert.h>
#include "../Game.hpp"

#define ORC_SPEED 200
#define ORC_POWER 3
#define FULL_HP 5;
#define PLAYERS_POSITION_UPDATE_INTERVAL 1.5f
#define GUARD_RADIUS  240
#define CHASE_RADIUS  360
#define ATTACK_RADIUS 75

EnemyOrc::EnemyOrc(int x, int y) : Enemy(x,y)
{
  this->SetSpeedX(ORC_SPEED);
  this->SetSpeedY(ORC_SPEED);

  this->animations.push_back(&orc_attack_up);
  this->animations.push_back(&orc_attack_left);
  this->animations.push_back(&orc_attack_down);
  this->animations.push_back(&orc_attack_right);
  this->animations.push_back(&orc_up);
  this->animations.push_back(&orc_left);
  this->animations.push_back(&orc_down);
  this->animations.push_back(&orc_right);
  this->animations.push_back(&orc_dying);

  for(int i=0; i<this->animations.size();i++)
  {
    this->Load(*animations[i],animationIDs[i]);
    assert(IsLoaded());
    this->animations[i]->setSpriteSheet(_texture);
  }

	this->currentAnimation = &(this->orc_left);
	_animatedSprite.setFrameTime(sf::seconds(0.1));
	_animatedSprite.setLooped(false);

  this->SetPosition(x,y);


 this->GetSprite().setScale(1.8,1.8);
  _animatedSprite.play(*currentAnimation);

  this->_enemyState = EnemyOrc::Guard;
  this->_targetPosition = this->GetCenterPosition();


  this->power = ORC_POWER;
  this->HP = FULL_HP;

}

EnemyOrc::~EnemyOrc(){
}

//for testing
void EnemyOrc::Draw(sf::RenderWindow& window)
{
  VisibleObject::Draw(window);
  this->viewRadiusCircle.setPosition(this->GetCenterPosition());
  this->viewRadiusCircle.setOrigin( this->viewRadiusCircle.getRadius(),
                                    this->viewRadiusCircle.getRadius());
  window.draw(this->viewRadiusCircle);

}

void EnemyOrc::Update(sf::Time timeDelta)
{
  Enemy::Update(timeDelta);

  if(HP <= 0 && this->_enemyState != Enemy::Dying)
  {
    this->_enemyState =  Enemy::Dying;
    this->currentAnimation = &(this->orc_dying);
    this->_clock.restart();
  }

  switch(this->_enemyState)
  {
    case EnemyOrc::Guard:
    {

      this->viewRadiusCircle.setFillColor(sf::Color(0,0,255,40));
      this->viewRadiusCircle.setRadius(GUARD_RADIUS);

      if(isPlayerInRadius(GUARD_RADIUS))
      {
        this->pathPoints.clear();
        sf::Rect<float> EnemyRect = Game::getPlayersBorderRect();
        sf::Rect<float> borderRect = this->GetBorderBoundingRect();

        if(!Game::FindPath(this->GetBottomLeftPosition(), EnemyRect,
                            (this->pathPoints),borderRect))
        {
          break;
        }

        if(pathPoints.size() <= 26)
        {
          this->_enemyState = EnemyOrc::Chase;
          this->_clock.restart();
          break;
        }
      }
      if(reachedTargetPosition(this->GetBottomLeftPosition(),
                              _targetPosition) || this->Collided())
      {
          updateGuardTarget();
      }
      double angle = updateBottomLeftPosition(timeDelta,_targetPosition);

      if(angle > -135 && angle < -45)
        this->currentAnimation = &(this->orc_up);
      else if(angle > -45 && angle < 45)
        this->currentAnimation = &(this->orc_right);
      else if(angle > 45 && angle < 135)
        this->currentAnimation = &(this->orc_down);
      else
        this->currentAnimation = &(this->orc_left);

      break;
    }
    case EnemyOrc::Chase:
    {

      this->viewRadiusCircle.setFillColor(sf::Color(255,0,0,40));
      this->viewRadiusCircle.setRadius(CHASE_RADIUS);

      sf::Vector2f tempTarget;
      if(isPlayerInRadius(ATTACK_RADIUS))
      {
        this->_enemyState = EnemyOrc::Attack;
        this->pathPoints.clear();
        break;
      }
      //first time in chase
      else if(isPlayerInRadius(CHASE_RADIUS))
      {
        if(_clock.getElapsedTime().asSeconds() >
             PLAYERS_POSITION_UPDATE_INTERVAL ||  pathPoints.empty())
        {
          this->pathPoints.clear();
          sf::Rect<float> EnemyRect = Game::getPlayersBorderRect();
          sf::Rect<float> borderRect = this->GetBorderBoundingRect();

          if(!Game::FindPath(this->GetBottomLeftPosition(), EnemyRect,
                              (this->pathPoints),borderRect))
          {
            break;
          }
          tempTarget =  sf::Vector2f((this->pathPoints.front()).x,
                                                (this->pathPoints.front()).y);
          this->_clock.restart();
        }
        else
        {
          tempTarget =  sf::Vector2f((this->pathPoints.front()).x,
                                          (this->pathPoints.front()).y);
          if(reachedTargetPosition(this->GetBottomLeftPosition(),tempTarget))
          {
            this->pathPoints.pop_front();
            if(this->pathPoints.empty())
            {
              break;
            }
          }
        }
      }
      else if(!isPlayerInRadius(CHASE_RADIUS) && !pathPoints.empty())
      {
        tempTarget =  sf::Vector2f((this->pathPoints.front()).x,
                                        (this->pathPoints.front()).y);
        if(reachedTargetPosition(this->GetBottomLeftPosition(),tempTarget))
        {
          this->pathPoints.pop_front();
          if(this->pathPoints.empty())
          {
            break;
          }
        }
      }
      else//find return path
      {
        this->_enemyState = EnemyOrc::Return;

        if(!this->pathPoints.empty())
        {
          this->pathPoints.clear();
        }
        sf::Rect<float> TargetRect(guardingPoints[targetId].x,
                                   guardingPoints[targetId].y,0,0);

        sf::Rect<float> borderRect = this->GetBorderBoundingRect();

        if(!Game::FindPath(this->GetBottomLeftPosition(), TargetRect, (this->pathPoints),
                      borderRect))
        {
            break;
        }
      }
       double angle = updateBottomLeftPosition(timeDelta,tempTarget);

        if(angle > -135 && angle < -45)
          this->currentAnimation = &(this->orc_up);
        else if(angle > -45 && angle < 45)
          this->currentAnimation = &(this->orc_right);
        else if(angle > 45 && angle < 135)
          this->currentAnimation = &(this->orc_down);
        else
          this->currentAnimation = &(this->orc_left);
        break;
    }
    case EnemyOrc::Attack:
    {
      this->viewRadiusCircle.setFillColor(sf::Color(255,0,255,40));
      this->viewRadiusCircle.setRadius(ATTACK_RADIUS);
      //first time
      if(isPlayerInRadius(ATTACK_RADIUS) && isAttacking() == false)
      {
        this->_clock.restart();
        setAttacking(true);

        double angle = updateBottomLeftPosition(timeDelta,
                                          Game::getPlayerBottomLeftPosition());

        if(angle > -135 && angle < -45)
          this->currentAnimation = &(this->orc_attack_up);
        else if(angle > -45 && angle < 45)
          this->currentAnimation = &(this->orc_attack_right);
        else if(angle > 45 && angle < 135)
          this->currentAnimation = &(this->orc_attack_down);
        else
          this->currentAnimation = &(this->orc_attack_left);
      }
      //when _attacking ends
      else if(isAttacking() == true  && _clock.getElapsedTime().asSeconds() >= 0.1 *
            this->orc_attack_down.getSize())
      {
        checkIfHit();
        this->_clock.restart();
      }
      else if(isAttacking() == true  && _clock.getElapsedTime().asSeconds() < 0.1 *
            this->orc_attack_down.getSize())
      {
        sf::Vector2f playersPos = Game::getPlayerBottomLeftPosition();
        sf::Vector2f movementVector(playersPos.x - this->GetBottomLeftPosition().x,
                        playersPos.y - this->GetBottomLeftPosition().y);
        double angle = atan2(movementVector.y,movementVector.x) * 180 / PI;

        //restart clock if different animation than previous
        if(angle > -135 && angle < -45)
          this->currentAnimation = &(this->orc_attack_up);
        else if(angle > -45 && angle < 45)
          this->currentAnimation = &(this->orc_attack_right);
        else if(angle > 45 && angle < 135)
          this->currentAnimation = &(this->orc_attack_down);
        else
          this->currentAnimation = &(this->orc_attack_left);
      }

      if(!isPlayerInRadius(ATTACK_RADIUS))
      {
        this->_enemyState = Enemy::Chase;
        this->_clock.restart();
        this->setAttacking(false);

      }

      break;
    }
    case EnemyOrc::Return:
    {
      this->viewRadiusCircle.setFillColor(sf::Color(255,255,0,40));
      this->viewRadiusCircle.setRadius(CHASE_RADIUS);
      //check to see if player in chase radius
      if(isPlayerInRadius(CHASE_RADIUS))
      {
        this->pathPoints.clear();
        std::list<sf::Vector2i> tempPathPoints;
        sf::Rect<float> EnemyRect = Game::getPlayersBorderRect();
        sf::Rect<float> borderRect = this->GetBorderBoundingRect();

        if(!Game::FindPath(this->GetBottomLeftPosition(), EnemyRect,
                            tempPathPoints,borderRect))
        {
          break;
        }
        if(tempPathPoints.size() <= 26)
        {
          this->_enemyState = EnemyOrc::Chase;
          this->_clock.restart();
          break;
        }
        else
        {

        }
      }

      sf::Vector2f tempTarget((this->pathPoints.front()).x,(this->pathPoints.front()).y);
      if(reachedTargetPosition(this->GetBottomLeftPosition(),tempTarget))
      {
        this->pathPoints.pop_front();
        if(this->pathPoints.empty())
        {
          this->_enemyState = Enemy::Guard;
          this->setCollision(false);
          break;
        }
        tempTarget = sf::Vector2f((this->pathPoints.front()).x,(this->pathPoints.front()).y);
      }
      double angle = updateBottomLeftPosition(timeDelta,tempTarget);

      if(angle > -135 && angle < -45)
        this->currentAnimation = &(this->orc_up);
      else if(angle > -45 && angle < 45)
        this->currentAnimation = &(this->orc_right);
      else if(angle > 45 && angle < 135)
        this->currentAnimation = &(this->orc_down);
      else
        this->currentAnimation = &(this->orc_left);
      break;
    }
    case EnemyOrc::Dying:
    {
      if( _clock.getElapsedTime().asSeconds() < 0.1 *
          (this->orc_dying.getSize() - 1))
      {

      }
      else if(_clock.getElapsedTime().asSeconds() < 0.2 * this->orc_dying.getSize())
      {
        _animatedSprite.setFrame(5, 2);
      }
      else
      {
        this->_enemyState =  Enemy::Died;
      }
      break;
    }
    case EnemyOrc::Pushed:
    {
      //push
      if( _clock.getElapsedTime().asSeconds() < pushDuration.asSeconds())
      {
        this->GetSprite().move(70 * timeDelta.asSeconds() * cos(pushedAngle * PI / 180.0),
                  70  * timeDelta.asSeconds() * sin(pushedAngle * PI / 180.0));
      }
      //animation
      if( _clock.getElapsedTime().asSeconds() < 0.1 *
          (this->orc_dying.getSize() - 1))
      {
      }
      else if(_clock.getElapsedTime().asSeconds() <=
                                        this->pushDuration.asSeconds())
      {
        _animatedSprite.setFrame(5, 2);
      }
      else
      {
        this->_enemyState =  Enemy::Chase;
        this->_clock.restart();
        this->_isPushed = false;
      }
      break;
    }
    default:
      break;
  }


  _animatedSprite.play(*currentAnimation);

  _animatedSprite.update(timeDelta);

  this->setCollision(false);
}

void EnemyOrc::checkIfHit()
{
  if(isPlayerInRadius(ATTACK_RADIUS))
    this->damagePlayer();
}

void EnemyOrc::resetObject()
{
  VisibleObject::resetObject();
  this->HP = FULL_HP;
  this->currentAnimation = &(this->orc_up);
  this->slowFactor = SLOW_FACTOR;
  this->targetId = 0;
  this->_isPushed = false;
  this->_enemyState = Enemy::Guard;
}

void EnemyOrc::SetPush(double angle, sf::Time time)
{
  Enemy::SetPush(angle,time);
  this->_isPushed = true;
  this->currentAnimation = &(this->orc_dying);
  this->_clock.restart();
  //also setDamage();
}

void EnemyOrc::setDamage(int value)
{
  VisibleObject::setDamage(value);
  if(this->_enemyState == EnemyOrc::Guard ||
     this->_enemyState == Enemy::Return)
  {
    this->pathPoints.clear();
    sf::Rect<float> EnemyRect = Game::getPlayersBorderRect();
    sf::Rect<float> borderRect = this->GetBorderBoundingRect();

    if(!Game::FindPath(this->GetBottomLeftPosition(), EnemyRect,
                        (this->pathPoints),borderRect))
    {
      return;
    }

    this->_enemyState = EnemyOrc::Chase;
    this->_clock.restart();
  }
}
