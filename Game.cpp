#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <memory>
#include "Game.hpp"
#include "PugiMapLoader.hpp"

using namespace std;

static std::string levelMaps[2] = {"maps/level1.tmx","maps/p.xml"};
static int currentLevel = 0;
void Game::Start()
{
//this should run only the first time
  if(_gameState != Uninitialized){
    throw("Game::Start() called twice\n");
    exit(1);
  }

  imageManager.init();
  _gameState = Game::LoadingLevel;



  _mainWindow.create(sf::VideoMode(960,640,32),"Who Knows?");
  _mainWindow.setFramerateLimit(60);


  sf::View view(sf::FloatRect(0,0,960,640));
  view.setCenter(_mainWindow.getSize().x/2,_mainWindow.getSize().y/2);
  _mainWindow.setView(view);

  while(!IsExiting()){
    GameLoop();
  }

  _mainWindow.close();
}

bool Game::IsExiting()
{
  return _gameState == Exiting;
}

void Game::GameLoop()
{
  sf::Event currentEvent;
  switch(_gameState)
  {
    case Game::Playing:
    {
      _objectManager.UpdateAll();
      _objectManager.checkBordersCollisions();
      _objectManager.checkObjectsCollisions();
      _objectManager.CheckProjectileCollision();
      render();

      if(_objectManager.LevelFinished())
      {
        _gameState = GameState::LoadingLevel;
        currentLevel++;
      }
      while(_mainWindow.pollEvent(currentEvent)){
        if(currentEvent.type == sf::Event::Closed)
        {
          _gameState = Game::Exiting;
        }
      }
      break;
    }
    case Game::LoadingLevel:
    {
      createLevel();
      _gameState = Game::Playing;
      break;
    }
  }
}

sf::RenderWindow& Game::getWindow()
{
  return _mainWindow;
}

void Game::render()
{
        _mainWindow.clear(sf::Color(0,0,0));

        sf::Sprite groundSprite = sf::Sprite(_groundTexture.getTexture());
        sf::Sprite topSprite = sf::Sprite(_topTexture.getTexture());
        _mainWindow.draw(groundSprite);
        _objectManager.DrawAll(_mainWindow);
        _mainWindow.draw(topSprite);
        _mainWindow.draw(_objectManager.Get("player")->GetSpellCircle());

        //hold camera on RenderTexture and on player
        sf::View view = _mainWindow.getDefaultView();

        view.zoom(1.2f);
        view.setCenter(_objectManager.Get("player")->getPreviousPosition());

        if(view.getCenter().x < view.getSize().x/2)
          view.setCenter(view.getSize().x/2,view.getCenter().y);
        else if(view.getCenter().x >
        _groundTexture.getSize().x - view.getSize().x/2)
        {
          view.setCenter(_groundTexture.getSize().x - view.getSize().x/2,
                         view.getCenter().y);
        }

        if(view.getCenter().y < view.getSize().y/2)
          view.setCenter(view.getCenter().x,view.getSize().y/2);
        else if(view.getCenter().y >
        _groundTexture.getSize().y - view.getSize().y/2)
        {
          view.setCenter(view.getCenter().x,
                _groundTexture.getSize().y - view.getSize().y/2);
        }
        _mainWindow.setView(view);


        _mainWindow.display();
}


//create ground and top textures and add all the objects and borders to the
//object manager. These are read from the map xml file
void Game::createLevel()
{
  sf::Vector2i mapSize;
  vector<TileSet> tileSetVector;
  vector<layerData> layerVector;
  pugiMapLoader loader;

  loader.loadMap(levelMaps[currentLevel],tileSetVector,layerVector,
                _objectManager,&mapSize);

  if(!_topTexture.create(1280,960,false))
  {
    throw("Failed to create renderTexture.\n");
  }
  if(!_groundTexture.create(1280,960,false))
  {
    throw("Failed to create renderTexture.\n");
  }
  for(unsigned int i =0; i<layerVector.size(); i++)
  {
    for(unsigned int j =0; j<layerVector[i].data.size(); j++)
    {
      TileSet *tempSet;
      tileData tempVal = layerVector[i].data[j];
      for(vector<TileSet>::iterator it = tileSetVector.begin();
          it!=tileSetVector.end();++it)
      {
        if(tempVal.val <= (*it).getLastGid() &&
          tempVal.val >= (*it).getFirstGid()){
          tempSet = &(*it);
          break;
        }
      }

      int xOfTile = (tempVal.val - tempSet->getFirstGid()) %
        tempSet->getTileAmountWidth() * tempSet->getTileWidth();
      int yOfTile = (tempVal.val - tempSet->getFirstGid()) /
        tempSet->getTileAmountWidth() * tempSet->getTileHeight();

      sf::Sprite tempSprite = sf::Sprite(*tempSet->getTilesetTexture(),
        sf::IntRect(xOfTile,yOfTile,tempSet->getTileWidth(),
        tempSet->getTileHeight()));
      xOfTile = (tempVal.pos % mapSize.x) * tempSet->getTileWidth();
      yOfTile = (tempVal.pos / mapSize.x) * tempSet->getTileHeight();
      tempSprite.setPosition(xOfTile,yOfTile);
      //if there is no top layer very good segme chances
      if(layerVector[i].layerName.compare("Top") == 0 )
        _topTexture.draw(tempSprite);
      else
        _groundTexture.draw(tempSprite);
     // delete tempSet;
     // tempSet = NULL;
    }
  }
  sf::View view = _mainWindow.getDefaultView();
  sf::Vector2f center(_objectManager.Get("player")->GetPosition().x -
                      _objectManager.Get("player")->GetWidth()/2,
                      _objectManager.Get("player")->GetPosition().y -
                      _objectManager.Get("player")->GetHeight()/2);
  view.setCenter(center);
  _mainWindow.setView(view);
  _groundTexture.display();
  _topTexture.display();
}

sf::Vector2f Game::getPlayersPosition()
{
  return _objectManager.Get("player")->GetCenterPosition();
}

VisibleObject* Game::getVisibleObject(std::string id)
{
  return _objectManager.Get(id);
}

void Game::ResetMap()
{
  _objectManager.ResetMap();
}

void Game::GetObjectsInRadius(std::vector<VisibleObject*>& vec,
                              sf::CircleShape circle)
{
  _objectManager.GetObjectsInRadius(vec,circle);
}

void Game::AddProjectile(VisibleObject* projectile)
{
  _objectManager.AddProjectile(projectile);
}

sf::FloatRect Game::GetViewRectangle()
{
  sf::Vector2f center = _mainWindow.getView().getCenter();
  sf::Vector2f size = _mainWindow.getView().getSize();
  float left = center.x - size.x/2;
  float top = center.y - size.y/2;
  return sf::FloatRect(left,top,size.x,size.y);
}

Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;
sf::RenderTexture Game::_groundTexture ;
sf::RenderTexture Game::_topTexture ;
ObjectManager Game::_objectManager;
ImageManager Game::imageManager;

