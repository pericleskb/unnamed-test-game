#include "VisibleObject.hpp"
#include "../ImageManager.hpp"
#include "../Game.hpp"

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

  /*sf::Rect<float> temp = this->GetBorderBoundingRect();
  this->BoundingRect.setSize(sf::Vector2f(temp.width,temp.height));
  this->BoundingRect.setFillColor(sf::Color(0,0,0,0));
  this->BoundingRect.setOutlineThickness(1);
  this->BoundingRect.setOutlineColor(sf::Color(250, 0, 0));
  sf::Vector2i mpla;
  sf::Vector2f localPosition(temp.left,temp.top);
  this->BoundingRect.setPosition(localPosition);
  renderWindow.draw(this->BoundingRect);

  sf::CircleShape kyklos(3,30);
  kyklos.setOrigin(kyklos.getRadius()/2,kyklos.getRadius()/2);
  kyklos.setPosition(this->GetBottomLeftPosition());
  kyklos.setFillColor(sf::Color(255,0,0));
  renderWindow.draw(kyklos);
*/
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
    pos.x += this->GetWidth()/2 * this->_animatedSprite.getScale().x;
    pos.y += this->GetHeight()/2  * this->_animatedSprite.getScale().y;
    return pos;
  }
  return sf::Vector2f();
}

sf::Vector2f VisibleObject::GetBottomLeftPosition() const
{
  if(_isLoaded)
  {
    sf::Rect<float> rect = this->GetBorderBoundingRect();
    sf::Vector2f pos(rect.left,rect.top +  rect.height);
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
						position.x + size.x * _animatedSprite.getScale().x * 0.25,
						position.y + size.y * _animatedSprite.getScale().y * 0.2,
						size.x * _animatedSprite.getScale().x * 0.5,
						size.y * _animatedSprite.getScale().y * 0.75
						);
}

sf::Rect<float> VisibleObject:: GetBorderBoundingRect() const
{
	sf::Rect<float> rect = this->GetBoundingRect();
	rect.top += rect.height * 0.75;
	rect.height = rect.height *0.25;
	rect.left += rect.width * 0.1;
	rect.width -= rect.width *0.2;
  return rect;
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

float VisibleObject::getHpPercentage()
{

}

float VisibleObject::getManaPercentage()
{

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

void VisibleObject::SetPush(double angle, sf::Time time)
{

}

bool VisibleObject::IsPushed()
{
  return _isPushed;
}

enum VisibleObject::EnemyState VisibleObject::getEnemyState()
{

};
