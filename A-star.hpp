#pragma once
#include <list>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

const char walkable = 1 , unwalkable = 0;


struct node{
  char isWalkable = walkable;
  int G;
  float H;
  float F;
  node* parent;
  int inOpenList = 0;
  int inClosedList = 0;
  sf::Vector2i position;
};

class A_star
{
public:
  A_star();
  ~A_star();
bool FindPath(sf::Vector2f currentPos, sf::Rect<float> ,
                std::list<sf::Vector2i>&, sf::Rect<float>);

  //this must be called right after the constructor
  void SetSize(int x, int y);
  void AddUnwalkables(sf::RectangleShape);

private:
  node** gameMap;
  int mapWidth, mapHeight,tileSize = 32;
  int closedListId = 1, openListId = 1;

  /*
  for now I will use the std::list to store open and closed lists
  but I will try to optimize it, when I get it to work.
  what we need to save is time, so we can use some
  extra structures to accomplish that
  */
  std::list<node> openList;
  std::list<node> closedList;

  sf::Vector2f GetCurrentTilePosition(int x, int y);
  sf::Vector2i GetCurrentTile(sf::Vector2f);
  sf::Vector2i GetBorderCurrentTile(sf::Vector2f);
  float ComputeHCost(sf::Vector2f, sf::Vector2f);
  node* GetNextNode();
  void PopNodeOpenList(node*);
  bool objectCollision(node* currentNode,sf::Rect<float>);

  short int adjacentX[8] = {1,1,1,0,-1,-1,-1,0};
  short int adjacentY[8] = {-1,0,1,1,1,0,-1,-1};
};
