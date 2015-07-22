#include "PushSpell.hpp"
#include "../Game.hpp"


#define MAX_RADIUS 106
PushSpell::PushSpell(float x,float y)
          : Spell(x,y,"push_spell")
{
  this->_spellDuration =  sf::seconds(0.03f * this->spellAnim.getSize());
  this->_clock.restart();
  this->UpdateRadius(); //we increment first so we won't divide by zero

	_animatedSprite.setColor(sf::Color(0,0,255,190));
  this->circle.setFillColor(sf::Color(200, 50, 50,125));
  this->power = 1;
  this->manaCost = 2;
}

PushSpell::~PushSpell()
{

}

void PushSpell::Update(sf::Time timeDelta)
{
  _animatedSprite.play(*currentAnimation);
  _animatedSprite.update(timeDelta);
  if(radius!= 0)//spell has been casted
    this->UpdateRadius();
}

void PushSpell::UpdateRadius()
{
  if(radius < MAX_RADIUS)
    radius +=1.5f + (MAX_RADIUS / this->spellAnim.getSize());
  this->circle.setRadius(radius);
  this->circle.setOrigin(radius,radius);
  this->circle.setPosition(this->GetPosition().x,this->GetPosition().y);
}

//radius grows with each update call
void PushSpell::SpellHit()
{
  std::vector<VisibleObject*> objectsToHit;
  Game::GetObjectsInRadius(objectsToHit,this->circle);

  for(std::vector<VisibleObject*>::iterator objectItr =
       objectsToHit.begin(); objectItr!=objectsToHit.end();++objectItr)
  {
    if(!(*objectItr)->IsPushed())
    {
      sf::Vector2f tar((*objectItr)->GetCenterPosition());
      sf::Vector2f pos = Game::getPlayersPosition();

      sf::Vector2f movementVector(tar.x - pos.x,tar.y - pos.y);
      double angle = atan2(movementVector.y,movementVector.x) * 180 / PI;

      sf::Time time = sf::seconds(_spellDuration.asSeconds() -
                                     _clock.getElapsedTime().asSeconds());
      (*objectItr)->SetPush(angle ,time);
      (*objectItr)->setDamage(this->power);
    }
  }
}


void PushSpell::SetSpellPosition(float x, float y)
{
  this->SetPosition(x,y);
}


