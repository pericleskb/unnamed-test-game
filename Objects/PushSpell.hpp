#pragma once
#include "Spell.hpp"

class PushSpell: public Spell
{
public:
  PushSpell(float x,float y) ;
  ~PushSpell();
  void SpellHit();
  virtual void Update(sf::Time timeDelta);
  virtual void UpdateRadius();
  virtual void SetSpellPosition(float x, float y);
private:
  sf::Time _spellDuration;
};
