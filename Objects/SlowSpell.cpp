#include "SlowSpell.hpp"
#include "../Game.hpp"

SlowSpell::SlowSpell(float x,float y)
          : Spell(x,y,"slow_spell")
{
  this->manaCost = 2;

}

SlowSpell::~SlowSpell()
{

}

//radius grows with each update call
void SlowSpell::SpellHit()
{

  std::vector<VisibleObject*> objectsToHit;
  Game::GetObjectsInRadius(objectsToHit,this->circle);

  for(std::vector<VisibleObject*>::iterator objectItr =
       objectsToHit.begin(); objectItr!=objectsToHit.end();++objectItr)
  {
    (*objectItr)->setSlowFactor(0.5,slowDuration);
  }

}
