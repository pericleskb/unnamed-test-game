#include "Player.hpp"
#include <assert.h>
#include "Game.hpp"
#include <iostream>

#define PLAYERS_SPEED 200
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
  this->HP = 3;
}

Player::~Player()
{

}

void Player::Update(sf::Time timeDelta)
{
  VisibleObject::Update(timeDelta);
  bool keyWasPressed = false;
  this->SetSpeedX(0);
  this->SetSpeedY(0);

  if(HP <= 0 && this->_playerState != Player::Dying)
  {
    this->_playerState =  Player::Dying;
    this->currentAnimation = &(this->player_dead);
    this->_clock.restart();
    keyWasPressed = true;
  }
  //if not dying
  else{
      this->checkButtonsPressed();
  }
  switch(this->_playerState)
  {
    case Player::Walking:
    {
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        this->SetSpeedY(PLAYERS_SPEED);
        this->currentAnimation = &(this->player_down);
        keyWasPressed = true;
      }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        this->SetSpeedY(-PLAYERS_SPEED);
        this->currentAnimation = &(this->player_up);
        keyWasPressed = true;
      }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
      {
        this->SetSpeedX(-PLAYERS_SPEED);
        this->currentAnimation = &(this->player_left);
        keyWasPressed = true;
      }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        this->SetSpeedX(PLAYERS_SPEED);
        this->currentAnimation = &(this->player_right);
        keyWasPressed = true;
      }
      break;
    }
    case Player::Attacking:
    { //first time
      if(isAttacking() == false)
      {
        this->_clock.restart();
        setAttacking(true);
        Fireball* fireball;
        if(this->currentAnimation == &(this->player_right))
        {
          this->currentAnimation = &(this->cast_right);
          fireball = new Fireball(this->GetCenterPosition().x +
                            this->GetWidth()/2,this->GetCenterPosition().y,
                            RIGHT);
        }
        else if(this->currentAnimation == &(this->player_up))
        {
          this->currentAnimation = &(this->cast_up);
          fireball = new Fireball(this->GetCenterPosition().x +
                            this->GetWidth()/2,this->GetCenterPosition().y,
                                            UP);
        }
        else if(this->currentAnimation == &(this->player_down))
        {
          this->currentAnimation = &(this->cast_down);
          fireball = new Fireball(this->GetCenterPosition().x +
                            this->GetWidth()/2,this->GetCenterPosition().y,
                                            DOWN);
        }
        else if(this->currentAnimation == &(this->player_left))
        {
          this->currentAnimation = &(this->cast_left);
          fireball = new Fireball(this->GetCenterPosition().x +
                            this->GetWidth()/2,this->GetCenterPosition().y,
                                            LEFT);
        }
        Game::AddProjectile(fireball);
        keyWasPressed = true;
      }
      //when _attacking ends
      else if(isAttacking() == true  && _clock.getElapsedTime().asSeconds() >= 0.1 *
              this->cast_down.getSize())
      {
        setAttacking(false);
        this->_playerState =  Player::Walking;
      }
      //while casting
      else if(isAttacking() == true  && _clock.getElapsedTime().asSeconds() <
              0.1* this->cast_down.getSize())
      {
        keyWasPressed = true;
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
/*here we are treating the situation when someone is prepared to cast a
    targeted spell not as a state.
    This may be right since when we are about to cast a targeted spell we are
    also able to walk, die and even cast an instant spell.

    So we can just use the _spellSelected cast to paint a circle where the mouse
    is, or to select an enemy.

    When _spellSelected and LeftClick then
      the state becomes Attacking
      and we can also draw a second animation for the casted spell
      and get the enemies that are in radius and apply the effects of the spell on them
    */

    //update cast circle position
    if(_spellSelected)
    {
      sf::Vector2i localPosition = sf::Mouse::getPosition(Game::getWindow());
      sf::Vector2f worldPos = Game::getWindow().mapPixelToCoords(localPosition);
      _paintedShape.setPosition(worldPos);
      //std::cout << "x = " << worldPos.x << "y = "<< worldPos.y << std::endl;
    }



    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) &&
                  isAttacking() == false)
    {
        this->_playerState =  Player::Attacking;
        //here we can assign a value to an enum , as to specify which spell we
        //have selected and then add it to the projectiles in the player's state
        //switch
        //for now as we only have fireball it is not needed
    }

    //for another spell we can have different radius etc
    //also an enum of spells in a function that will generate
    //the appropriate cirlce.
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
    {
        this->_spellSelected =  true;
        this->_paintedShape.setRadius(100);
        float radius = this->_paintedShape.getRadius();
        this->_paintedShape.setOrigin(radius,radius);
        this->_paintedShape.setFillColor(sf::Color(200, 50, 50,125));
        this->_paintedShape.setOutlineThickness(3);
        this->_paintedShape.setOutlineColor(sf::Color(250, 0, 0));
        sf::Vector2i localPosition = sf::Mouse::getPosition(Game::getWindow());
        sf::Vector2f worldPos = Game::getWindow().mapPixelToCoords(localPosition);
        this->_paintedShape.setPosition(worldPos);

    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        this->_spellSelected = false;
        this->_paintedShape = sf::CircleShape();
    }

    if (this->_spellSelected && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2i localPosition = sf::Mouse::getPosition(Game::getWindow());
        sf::Vector2f worldPos = Game::getWindow().mapPixelToCoords(localPosition);

        //for when the enum is implemented
        //this->_playerState = Player::Attacking;

        std::vector<VisibleObject*> objectsToHit;
        Game::GetObjectsInRadius(objectsToHit,this->_paintedShape);
        for(std::vector<VisibleObject*>::iterator objectItr =
             objectsToHit.begin(); objectItr!=objectsToHit.end();++objectItr)
        {
          (*objectItr)->setSlowFactor(0.5,this->slowDuration);
          std::cout<< "spell hit" <<std::endl;
        }
        this->_spellSelected = false;
        this->_paintedShape = sf::CircleShape();
    }
}

void Player::Draw(sf::RenderWindow& window)
{
  VisibleObject::Draw(window);
  if(this->_fireball != NULL)
  {
   std::cout<<"Draw Ball"<<std::endl;
    this->_fireball->Draw(window);
  }
}

void Player::resetObject()
{
  VisibleObject::resetObject();
  this->HP = FULL_HP;
}

sf::CircleShape Player::GetSpellCircle()
{
  return this->_paintedShape;
}

