#include "ObjectManager.hpp"
#include "Game.hpp"
#include "Collision.hpp"
#include <iostream>
#include <string.h>

//the player class shouldn't be handled by the same functions that handles
//other visible objects. Too late to change now but the VisibleObject class
//is full of variables that have no use for the player and vice versa

static int bounceValue = 0;

ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
	std::for_each(_gameObjects.begin(),_gameObjects.end(),ObjectDeallocator());
}


void ObjectManager::Add(std::string name, VisibleObject* gameObject)
{
  _gameObjects.insert(std::pair<std::string,VisibleObject*>(name,gameObject));
}


void ObjectManager::Remove(std::string name)
{
  std::map<std::string, VisibleObject*>::iterator results =
    _gameObjects.find(name);
  if(results != _gameObjects.end()){
    delete results->second;
		_gameObjects.erase(results);
  }
}

void ObjectManager::sortObjectsByHeight(std::list<VisibleObject*>& objList)
{
  std::map<std::string, VisibleObject*>::iterator mapIt = _gameObjects.begin();
  objList.push_front((*mapIt).second);
  mapIt++;
  while(mapIt !=_gameObjects.end())
  {

    std::list<VisibleObject*>::iterator listIt = objList.begin() ;
    int i =0;
    while(i < objList.size())
    {
      if((*mapIt).second->GetPosition().y < (*listIt)->GetPosition().y)
      {
        objList.insert(listIt,(*mapIt).second);
        break;
      }
      else if(i + 1 == objList.size())
      {
        objList.insert(++listIt,(*mapIt).second);
        break;
      }

      listIt++;
      i++;
    }

    mapIt++;
  }
}

void ObjectManager::DrawAll(sf::RenderWindow& renderWindow)
{
  std::list<VisibleObject*> tempList;
  sortObjectsByHeight(tempList);

  int objectsAmount = tempList.size();

  for (int i = 0; i < objectsAmount; i++)
  {
    (tempList.front())->Draw(renderWindow);
    tempList.pop_front();
  }

  for(std::list<VisibleObject*>::iterator projectileItr =
        _projectiles.begin();projectileItr!=_projectiles.end();++projectileItr)
  {
    (*projectileItr)->Draw(renderWindow);
  }
}

void ObjectManager::UpdateAll()
{
  sf::Time elapsed = _clock.restart();
	std::map<std::string,VisibleObject*>::const_iterator itr =
                                                _gameObjects.begin();
	while(itr != _gameObjects.end())
	{
    itr->second->Update(elapsed);
    itr++;
	}
  for(std::list<VisibleObject*>::iterator projectileItr =
        _projectiles.begin();projectileItr!=_projectiles.end();++projectileItr)
  {

    (*projectileItr)->Update(elapsed);
  }

  this->CheckObjectsToRemove();

}


int ObjectManager::GetObjectCount() const
{
  return _gameObjects.size();
}


VisibleObject* ObjectManager::Get(std::string name) const
{
std::map<std::string, VisibleObject*>::const_iterator results =
    _gameObjects.find(name);
  if(results != _gameObjects.end()){
    return results->second;
  }
  return NULL;
}

bool collisionBetween(sf::Rect<float> staticObject,sf::Rect<float> movingObject)
{
   return   (staticObject.left < movingObject.left + movingObject.width &&
      staticObject.left + staticObject.width > movingObject.left &&
      staticObject.top <= movingObject.top + movingObject.height +2 &&
      staticObject.top + staticObject.height >
      movingObject.top + (movingObject.height * 0.75));
}

bool collisionBetweenVisibleObjs(sf::Rect<float> staticObject,sf::Rect<float> movingObject)
{
   return   (staticObject.left < movingObject.left + movingObject.width &&
      staticObject.left + staticObject.width > movingObject.left &&
      staticObject.top - (staticObject.height  * 0.5)  < movingObject.top &&
      staticObject.top + staticObject.height >
      movingObject.top + (movingObject.height * 0.5));
}

