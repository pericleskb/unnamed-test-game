#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <memory>
#include "Game.hpp"
#include "PugiMapLoader.hpp"

#define WINDOW_WIDTH  960
#define WINDOW_HEIGHT 640
#define VIEW_ZOOM     1.2f

using namespace std;

//to be read from a file
static std::string levelMaps[2] = {"maps/level3.tmx","maps/level4.tmx"};
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



  _mainWindow.create(sf::VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT,32),"Who Knows?");
  _mainWindow.setFramerateLimit(60);


  sf::View view(sf::FloatRect(0,0,WINDOW_WIDTH,WINDOW_HEIGHT));
  view.setCenter(_mainWindow.getSize().x/2,_mainWindow.getSize().y/2);
  _mainWindow.setView(view);

  loadUITextures();

  pathFinder = new A_star();

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
      while(_mainWindow.pollEvent(currentEvent))
      {
        if(currentEvent.type == sf::Event::Closed)
        {
          _gameState = Game::Exiting;
        }
      }
      break;
    }
    case Game::LoadingLevel:
    {
      _objectManager.clearLevel();
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
        view.zoom(VIEW_ZOOM);
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

        //drawing UI elements. make it a function
        _spellFrame.setPosition(sf::Vector2f(view.getCenter().x - view.getSize().x/2,
        (view.getCenter().y + view.getSize().y/2.f -
            (_spellFrame.getScale().y * _spellTexture.getSize().y))));

        if(_objectManager.IsObjectBehindUI(_spellFrame.getGlobalBounds()))
          _spellFrame.setColor(sf::Color(255,255,255,128));
        else
          _spellFrame.setColor(sf::Color(255,255,255,255));

        _mainWindow.draw(_spellFrame);


        _barSprite.setPosition(sf::Vector2f(view.getCenter().x -
                view.getSize().x/2 +
                _spellFrame.getLocalBounds().width * _spellFrame.getScale().x,
                (view.getCenter().y + view.getSize().y/2.f -
                _barSprite.getLocalBounds().height * 2 + 10)));


        _HPSprite.setPosition(_barSprite.getPosition());
        _HPSprite.setTextureRect(sf::IntRect(0, 0,
                            _HPTexture.getSize().x *
                    _objectManager.Get("player")->getHpPercentage(),
                            _HPTexture.getSize().y));
        _mainWindow.draw(_HPSprite);

        if(_objectManager.IsObjectBehindUI(_barSprite.getGlobalBounds()))
        {
          _HPSprite.setColor(sf::Color(255,255,255,128));
          _manaSprite.setColor(sf::Color(255,255,255,128));
          _barSprite.setColor(sf::Color(255,255,255,128));
        }
        else
        {
          _HPSprite.setColor(sf::Color(255,255,255,255));
          _manaSprite.setColor(sf::Color(255,255,255,255));
          _barSprite.setColor(sf::Color(255,255,255,255));
        }

        _mainWindow.draw(_barSprite);

        _barSprite.setPosition(sf::Vector2f(view.getCenter().x -
                view.getSize().x/2 +
                _spellFrame.getLocalBounds().width * _spellFrame.getScale().x,
                (view.getCenter().y + view.getSize().y/2.f -
                _barSprite.getLocalBounds().height + 5 )));

        _manaSprite.setPosition(_barSprite.getPosition());
        _manaSprite.setTextureRect(sf::IntRect(0, 0,
                            _HPTexture.getSize().x *
                    _objectManager.Get("player")->getManaPercentage(),
                            _HPTexture.getSize().y));
        _mainWindow.draw(_manaSprite);

        _mainWindow.draw(_barSprite);
        _mainWindow.display();
}


//create ground and top textures and add all the objects and borders to the
//object manager. These are read from the map xml file
void Game::createLevel()
{
  sf::Vector2i mapSize;
  sf::Vector2i mapDimensions;
  vector<TileSet> tileSetVector;
  vector<layerData> layerVector;
  pugiMapLoader loader;

  _topTexture.clear();
  _groundTexture.clear();

  loader.loadMap(levelMaps[currentLevel],tileSetVector,layerVector,
                _objectManager,&mapSize, &mapDimensions,pathFinder);

  if(!_topTexture.create(mapDimensions.x,mapDimensions.y,false))
  {
    throw("Failed to create renderTexture.\n");
  }
  if(!_groundTexture.create(mapDimensions.x,mapDimensions.y,false))
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
      if(layerVector[i].layerName.compare("Top") == 0 )
      {
        //for making Objects that appear in front of the player transparent
        //tempSprite.setColor(sf::Color(255,255,255,120));
        _topTexture.draw(tempSprite);
      }
      else
      {
        _groundTexture.draw(tempSprite);
      }
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

sf::FloatRect Game::GetViewRectangle()
{
  sf::Vector2f center = _mainWindow.getView().getCenter();
  sf::Vector2f size = _mainWindow.getView().getSize();
  float left = center.x - size.x/2;
  float top = center.y - size.y/2;
  return sf::FloatRect(left,top,size.x,size.y);
}

void Game::loadUITextures()
{

  if (!_spellTexture.loadFromFile("res/spell-frame.png"))
  {
    throw("spell-frame.png Not Found\n");
    exit(1);
  }
  _spellTexture.setSmooth(true);
  _spellFrame.setTexture(_spellTexture);
  _spellFrame.setScale(sf::Vector2f(1.3f, 1.3f));


  if (!_HPTexture.loadFromFile("res/RedBar.png"))
  {
    throw("RedBar.png Not Found\n");
    exit(1);
  }
  _HPTexture.setSmooth(true);
  _HPSprite.setTexture(_HPTexture);

  if (!_manaTexture.loadFromFile("res/BlueBar.png"))
  {
    throw("RedBar.png Not Found\n");
    exit(1);
  }
  _manaTexture.setSmooth(true);
  _manaSprite.setTexture(_manaTexture);

  if (!_barTexture.loadFromFile("res/EmptyBar.png"))
  {
    throw("RedBar.png Not Found\n");
    exit(1);
  }
  _barTexture.setSmooth(true);
  _barSprite.setTexture(_barTexture);
}

sf::Vector2f Game::getPlayersPosition()
{

  return _objectManager.Get("player")->GetCenterPosition();
}


sf::Vector2f Game::getPlayerBottomLeftPosition()
{
 return _objectManager.Get("player")->GetBottomLeftPosition();
}

sf::Rect<float> Game::getPlayersBorderRect()
{
 return (_objectManager.Get("player")->GetBorderBoundingRect());
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

bool Game::FindPath(sf::Vector2f currentPos, sf::Rect<float> EnemyRect,
    std::list<sf::Vector2i>& pathList, sf::Rect<float> rect)
{
  return (pathFinder->FindPath(currentPos,EnemyRect,pathList,rect));
}

sf::Vector2f Game::GetSpellFramePos()
{
  return _spellFrame.getPosition();
}

Game::GameState   Game::_gameState = Uninitialized;
sf::RenderWindow  Game::_mainWindow;
sf::RenderTexture Game::_groundTexture ;
sf::RenderTexture Game::_topTexture ;
sf::Texture       Game::_spellTexture,Game::_HPTexture,Game::_manaTexture,
                                                          Game::_barTexture;
sf::Sprite        Game::_spellFrame,Game::_HPSprite,Game::_manaSprite,
                                                          Game::_barSprite;
ObjectManager     Game::_objectManager;
ImageManager      Game::imageManager;
A_star*           Game::pathFinder;

