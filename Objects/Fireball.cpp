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
      this->SetPosition(x,y);
      break;
    }
    case UP:
    {
      this->Load(fireball_up,animationIDs[1]);
      this->currentAnimation = &(this->fireball_up);
      this->SetSpeedX(0);
      this->SetSpeedY(-400);
      this->SetPosition(x,y);
      break;
    }
    case RIGHT:
    {
      this->Load(fireball_right,animationIDs[2]);
      this->currentAnimation = &(this->fireball_right);
      this->SetSpeedX(400);
      this->SetSpeedY(0);
      this->SetPosition(x,y);
      break;
    }
    case DOWN:
    {
      this->Load(fireball_down,animationIDs[3]);
      this->currentAnimation = &(this->fireball_down);
      this->SetSpeedX(0);
      this->SetSpeedY(400);
      this->SetPosition(x,y);
      break;
    }
  }
	_animatedSprite.setFrameTime(sf::seconds(0.1));
	_animatedSprite.setLooped(false);
  _animatedSprite.setScale(1.3f,1.3f);
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

sf::Rect<float> Fireball::GetBoundingRect() const
{
  sf::Vector2f sizeOfTexture(_animatedSprite.getLocalBounds().width,
                             _animatedSprite.getLocalBounds().height);
	sf::Vector2f size = sizeOfTexture;
	sf::Vector2f position = _animatedSprite.getPosition();

  if(this->currentAnimation == &(this->fireball_left))
  {
    return sf::Rect<float>(
        position.x + size.x * _animatedSprite.getScale().x * 0.1,
        position.y + size.y * _animatedSprite.getScale().y * 0.40,
        size.x * _animatedSprite.getScale().x * 0.5,
        size.y * _animatedSprite.getScale().y * 0.20
        );
  }
  else if(this->currentAnimation == &(this->fireball_up))
  {
    return sf::Rect<float>(
        position.x + size.x * _animatedSprite.getScale().x * 0.45,
        position.y + size.y * _animatedSprite.getScale().y * 0.25,
        size.x * _animatedSprite.getScale().x * 0.15,
        size.y * _animatedSprite.getScale().y * 0.5
        );
  }
  else if(this->currentAnimation == &(this->fireball_right))
  {
    return sf::Rect<float>(
        position.x + size.x * _animatedSprite.getScale().x * 0.25,
        position.y + size.y * _animatedSprite.getScale().y * 0.40,
        size.x * _animatedSprite.getScale().x * 0.66,
        size.y * _animatedSprite.getScale().y * 0.20
        );
  }
  else if(this->currentAnimation == &(this->fireball_down))
  {
    return sf::Rect<float>(
        position.x + size.x * _animatedSprite.getScale().x * 0.45,
        position.y + size.y * _animatedSprite.getScale().y * 0.25,
        size.x * _animatedSprite.getScale().x * 0.15,
        size.y * _animatedSprite.getScale().y * 0.5
        );
  }

}

sf::Rect<float> Fireball:: GetBorderBoundingRect() const
{
	sf::Rect<float> rect = this->GetBoundingRect();
	rect.top += rect.height * 0.75;
	rect.height = rect.height *0.25;
  return rect;
}
