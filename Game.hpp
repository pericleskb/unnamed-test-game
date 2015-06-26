#pragma once
#include <SFML/Graphics.hpp>
#include "ObjectManager.hpp"
#include "ImageManager.hpp"
#include <iostream>


/*Things to do

*segme when 1 pressed continuously
*player shouldn't be added to the object manager. Don't know if it is too late
 to change

*enemy orcs bounding rectangle is much higher than their height


 -if needed to add more projectiles we need more Spell classes


 Boring:
 -could make a bigger map and try having smaller objects

 Hard:
 -study pathfindjng

- click and lock on player or position

- set up some attacks for the players

  -if the attacks are added the game is set for making it a game by adding
    different kinds of enemies setting up maps and creating levels
*/

class Game
{

public:

  static void Start();
  static sf::RenderWindow& getWindow();
  static ImageManager imageManager;
  static sf::Vector2f getPlayersPosition();
  static VisibleObject* getVisibleObject(std::string);
  static void ResetMap();
  static void GetObjectsInRadius(std::vector<VisibleObject*>& ,sf::CircleShape);
  static void AddProjectile(VisibleObject*);
  static sf::FloatRect GetViewRectangle();
private:

  static bool IsExiting();
  static void GameLoop();

  static void createLevel();
  static void render();


  static ObjectManager _objectManager;

  enum GameState { Uninitialized, ShowingSplash, Paused,
          ShowingMenu, Playing, LoadingLevel, Exiting };

  static GameState _gameState;
  static sf::RenderWindow _mainWindow;
  static sf::RenderTexture _groundTexture;
  static sf::RenderTexture _topTexture;
};

