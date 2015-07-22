#include "Player.hpp"
#include <assert.h>
#include "../Game.hpp"
#include "SlowSpell.hpp"
#include "PushSpell.hpp"
#include "HealSpell.hpp"
#include <iostream>


//Some of these will be class variables
#define PLAYERS_STRAIGHT_SPEED 175
#define PLAYERS_DIAGONAL_SPEED 124 //straight_speed/(sqrt(2)
#define FIREBALL_COST 3
#define HEAL_COST 3
#define FULL_HP 10
#define FULL_MANA 100
#define HEAL_POINTS 5
#define LEFT  0
#define UP    1
#define RIGHT 2
#define DOWN 3


Player::Player(int x, int y) : VisibleObject(x,y)
{
  this->SetSpeedX(0);
  this->SetSpeedY(0);

  //must be in the same order as in animationIDs
  this->animations.push_back(&cast_up);
  this->animations.push_back(&cast_left);
  this->animations.push_back(&cast_down);
  this->animations.push_back(&cast_right);

  this->animations.push_back(&player_up);
  this->animations.push_back(&player_left);
  this->animations.push_back(&player_down);
  this->animations.push_back(&player_right);

  this->animations.push_back(&player_dead);

  for(unsigned int i=0; i < this->animations.size(); i++)
  {
    this->Load(*animations[i],animationIDs[i]);
    assert(IsLoaded());
    this->animations[i]->setSpriteSheet(_texture);
  }

	this->currentAnimation = &(this->player_right);
	_animatedSprite.setFrameTime(sf::seconds(0.1));
	_animatedSprite.setLooped(false);

  this->SetPosition(x,y);

  this->GetSprite().setScale(1.5,1.5);
  _animatedSprite.play(*currentAnimation);
  this->_playerState = Player::Walking;

  this->power = 1;
  this->HP = FULL_HP;
  this->mana = FULL_MANA;
}

Player::~Player()
{

}

void Player::Draw(sf::RenderWindow& window)
{
  VisibleObject::Draw(window);
  if(selectedSpell != NULL)
  {
    window.draw(this->selectedSpell->_animatedSprite);
    window.draw(this->selectedSpell->circle);
  }
  if(castedSpell != NULL)
  {
    window.draw(this->castedSpell->_animatedSprite);
    window.draw(this->castedSpell->circle);
  }

}