void bounce(VisibleObject& object, sf::Rect<float> objectsRect,
            sf::Rect<float> otherRect)
{
  if(object.GetSpeed().y != 0)
    object.setPositionYToPrevious(bounceValue);

  if(object.GetSpeed().x != 0)
    object.setPositionXToPrevious(bounceValue);
  //if horizontal collision
 /* if(objectsRect.left <= otherRect.left){
    if(object.GetSpeed().x > 0)
      object.setPositionXToPrevious(bounceValue);
  }
  if(objectsRect.left+objectsRect.width >= otherRect.left + otherRect.width){
    if(object.GetSpeed().x < 0)
      object.setPositionXToPrevious(bounceValue);
  }
  //if vertical collision
  if(objectsRect.top <= otherRect.top ){
    if(object.GetSpeed().y > 0)
      object.setPositionXToPrevious(bounceValue);
  }
  if(objectsRect.top + objectsRect.height >= otherRect.top + otherRect.height){
    if(object.GetSpeed().y < 0)
      object.setPositionXToPrevious(bounceValue);
  }
*/
}

void ObjectManager::checkBordersCollisions()
{
  std::map<std::string,VisibleObject*>::const_iterator ObjectItr =
  _gameObjects.begin();
	while(ObjectItr != _gameObjects.end())
	{
    for(std::vector<sf::RectangleShape>::iterator bordersItr =borders.begin();
      bordersItr!=borders.end();++bordersItr)
    {
      sf::Rect<float> bord((*bordersItr).getPosition(),(*bordersItr).
                                                                  getSize());
      sf::Rect<float> obj((*ObjectItr).second->GetBorderBoundingRect());

      if( collisionBetween(bord,obj) )
      {
        (*ObjectItr).second->setCollision(true);
        if(strcmp((*ObjectItr).first.c_str(),"player") == 0 ||
          (*ObjectItr).second->getEnemyState() ==
                          VisibleObject::EnemyState::Pushed)
        {
          bounce(*(*ObjectItr).second,obj,bord);
        }
      }
    }
    ObjectItr++;
  }
}

void ObjectManager::checkObjectsCollisions()
{
  VisibleObject* player = _gameObjects.find("player")->second;
  std::map<std::string,VisibleObject*>::const_iterator ObjectItr =
    _gameObjects.begin();
	while(ObjectItr != _gameObjects.end())
  {
    if(strcmp((*ObjectItr).first.c_str(),"player") != 0)
    {
        sf::Rect<float> obj((*ObjectItr).second->GetBoundingRect());
        sf::Rect<float> obj2(player->GetBoundingRect());
        if( collisionBetweenVisibleObjs(obj2,obj) )
        {
           bounce(*(*ObjectItr).second,obj,obj2);
           bounce(*player,obj2,obj);
        }
    }
    ObjectItr++;
  }

  /*
  With this, enemies collide with each other
  std::map<std::string,VisibleObject*>::const_iterator ObjectItr =
  _gameObjects.begin();
	while(ObjectItr != _gameObjects.end())
  {
    std::map<std::string,VisibleObject*>::const_iterator ObjectItr2 =
      --_gameObjects.end();
      while(ObjectItr2 != ObjectItr)
      {
        sf::Rect<float> obj((*ObjectItr).second->GetBoundingRect());
        sf::Rect<float> obj2((*ObjectItr2).second->GetBoundingRect());
        if( collisionBetweenVisibleObjs(obj2,obj) )
        {
           bounce(*(*ObjectItr).second,obj,obj2);
           bounce(*(*ObjectItr2).second,obj2,obj);
        }
        ObjectItr2--;
    }
    ObjectItr++;
  }
  */
}


void ObjectManager::CheckProjectileCollision()
{
  for(std::list<VisibleObject*>::iterator projectileItr =
        _projectiles.begin();projectileItr!=_projectiles.end();++projectileItr)
  {
    sf::Rect<float> proj((*projectileItr)->GetBoundingRect());
    //for objects
    std::map<std::string,VisibleObject*>::const_iterator ObjectItr =
          _gameObjects.begin();
    while(ObjectItr != _gameObjects.end())
    {
      if(strcmp((*ObjectItr).first.c_str(),"player") != 0)
      {
        sf::Rect<float> obj((*ObjectItr).second->GetBoundingRect());
        if( collisionBetween(obj,proj) )
        {
          (*ObjectItr).second->setDamage((*projectileItr)->getPower());
          delete *projectileItr;
          _projectiles.erase(projectileItr);
          projectileItr--;
          break;
        }
      }
      ObjectItr++;
    }

    //for borders
    for(std::vector<sf::RectangleShape>::iterator bordersItr = borders.begin();
      bordersItr!=borders.end();++bordersItr)
    {
      sf::Rect<float> bord((*bordersItr).getPosition(),(*bordersItr).getSize());

      if( collisionBetween(bord,proj) )
      {
        delete *projectileItr;
        _projectiles.erase(projectileItr);
        projectileItr--;
      }
    }
  }
}

