#pragma once
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "../Animation/AnimatedSprite.hpp"
#include "../Animation/Animation.hpp"

#define PI 3.14159265

/*This class contains things needed only for the player or only for the enemies.
But due to bad design all the functions in object manager use the visibleObject
class and I don't feel like changing all these stuff at this point.
*/

class VisibleObject{

public:
  VisibleObject();
  VisibleObject(int x, int y);

  virtual ~VisibleObject();
  virtual void Load(Animation& animation,std::string filename);
	virtual void Draw(sf::RenderWindow & window);
	virtual void Update(sf::Time elapsedTime);

	virtual void SetPosition(float x, float y);
	//for seting camera,borders and animations
	virtual sf::Vector2f GetPosition() const;
	//for graphics and checking if an object is in the radius of other objects
	virtual sf::Vector2f GetCenterPosition() const;
	//for A-star and following other VisibleObjects
	virtual sf::Vector2f GetBottomLeftPosition() const;
	virtual float GetWidth() const;
	virtual float GetHeight() const;

	virtual bool IsLoaded() const;
	virtual sf::Rect<float> GetBoundingRect() const;
	virtual sf::Rect<float> GetBorderBoundingRect() const;
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
  virtual float getHpPercentage();
  virtual float getManaPercentage();
  int getPower();

  virtual void SetStartingPosition(int, int);
  virtual void resetObject();

  //to be implemented only in the player class
  virtual sf::CircleShape GetSpellCircle();

  virtual void setSlowFactor(float,sf::Time);

  virtual bool isDead();

  virtual void SetPush(double angle, sf::Time time);
  bool IsPushed();

  //bad bad bad
  enum EnemyState { Guard, Chase, Attack, Return, Dying, Died, Pushed };
  virtual enum EnemyState getEnemyState();

protected:
	AnimatedSprite& GetSprite();
  Animation* currentAnimation;

  //this is loaded in the Load function.
  //it is needed to hold the texture for the lifetime of the object
  sf::Texture  _texture;

  sf::Vector2f prevPosition;
  sf::Vector2f startingPosition;
  //clock for counting the duration of an animation
	sf::Clock _clock;
	sf::Vector2f speed;
	int mana = 0;
  int HP;
  int power = 1;

  bool _isPushed = false;
  sf::RectangleShape BoundingRect;
	bool _isLoaded;

private:
  bool _collided;
	std::string _filename;
  bool _attacking = false;
};
