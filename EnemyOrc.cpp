#include "EnemyOrc.hpp"
#include <assert.h>
#include "Game.hpp"

#define ORC_SPEED 200


int speed = 200;
std::string EnemyTarget = "player";

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


  this->power = 1;
  this->HP = 5;
/*
  this->chaseRadius = 200;
  this->attackRadius = 80;
  */
}

EnemyOrc::~EnemyOrc(){
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
      //when exiting from attack or chase states _targetPosition.x becomes -1
      if(_targetPosition.x == -1)
      {
        _targetPosition = this->GetCenterPosition();
        targetId = 0;
        updateGuardTarget();
      }
      else if(isPlayerInRadius(chaseRadius))
      {
        this->_enemyState = EnemyOrc::Chase;
        this->_targetPosition = Game::getPlayersPosition();
        break;
      }
      else
      {


        if(reachedTargetPosition(this->GetCenterPosition(),_targetPosition) ||
           this->Collided())
        {
          updateGuardTarget();
        }
          double angle = updatePosition(timeDelta,_targetPosition);

          if(angle > -135 && angle < -45)
            this->currentAnimation = &(this->orc_up);
          else if(angle > -45 && angle < 45)
            this->currentAnimation = &(this->orc_right);
          else if(angle > 45 && angle < 135)
            this->currentAnimation = &(this->orc_down);
          else
            this->currentAnimation = &(this->orc_left);

      }
      break;
    }
    case EnemyOrc::Chase:
    {
      if(isPlayerInRadius(attackRadius))
      {
        this->_enemyState = EnemyOrc::Attack;
      }
      else if(isPlayerInRadius(chaseRadius))
      {
        double angle = updatePosition(timeDelta,Game::getPlayersPosition());

        if(angle > -135 && angle < -45)
          this->currentAnimation = &(this->orc_up);
        else if(angle > -45 && angle < 45)
          this->currentAnimation = &(this->orc_right);
        else if(angle > 45 && angle < 135)
          this->currentAnimation = &(this->orc_down);
        else
          this->currentAnimation = &(this->orc_left);
      }
      else
      {
        this->_enemyState = EnemyOrc::Guard;
        _targetPosition.x = -1;

      }
      break;
    }
    case EnemyOrc::Attack:
    {
      //first time
      if(isPlayerInRadius(attackRadius) && isAttacking() == false)
      {
        this->_clock.restart();
        setAttacking(true);

        //so it won't move
        double angle = updatePosition(timeDelta,Game::getPlayersPosition());

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
        setAttacking(false);
      }
      else if(isPlayerInRadius(chaseRadius) && isAttacking() == false)
      {
        this->_enemyState = EnemyOrc::Chase;
      }
      else if(isAttacking() == false)
      {
        this->_enemyState = EnemyOrc::Guard;
        _targetPosition.x = -1;
      }
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
    default:
      break;
  }


  _animatedSprite.play(*currentAnimation);

  _animatedSprite.update(timeDelta);

  this->setCollision(false);
}

void EnemyOrc::checkIfHit()
{
  if(isPlayerInRadius(attackRadius))
    this->damagePlayer();
}

void EnemyOrc::resetObject()
{
  VisibleObject::resetObject();
  this->HP = FULL_HP;
  this->currentAnimation = &(this->orc_up);
  this->slowFactor = SLOW_FACTOR;
  this->targetId = 0;
}
