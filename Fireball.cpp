#pragma once
#include "Fireball.hpp"
#include <assert.h>

#define LEFT  0
#define UP    1
#define RIGHT 2
#define DOWN 3

//the constructor could take as a formal argument the power of the player.
//and multiply it with the power of the fireball in case we want to make the
//player stronger as he progresses
Fireball::Fireball(int x, int y, int direction)
{

  //must be in the same order as in animationIDs
/*
  this->animations.push_back(&fireball_left);
  this->animations.push_back(&fireball_up);
  this->animations.push_back(&fireball_right);
  this->animations.push_back(&fireball_down);

  for(unsigned int i=0; i < this->animations.size(); i++)
  {
    assert(IsLoaded());
    this->animations[i]->setSpriteSheet(_texture);
  }
*/
  switch(direction)
  {
    case LEFT:
    {
      this->Load(fireball_left,animationIDs[0]);
      this->currentAnimation = &(this->fireball_left);
      this->SetSpeedX(-400);
      this->SetSpeedY(0);
      break;
    }
    case UP:
    {
      this->Load(fireball_up,animationIDs[1]);
      this->currentAnimation = &(this->fireball_up);
      this->SetSpeedX(0);
      this->SetSpeedY(-400);
      break;
    }
    case RIGHT:
    {
      this->Load(fireball_right,animationIDs[2]);
      this->currentAnimation = &(this->fireball_right);
      this->SetSpeedX(400);
      this->SetSpeedY(0);
      break;
    }
    case DOWN:
    {
      this->Load(fireball_down,animationIDs[3]);
      this->currentAnimation = &(this->fireball_down);
      this->SetSpeedX(0);
      this->SetSpeedY(400);
      break;
    }
  }
	_animatedSprite.setFrameTime(sf::seconds(0.1));
	_animatedSprite.setLooped(false);

  this->SetPosition(x,y);
  _animatedSprite.play(*currentAnimation);

  this->power = 3;
}

Fireball::~Fireball()
{

}

void Fireball::Update(sf::Time timeDelta)
{

  this->GetSprite().move(this->GetSpeed().x * timeDelta.asSeconds(),
                         this->GetSpeed().y * timeDelta.asSeconds());

  _animatedSprite.play(*currentAnimation);


  _animatedSprite.update(timeDelta);
}
