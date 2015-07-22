#pragma once
#include "VisibleObject.hpp"

class Spell: public VisibleObject
{
public:

  Spell();
  Spell(float x,float y, const char* animationID);
  ~Spell();
  virtual void Update(sf::Time elapsedTime);
  sf::CircleShape circle;
  void spellCast();
  bool HasAnimationStopped();
  virtual void UpdateRadius();
  virtual void SetSpellPosition(float x, float y);
  virtual void SpellHit();
  int getManaCost();

protected:
  Animation spellAnim;
  double radius = 0;
  int manaCost = 0;
};