void Player::Update(sf::Time timeDelta)
{
  VisibleObject::Update(timeDelta);
  bool keyWasPressed = false;
  this->SetSpeedX(0);
  this->SetSpeedY(0);

  //update cast circle position
  if(_spellSelected)
  {
    //update the position of the animation to this of the mouse
    sf::Vector2i localPosition = sf::Mouse::getPosition(Game::getWindow());
    sf::Vector2f worldPos = Game::getWindow().mapPixelToCoords(localPosition);
    this->selectedSpell->SetPosition(worldPos.x,worldPos.y);
    this->selectedSpell->Update(timeDelta);
  }

  if(this->castedSpell != NULL)
  {
    if(this->castedSpell->HasAnimationStopped())
    {
      delete(this->castedSpell);
      this->castedSpell = NULL;
    }
    else
    {
      this->castedSpell->Update(timeDelta);
      this->castedSpell->SpellHit();
      this->castedSpell->SetSpellPosition(this->GetCenterPosition().x,
                                     this->GetCenterPosition().y);
    }
  }

  if(HP <= 0 && this->_playerState != Player::Dying)
  {
    this->_playerState =  Player::Dying;
    this->currentAnimation = &(this->player_dead);
    this->_clock.restart();
    keyWasPressed = true;
  }
  //if not dying
  else
  {
      this->checkButtonsPressed();
  }

  switch(this->_playerState)
  {
    case Player::Walking:
    {
      int tempSpeed;
      if((sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
         sf::Keyboard::isKeyPressed(sf::Keyboard::W)) &&
         (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
         sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
          tempSpeed = PLAYERS_DIAGONAL_SPEED;
      else
          tempSpeed = PLAYERS_STRAIGHT_SPEED;

      if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        this->SetSpeedY(tempSpeed);
        this->currentAnimation = &(this->player_down);
        keyWasPressed = true;
      }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        this->SetSpeedY(-tempSpeed);
        this->currentAnimation = &(this->player_up);
        keyWasPressed = true;
      }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
      {
        this->SetSpeedX(-tempSpeed);
        this->currentAnimation = &(this->player_left);
        keyWasPressed = true;
      }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        this->SetSpeedX(tempSpeed);
        this->currentAnimation = &(this->player_right);
        keyWasPressed = true;
      }
      break;
    }
    case Player::Attacking:
    { //first time
      if(isAttacking() == false)
      {
        _animatedSprite.setFrameTime(sf::seconds(0.06));
        this->_clock.restart();
        setAttacking(true);
        this->_previousAnimation = this->currentAnimation;

        if(this->currentAnimation == &(this->player_right))
        {
          this->currentAnimation = &(this->cast_right);
        }
        else if(this->currentAnimation == &(this->player_up))
        {
          this->currentAnimation = &(this->cast_up);
        }
        else if(this->currentAnimation == &(this->player_down))
        {
          this->currentAnimation = &(this->cast_down);
        }
        else if(this->currentAnimation == &(this->player_left))
        {
          this->currentAnimation = &(this->cast_left);
        }
        keyWasPressed = true;
      }
      //while casting
      else if(isAttacking() == true  && _clock.getElapsedTime().asSeconds() <
              0.06* this->cast_down.getSize())
      {
        keyWasPressed = true;
      }
      //when _attacking ends
      else if(isAttacking() == true  && _clock.getElapsedTime().asSeconds() >=
               0.06 *this->cast_down.getSize())
      {
        setAttacking(false);
        this->_playerState =  Player::Walking;
        _animatedSprite.setFrameTime(sf::seconds(0.1));
        this->currentAnimation = this->_previousAnimation;
      }
      break;
    }
    case Player::Dying:
    {
      if( _clock.getElapsedTime().asSeconds() < 0.1 *
          (this->player_dead.getSize() - 1))
      {
        keyWasPressed =true;

      }
       else if(_clock.getElapsedTime().asSeconds() < 0.2 *
          this->player_dead.getSize())
      {
        _animatedSprite.setFrame(5, 2);
        keyWasPressed =true;
      }
      else
      {
        Game::ResetMap();
        this->_playerState =  Player::Walking;
      }
      break;
    }
  }

  this->GetSprite().move(this->GetSpeed().x * timeDelta.asSeconds(),
                         this->GetSpeed().y * timeDelta.asSeconds());

  _animatedSprite.play(*currentAnimation);

  if(!keyWasPressed)
  {
    _animatedSprite.stop();
  }
  _animatedSprite.update(timeDelta);
}


void Player::checkButtonsPressed()
{
    if(mouseLeftPressedOnce == false &&
       sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        this->mouseLeftPressedOnce = true;
        this->firstClick = true;
    }
    else if(mouseLeftPressedOnce == true &&
            sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        this->firstClick = false;
    }
    else
    {
      this->mouseLeftPressedOnce = false;
      this->firstClick = false;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) &&
                  isAttacking() == false && this->castedSpell == NULL)
    {
        this->ShootFireball();
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
    {
        this->SlowSpellSelected();
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) &&
                  isAttacking() == false)
    {
      if(this->castedSpell == NULL)
        this->HealSpellSelected();
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
    {
      if(this->castedSpell == NULL)
        this->PushSpellSelected();
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        this->_spellSelected = false;
        delete(this->selectedSpell);
        selectedSpell = NULL;
       // this->_paintedShape = sf::CircleShape();
    }

    if (this->_spellSelected && firstClick &&
         sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      if(castedSpell == NULL)
        SpellCasted();
      else
        std::cout<<"Not Yet"<<std::endl;
    }
    else if(firstClick)
    {
      sf::Vector2i localPosition = sf::Mouse::getPosition(Game::getWindow());
      sf::Vector2f worldPos=Game::getWindow().mapPixelToCoords(localPosition);

      sf::Vector2f framePosition = Game::GetSpellFramePos();
      sf::Vector2f floatPos;
      floatPos.x = worldPos.x;
      floatPos.y = worldPos.y;

      fireSpell = sf::Rect<float>(framePosition.x,
                                  framePosition.y,1.3 *50,1.3 *50);
      slowSpell = sf::Rect<float>(framePosition.x + 1.3 * 50,
                                  framePosition.y,1.3 *50,1.3 *50);
      healSpell = sf::Rect<float>(framePosition.x + 1.3 * 100,
                                  framePosition.y,1.3 *50,1.3 *50);
      pushSpell = sf::Rect<float>(framePosition.x + 1.3 * 150,
                                  framePosition.y,1.3 *50,1.3 *50);

      if(fireSpell.contains(floatPos) && isAttacking() == false
         && this->castedSpell == NULL)
      {
        this->ShootFireball();
      }
      else if(slowSpell.contains(floatPos))
      {
          this->SlowSpellSelected();
      }
      else if(healSpell.contains(floatPos))
      {
        if(this->castedSpell == NULL)
          this->HealSpellSelected();
      }
      else if(pushSpell.contains(floatPos))
      {
        if(this->castedSpell == NULL)
          this->PushSpellSelected();
      }

    }
}