void ObjectManager::CheckObjectsToRemove()
{
  //Enemies
  std::map<std::string,VisibleObject*>::const_iterator ObjectItr =
    _gameObjects.begin();
	while(ObjectItr != _gameObjects.end())
  {
	  if(strcmp((*ObjectItr).first.c_str(),"player") != 0)
    {
      if((*ObjectItr).second->isDead())
      {
        _deadObjects.insert(std::pair<std::string,VisibleObject*>
                                ((*ObjectItr).first,(*ObjectItr).second));
        _gameObjects.erase(ObjectItr);
        ObjectItr--;
      }
    }
    ObjectItr++;
  }
  sf::FloatRect viewRect = Game::GetViewRectangle();

  //Projectiles
  std::list<VisibleObject*>::iterator projectileItr = _projectiles.begin() ;
  while(projectileItr != _projectiles.end())
  {
    if((*projectileItr)->GetCenterPosition().x > viewRect.left + viewRect.width
        ||(*projectileItr)->GetCenterPosition().y > viewRect.top + viewRect.height)
    {
      delete *projectileItr;
      _projectiles.erase(projectileItr);
      projectileItr--;
    }
    projectileItr++;
  }
}

bool ObjectManager::LevelFinished()
{
    if(collisionBetween(nextLevelRect,
                  _gameObjects.find("player")->second->GetBoundingRect()))
    {
      return true;
    }
    return false;
}

void ObjectManager::clearLevel()
{
  if(!_gameObjects.empty())
  {
    VisibleObject* tempPlayer = _gameObjects.find("player")->second;
    _gameObjects.clear();
    this->Add("player", tempPlayer);
    _gameObjects.find("player")->second->resetObject();
  }
  if(!_projectiles.empty())
    _projectiles.clear();
  if(!_deadObjects.empty())
    _deadObjects.clear();
}

void ObjectManager::ResetMap()
{

  //restore dead Enemies
  std::map<std::string,VisibleObject*>::const_iterator deadItr =
                    _deadObjects.begin();
	while(deadItr != _deadObjects.end())
  {
    this->Add((*deadItr).first,(*deadItr).second);
    deadItr++;
  }
  _deadObjects.clear();

  //setSlowFactors and Pushes. HPs and Mana
  std::map<std::string,VisibleObject*>::const_iterator itr =
                                              _gameObjects.begin();
	while(itr != _gameObjects.end())
	{

	  (*itr).second->resetObject();
	  itr++;
	}

	for(std::list<VisibleObject*>::iterator projectileItr =
          _projectiles.begin();projectileItr!=_projectiles.end();++projectileItr)
  {
    delete *projectileItr;
    _projectiles.erase(projectileItr);
    projectileItr--;
  }
}

float GetDistance(sf::Vector2f a, sf::Vector2f b)
{
  sf::Vector2f distanceVector(abs(a.x - b.x), abs(a.y - b.y));
  float distance = sqrt(pow(distanceVector.x,2) + pow(distanceVector.y,2));
  return distance;
}

void ObjectManager::GetObjectsInRadius(std::vector<VisibleObject*>&
                                        objectsVector, sf::CircleShape circle)
{
 std::map<std::string,VisibleObject*>::const_iterator ObjectItr =
  _gameObjects.begin();
	while(ObjectItr != _gameObjects.end())
  {
    if(strcmp((*ObjectItr).first.c_str(),"player") != 0)
    {
      if(GetDistance(circle.getPosition(),
              (*ObjectItr).second->GetCenterPosition()) <= circle.getRadius())
      {
        objectsVector.push_back((*ObjectItr).second);
      }
    }
    ObjectItr++;
  }
}

void ObjectManager::AddProjectile(VisibleObject* projectile)
{
  _projectiles.push_back(projectile);
}

bool ObjectManager::IsObjectBehindUI(sf::Rect<float> UI_Rect)
{
  std::map<std::string,VisibleObject*>::const_iterator ObjectItr =
    _gameObjects.begin();
	while(ObjectItr != _gameObjects.end())
  {
    sf::Rect<float> obj((*ObjectItr).second->GetBoundingRect());
    if( collisionBetween(UI_Rect,obj) )
    {
       return true;
    }
    ObjectItr++;
  }
  return false;
}
