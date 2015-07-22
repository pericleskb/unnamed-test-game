#pragma once
#include "Spell.hpp"

class HealSpell: public Spell
{
public:
  HealSpell(float x,float y) ;
  ~HealSpell();
  virtual void SetSpellPosition(float x, float y);
  //virtual void Update(sf::Time timeDelta);
};

