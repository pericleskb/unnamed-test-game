#pragma once
#include "Spell.hpp"

class SlowSpell: public Spell
{
public:
  SlowSpell(float x,float y) ;
  ~SlowSpell();
  void SpellHit();
private:
  sf::Time slowDuration = sf::seconds(5.f);
};
