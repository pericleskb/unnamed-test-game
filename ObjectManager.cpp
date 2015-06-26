#include "ObjectManager.hpp"
#include "Game.hpp"
#include <iostream>
#include "Collision.hpp"
#include <string.h>

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
 // std::cout<<"size:"<<tempList.size()<<std::endl;

  int objectsAmount = tempList.size();

  for (int i = 0; i < objectsAmount; i++)
  {
    (tempList.front())->Draw(renderWindow);
    tempList.pop_front();
  }

  for(std::vector<VisibleObject*>::iterator projectileItr =
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
	//float timeDelta = Game::getWindow().getGetFrameTime();
	while(itr != _gameObjects.end())
	{
    itr->second->Update(elapsed);
    itr++;
	}

    std::cout<<_projectiles.size()<<std::endl;
  for(std::vector<VisibleObject*>::iterator projectileItr =
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

//rect2 must be the moving object
bool collisionBetween(sf::Rect<float> staticObject,sf::Rect<float> movingObject)
{
   return   (staticObject.left < movingObject.left + movingObject.width &&
      staticObject.left + staticObject.width > movingObject.left &&
      staticObject.top < movingObject.top + movingObject.height &&
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
  object.setCollision(true);


if(object.GetSpeed().y != 0)
    object.setPositionYToPrevious(bounceValue);


  //if horizontal collision
  if(objectsRect.left < otherRect.left){
    if(object.GetSpeed().x > 0)
      object.setPositionXToPrevious(bounceValue);
  }
  if(objectsRect.left+objectsRect.width > otherRect.left + otherRect.width){
    if(object.GetSpeed().x < 0)
      object.setPositionXToPrevious(bounceValue);
  }
  //if vertical collision
  if(objectsRect.top <= otherRect.top ){
    if(object.GetSpeed().y > 0)
      object.setPositionXToPrevious(bounceValue);
  }
  if(objectsRect.top + objectsRect.height > otherRect.top + otherRect.height){
    if(object.GetSpeed().y < 0)
      object.setPositionXToPrevious(bounceValue);
  }

}

void ObjectManager::checkBordersCollisions()
{
  std::map<std::string,VisibleObject*>::const_iterator ObjectItr =
  _gameObjects.begin();
	while(ObjectItr != _gameObjects.end())
	{
    for(std::vector<sf::RectangleShape>::iterator bordersItr = borders.begin();
      bordersItr!=borders.end();++bordersItr)
    {
      sf::Rect<float> bord((*bordersItr).getPosition(),(*bordersItr).getSize());
      sf::Rect<float> obj((*ObjectItr).second->GetBoundingRect());

      if( collisionBetween(bord,obj) )
      {

        bounce(*(*ObjectItr).second,obj,bord);
      }


      /*
      //if collision


        std::cout <<"COLL"<<std::endl;
        obj.left = previousPos.x;
        //if no intersection set it back
        if( !collisionBetween(bord,obj) )
        {
          std::cout <<"1"<<std::endl;
          (*ObjectItr).second->setPositionXToPrevious(bounceValue);
        }
        else
        {
          obj = ((*ObjectItr).second->GetBoundingRect());

          obj.top = previousPos.y;
          if(!collisionBetween(bord,obj) )
          {
          std::cout <<"2"<<std::endl;

            (*ObjectItr).second->setPositionYToPrevious(bounceValue);
          }
        }

      */

    }
    ObjectItr++;
	}
}

void ObjectManager::checkObjectsCollisions()
{
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
}

bool ObjectManager::LevelFinished()
{
 // if(_gameObjects.find("player") != _gameObjects.end()){
    if(collisionBetween(nextLevelRect,
                  _gameObjects.find("player")->second->GetBoundingRect()))
    {
      return true;
    }
    return false;
 // }
}

void ObjectManager::ResetMap()
{
  std::map<std::string,VisibleObject*>::const_iterator itr =
                                              _gameObjects.begin();
	while(itr != _gameObjects.end())
	{

	  (*itr).second->resetObject();
	  itr++;
	}
}

float GetDistance(sf::Vector2f a, sf::Vector2f b)
{
  sf::Vector2f distanceVector(abs(a.x - b.x), abs(a.y - b.y));
  std::cout<<"x = "<<distanceVector.x << "  y= "<<distanceVector.y <<std::endl;
  float distance = sqrt(pow(distanceVector.x,2) + pow(distanceVector.y,2));
  std::cout<<"dis = "<<distance <<std::endl;
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

void ObjectManager::CheckProjectileCollision()
{
std::map<std::string,VisibleObject*>::const_iterator ObjectItr =
  _gameObjects.begin();
	while(ObjectItr != _gameObjects.end())
  {
    if(strcmp((*ObjectItr).first.c_str(),"player") != 0)
    {
      for(std::vector<VisibleObject*>::iterator projectileItr =
          _projectiles.begin();projectileItr!=_projectiles.end();++projectileItr)
      {
        sf::Rect<float> obj((*ObjectItr).second->GetBoundingRect());
        sf::Rect<float> proj((*projectileItr)->GetBoundingRect());
        if( collisionBetween(obj,proj) )
        {
          (*ObjectItr).second->setDamage((*projectileItr)->getPower());
          _projectiles.erase(projectileItr);
          projectileItr--;
        }
      }
    }
   ObjectItr++;
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
        _gameObjects.erase(ObjectItr);
        ObjectItr--;
      }
    }
    ObjectItr++;
  }
  sf::FloatRect viewRect = Game::GetViewRectangle();
  //Projectiles
  for(std::vector<VisibleObject*>::iterator projectileItr =
          _projectiles.begin();projectileItr!=_projectiles.end();++projectileItr)
  {
    if((*projectileItr)->GetCenterPosition().x > viewRect.left + viewRect.width
        ||(*projectileItr)->GetCenterPosition().y > viewRect.top + viewRect.height)
    {
      _projectiles.erase(projectileItr);
      projectileItr--;
    }
  }
}
