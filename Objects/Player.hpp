#pragma once
#include <iostream>
#include "VisibleObject.hpp"
#include "Fireball.hpp"
#include "Spell.hpp"


class Player: public VisibleObject
{
  public:
    Player(int x,int y);
    ~Player();

    void Update(sf::Time elapsedTime);
    void Draw(sf::RenderWindow & window);

    void resetObject();
    sf::CircleShape GetSpellCircle();

    float getHpPercentage();
    float getManaPercentage();
  private:

    void checkButtonsPressed();
    bool mouseLeftPressedOnce = false;
    bool firstClick = false;

    //for testing
    sf::CircleShape _paintedShape;

    enum PlayerState { Walking, Dying, Attacking };
    PlayerState _playerState;

    Fireball* _fireball = NULL;

    Animation cast_up,cast_down,cast_right,cast_left,
              player_up,player_down,player_right,player_left,
              player_dead;

    std::vector<Animation*> animations;
    std::string animationIDs[9] =
            {"cast_up","cast_left","cast_down","cast_right",
             "hero_up","hero_left","hero_down","hero_right",
             "hero_dead"};

    bool _spellSelected = false;

    Animation* _previousAnimation;

    sf::Rect<float> fireSpell,slowSpell,healSpell,pushSpell;
    void SpellCasted();
    void SlowSpellSelected();
    void PushSpellSelected();
    void ShootFireball();
    void HealSpellSelected();

    Spell* selectedSpell = NULL;
    Spell* castedSpell = NULL;
    std::string _slowSpell ="slow_spell";
    std::string _pushSpell ="push_spell";
};
