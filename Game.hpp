#pragma once
#include <SFML/Graphics.hpp>
#include "ObjectManager.hpp"
#include "ImageManager.hpp"
#include "A-star.hpp"
#include <iostream>


/*Things to do

 -if needed to add more projectiles we need more Spell classes


 Boring:
 -could make a bigger map

 Hard:
 -study pathfindjng
 -make collision better

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
  static sf::Vector2f getPlayerBottomLeftPosition();
  static sf::Rect<float> getPlayersBorderRect();
  static VisibleObject* getVisibleObject(std::string);
  static void ResetMap();
  static void GetObjectsInRadius(std::vector<VisibleObject*>& ,sf::CircleShape);
  static void AddProjectile(VisibleObject*);
  static sf::FloatRect GetViewRectangle();
  static bool FindPath(sf::Vector2f currentPos, sf::Rect<float>,
                       std::list<sf::Vector2i>&, sf::Rect<float>);
  static sf::Vector2f GetSpellFramePos();
private:

  static bool IsExiting();
  static void GameLoop();

  static void clearLevel();
  static void createLevel();
  static void render();

  static void loadUITextures();

  static A_star *pathFinder;
  static ObjectManager _objectManager;

  enum GameState { Uninitialized, ShowingSplash, Paused,
          ShowingMenu, Playing, LoadingLevel, Exiting };

  static GameState _gameState;
  static sf::RenderWindow _mainWindow;
  static sf::RenderTexture _groundTexture;
  static sf::RenderTexture _topTexture;
  static sf::Texture _spellTexture,_HPTexture,_manaTexture,_barTexture;
  static sf::Sprite  _spellFrame,_HPSprite,_manaSprite,_barSprite;

};

