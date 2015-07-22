#pragma once
#include "Objects/VisibleObject.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <list>

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	void Add(std::string name, VisibleObject* gameObject);
	void Remove(std::string name);
	int GetObjectCount() const;
	VisibleObject* Get(std::string name) const;

	void DrawAll(sf::RenderWindow& renderWindow);
	void UpdateAll();

  bool LevelFinished();
  void clearLevel();

  void AddProjectile(VisibleObject*);

  //these should be private and accessed with the proper functions
	std::vector<sf::RectangleShape> borders;
  void checkBordersCollisions();
  void checkObjectsCollisions();
  void CheckProjectileCollision();
  void CheckObjectsToRemove();

  sf::Rect<float> nextLevelRect;

  void ResetMap();

  void GetObjectsInRadius(std::vector<VisibleObject*>& , sf::CircleShape);

  bool IsObjectBehindUI(sf::Rect<float>);

private:


  sf::Clock _clock;
	std::map<std::string, VisibleObject*> _gameObjects;
	std::list<VisibleObject*> _projectiles;
  std::map<std::string, VisibleObject*> _deadObjects;
  void sortObjectsByHeight(std::list<VisibleObject*>&);
//called from the destructor to delete all elements
	struct ObjectDeallocator
	{
		void operator()(const std::pair<std::string,VisibleObject*> & p) const
		{
			delete p.second;
		}
	};

};
