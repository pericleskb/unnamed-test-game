#pragma once
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "AnimatedSprite.hpp"
#include "Animation.hpp"

class VisibleObject{

public:
  VisibleObject();
  VisibleObject(int x, int y);

  virtual ~VisibleObject();
  virtual void Load(Animation& animation,std::string filename);
	virtual void Draw(sf::RenderWindow & window);
	virtual void Update(sf::Time elapsedTime);

	virtual void SetPosition(float x, float y);
	virtual sf::Vector2f GetPosition() const;
	virtual sf::Vector2f GetCenterPosition() const;
	virtual float GetWidth() const;
	virtual float GetHeight() const;

	virtual bool IsLoaded() const;
	virtual sf::Rect<float> GetBoundingRect() const;
  AnimatedSprite _animatedSprite;

  virtual void setPositionXToPrevious(int direction);
  virtual void setPositionYToPrevious(int direction);
  virtual sf::Vector2f getPreviousPosition();

  virtual bool Collided();
  virtual void setCollision(bool);

  virtual bool isAttacking();
  virtual void setAttacking(bool);

  virtual sf::Vector2f GetSpeed();
  virtual void SetSpeedX(float);
  virtual void SetSpeedY(float);

  virtual void setDamage(int);
  virtual int getHP();
  virtual int getPower();

  virtual void SetStartingPosition(int, int);
  virtual void resetObject();

  //to be implemented only in the player class
  virtual sf::CircleShape GetSpellCircle();

  virtual void setSlowFactor(float,sf::Time);

  virtual bool isDead();

protected:
	AnimatedSprite& GetSprite();
  Animation* currentAnimation;

  //this is loaded in the Load function.
  //it is needed to hold the texture for the lifetime of the object
  sf::Texture  _texture;

  sf::Vector2f prevPosition;
  //clock for counting the duration of an animation
	sf::Clock _clock;
	sf::Vector2f speed;
  int HP;
  sf::Vector2f startingPosition;
  int power = 1;

private:
  bool _collided;
	std::string _filename;
	bool _isLoaded;
  bool _attacking = false;
};