void Player::SpellCasted()
{

  this->_spellSelected = false;
  this->castedSpell = this->selectedSpell;
  this->selectedSpell = NULL;
  this->castedSpell->spellCast();
  this->mana -= this->castedSpell->getManaCost();
  this->_playerState =  Player::Attacking;
}

void Player::SlowSpellSelected()
{
  sf::Vector2i localPosition = sf::Mouse::getPosition(Game::getWindow());
  sf::Vector2f worldPos = Game::getWindow().mapPixelToCoords(localPosition);
  this->selectedSpell = new SlowSpell(worldPos.x,worldPos.y);
  if(this->mana < this->selectedSpell->getManaCost())
  {
    this->selectedSpell = NULL;
    return;
  }
  this->_spellSelected =  true;
}

void Player::PushSpellSelected()
{
  this->castedSpell =  new PushSpell(this->GetCenterPosition().x
            ,this->GetCenterPosition().y);
  if(this->mana < this->castedSpell->getManaCost())
  {
    this->castedSpell = NULL;
    return;
  }
  this->mana -= this->castedSpell->getManaCost();
  this->_playerState =  Player::Attacking;
}

void Player::ShootFireball()
{
  if(this->mana >= FIREBALL_COST)
  {
    this->_playerState =  Player::Attacking;
    this->mana -= FIREBALL_COST;
    Fireball* fireball;
    if(this->currentAnimation == &(this->player_right))
    {
      fireball = new Fireball(GetCenterPosition().x,
                              GetCenterPosition().y, RIGHT);
      fireball->SetPosition(fireball->GetPosition().x,
                          fireball->GetPosition().y  - fireball->GetHeight()/2);
    }
    else if(this->currentAnimation == &(this->player_up))
    {

      fireball = new Fireball(GetCenterPosition().x,
                              GetCenterPosition().y, UP);
      fireball->SetPosition(fireball->GetPosition().x - fireball->GetWidth()/2,
                          fireball->GetPosition().y  - fireball->GetHeight()/2);
    }
    else if(this->currentAnimation == &(this->player_down))
    {

      fireball = new Fireball(GetCenterPosition().x,
                              GetCenterPosition().y, DOWN);
      fireball->SetPosition(fireball->GetPosition().x - fireball->GetWidth()/2,
                          fireball->GetPosition().y  - fireball->GetHeight()/2);
    }
    else if(this->currentAnimation == &(this->player_left))
    {

      fireball = new Fireball(GetCenterPosition().x,
                              GetCenterPosition().y, LEFT);
      fireball->SetPosition(fireball->GetPosition().x  - fireball->GetWidth(),
                          fireball->GetPosition().y  - fireball->GetHeight()/2);
    }
    Game::AddProjectile(fireball);
  }
}

void Player::HealSpellSelected()
{

  if(this->mana >= HEAL_COST && this->HP < FULL_HP)
  {
    this->castedSpell =  new HealSpell(this->GetCenterPosition().x
            ,this->GetCenterPosition().y);
    this->mana -= HEAL_COST;
    this->_playerState =  Player::Attacking;
    if(this->HP + HEAL_POINTS > FULL_HP)
      this->HP = FULL_HP;
    else
      this->HP += HEAL_POINTS;
  }
}

void Player::resetObject()
{
  VisibleObject::resetObject();
  this->HP = FULL_HP;
  this->mana = FULL_MANA;
  this->castedSpell = NULL;
  this->selectedSpell = NULL;
  this->_playerState =  Player::Walking;
}

sf::CircleShape Player::GetSpellCircle()
{
  return this->_paintedShape;
}

float Player::getHpPercentage()
{
  return (float)this->HP/FULL_HP;
}

float Player::getManaPercentage()
{
  return (float)this->mana/FULL_MANA;
}
