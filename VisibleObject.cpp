#include "VisibleObject.hpp"
#include "ImageManager.hpp"
#include "Game.hpp"

VisibleObject::VisibleObject() : _isLoaded(false)
{

}

VisibleObject::VisibleObject(int x, int y) : _isLoaded(false)
{
  this->startingPosition.x = x;
  this->startingPosition.y = y;
}



VisibleObject::~VisibleObject()
{
}

void VisibleObject::Load(Animation& animation, std::string animationID)
{
  bool loaded = Game::imageManager.createAnimation(animation,_texture,
                                                   animationID);
  _isLoaded = loaded;
}

void VisibleObject::Draw(sf::RenderWindow & renderWindow){
  if(_isLoaded){
    renderWindow.draw(_animatedSprite);
  }
}

void VisibleObject::Update(sf::Time elapsedTime)
{
  prevPosition = this->GetPosition();
}

void VisibleObject::SetPosition(float x, float y){
  if(_isLoaded){
    _animatedSprite.setPosition(sf::Vector2f(x,y));
  }
}

sf::Vector2f VisibleObject::GetPosition() const
{
  if(_isLoaded){
    return _animatedSprite.getPosition();
  }
  return sf::Vector2f();
}

sf::Vector2f VisibleObject::GetCenterPosition() const
{
  if(_isLoaded){
    sf::Vector2f pos = _animatedSprite.getPosition();
    pos.x += this->GetWidth()/2;
    pos.y += this->GetHeight()/2;
    return pos;
  }
  return sf::Vector2f();
}


float VisibleObject::GetWidth() const
{
  if(_isLoaded){
    return _animatedSprite.getLocalBounds().width;
  }
  return -1;
}

float VisibleObject::GetHeight() const
{
  if(_isLoaded){
    return _animatedSprite.getLocalBounds().height;
  }
  return -1;
}


bool VisibleObject::IsLoaded() const
{
  return _isLoaded;
}

void VisibleObject::setPositionXToPrevious(int direction)
{
  this->SetPosition(prevPosition.x + (direction * 1),
                    _animatedSprite.getPosition().y);
}

void VisibleObject::setPositionYToPrevious(int direction)
{
  this->SetPosition(_animatedSprite.getPosition().x,
                    prevPosition.y + (direction * 1));
}

sf::Vector2f VisibleObject::getPreviousPosition()
{
  return this->prevPosition;
}

sf::Rect<float> VisibleObject::GetBoundingRect() const
{
  sf::Vector2f sizeOfTexture(_animatedSprite.getLocalBounds().width,
                             _animatedSprite.getLocalBounds().height);
	sf::Vector2f size = sizeOfTexture;
	sf::Vector2f position = _animatedSprite.getPosition();
	//get scale and multiply
	return sf::Rect<float>(
						position.x + size.x * _animatedSprite.getScale().x * 0.33,
						position.y,
						size.x * _animatedSprite.getScale().x * 0.33,
						size.y * _animatedSprite.getScale().y
						);
}

AnimatedSprite& VisibleObject::GetSprite()
{
  return _animatedSprite;
}

bool VisibleObject::Collided()
{
  return _collided;
}

void VisibleObject::setCollision(bool val)
{
  this->_collided = val;
}

bool VisibleObject::isAttacking()
{
  return _attacking;
}

void VisibleObject::setAttacking(bool val)
{
  this->_attacking = val;
}

sf::Vector2f VisibleObject::GetSpeed()
{
  return this->speed;
}

void VisibleObject::SetSpeedX(float newSpeed)
{
  this->speed.x = newSpeed;
}

void VisibleObject::SetSpeedY(float newSpeed)
{
  this->speed.y = newSpeed;
}

void VisibleObject::setDamage(int value)
{
  this->HP -= value;
}

int VisibleObject::getHP()
{
  return this->HP;
}

int VisibleObject::getPower()
{
  return this->power;
}

void VisibleObject::SetStartingPosition(int x, int y)
{
  this->startingPosition.x = x;
  this->startingPosition.y = y;
}

void VisibleObject::resetObject()
{
  this->SetPosition(startingPosition.x,startingPosition.y);
}

sf::CircleShape VisibleObject::GetSpellCircle()
{
  return sf::CircleShape();
}

void VisibleObject::setSlowFactor(float factor,sf::Time duration)
{

}

bool VisibleObject::isDead()
{

}
