#include "HealSpell.hpp"

HealSpell::HealSpell(float x,float y)
          : Spell(x,y,"heal_spell")
{
	_animatedSprite.setColor(sf::Color(255,50,90,190));

}

HealSpell::~HealSpell()
{

}

void HealSpell::SetSpellPosition(float x, float y)
{
  this->SetPosition(x,y);
}
