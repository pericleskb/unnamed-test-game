#pragma once
#include <iostream>
#include "VisibleObject.hpp"
#include "Fireball.hpp"

#define FULL_HP 3;

class Player: public VisibleObject
{
  public:
    Player(int x,int y);
    ~Player();

    void Update(sf::Time elapsedTime);
    void Draw(sf::RenderWindow & window);

    void resetObject();
    sf::CircleShape GetSpellCircle();

  private:

    void checkButtonsPressed();

    sf::Time slowDuration = sf::seconds(5.f);
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
    /*std::string player_down = "player_down";
    std::string player_up = "player_up";
    std::string player_right = "player_right";
    std::string player_left = "player_left";*/
};
