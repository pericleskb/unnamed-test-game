#include "Spell.hpp"
#define MAX_RADIUS 106

Spell::Spell()
{

}

Spell::Spell(float x, float y,const char* animationID)
{
  std::string animationName(animationID);
  this->Load(spellAnim,animationName);
  this->currentAnimation = &(this->spellAnim);
  this->SetPosition(x,y);

	_animatedSprite.setFrameTime(sf::seconds(0.03));
	_animatedSprite.setLooped(true);
	_animatedSprite.setColor(sf::Color(255,255,255,180));
  _animatedSprite.play(*currentAnimation);

  this->_animatedSprite.setScale(sf::Vector2f(2.f,2.f));
  sf::Vector2f origin(_animatedSprite.getLocalBounds().width/2,
                      _animatedSprite.getLocalBounds().height/2);
  _animatedSprite.setOrigin(origin);

}

Spell::~Spell()
{

}

void Spell::Update(sf::Time timeDelta)
{
  _animatedSprite.play(*currentAnimation);
  _animatedSprite.update(timeDelta);
  this->circle.setPosition(this->GetPosition());
  if(radius!= 0)//spell has been casted
    this->UpdateRadius();
}

void Spell::spellCast()
{
  this->_animatedSprite.setLooped(false);
  this->_animatedSprite.setAnimation(this->spellAnim);
  this->_clock.restart();
  this->UpdateRadius(); //we increment first so we won't divide by zero

  this->circle.setFillColor(sf::Color(200, 50, 50,125));
}

bool Spell::HasAnimationStopped()
{
  if(this->_clock.getElapsedTime().asSeconds() >=
               0.03 * this->spellAnim.getSize())
  {
    return true;
  }
  return false;
}

void Spell::UpdateRadius()
{
  if(radius < MAX_RADIUS)
    radius +=0.65 + (MAX_RADIUS / this->spellAnim.getSize());
  this->circle.setRadius(radius);
  this->circle.setOrigin(radius,radius);
  this->circle.setPosition(this->GetPosition().x,this->GetPosition().y);
}

void Spell::SetSpellPosition(float x, float y)
{

}

void Spell::SpellHit()
{
}

int Spell::getManaCost()
{
  return this->manaCost;
}


