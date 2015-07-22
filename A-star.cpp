#include "A-star.hpp"
#include <iostream>

A_star::A_star()
{

}

void A_star::SetSize(int width,int height)
{
  this->mapWidth = width;
  this->mapHeight = height;
  this->gameMap = new node*[width];
  for(int i = 0; i < width;i++)
    this->gameMap[i] = new node[height];
}

//boundingRect and distanceFromPosition provide the needed information to check
//if an object collides with borders.
bool A_star::FindPath(sf::Vector2f position, sf::Rect<float> enemyRect ,
    std::list<sf::Vector2i>& targetList, sf::Rect<float> boundingRect)
{
  sf::Vector2f target(enemyRect.left,enemyRect.top + enemyRect.height);
  sf::Vector2i currentSquare = GetCurrentTile(position);
  sf::Vector2i targetSquare = GetCurrentTile(target);
  node* startingNode;

  std::cout<<"Target Tile: " <<targetSquare.x <<","<<targetSquare.y <<std::endl;

  if(gameMap[targetSquare.x][targetSquare.y].isWalkable == unwalkable)
  {
    sf::Vector2f tempTarget(target.x + enemyRect.width,
                            target.y - enemyRect.height);
    targetSquare = GetCurrentTile(tempTarget);
    if(gameMap[targetSquare.x][targetSquare.y].isWalkable == unwalkable)
    {
      return false;
    }
    else
    {
      target.x += enemyRect.width;
      target.y -= enemyRect.height;
    }
  }

  startingNode = &gameMap[currentSquare.x][currentSquare.y];
  //initialisation of current node
  startingNode->G = 0;
  startingNode->H = ComputeHCost(position,target);
  startingNode->F = startingNode->H;
  startingNode->parent = NULL;
  startingNode->position.x = currentSquare.x;
  startingNode->position.y = currentSquare.y;

  startingNode->inClosedList = openListId;
  openList.push_back(*startingNode);

  node* currentNode;
  int k = -1;
  while(1)
  {
    k++;
    currentNode = GetNextNode();
    //openList empty
    if(currentNode == NULL)
    {
      std::cout<<" A* DID NOT Find solution"<<std::endl;
      openList.clear();
      closedList.clear();
      openListId += 5;
      closedListId += 5;
      return false;
    }

    for(int i=0;i<8;i++)
    {
      node* adjacentNode = &gameMap[currentNode->position.x + adjacentX[i]]
                          [currentNode->position.y + adjacentY[i]];

      if(currentNode->position.x + adjacentX[i] < mapWidth &&
         currentNode->position.y + adjacentY[i] < mapHeight &&
         currentNode->position.x + adjacentX[i] >= 0 &&
         currentNode->position.y + adjacentY[i] >= 0 &&
         adjacentNode->inClosedList != closedListId &&
         adjacentNode->isWalkable == walkable &&
         !objectCollision(adjacentNode, boundingRect))
      {
        int costToSquare = 45;
        node tempNode1, tempNode2;
        if(adjacentX[i] == 1 && adjacentY[i] == -1)
        {
          tempNode1 = gameMap[currentNode->position.x + 1]
                          [currentNode->position.y];
          tempNode2 = gameMap[currentNode->position.x]
                          [currentNode->position.y - 1];
        }
        else if(adjacentX[i] == 1 && adjacentY[i] == 1)
        {
          tempNode1 = gameMap[currentNode->position.x + 1]
                          [currentNode->position.y];
          tempNode2 = gameMap[currentNode->position.x]
                          [currentNode->position.y + 1];
        }
        else if(adjacentX[i] == -1 && adjacentY[i] == 1)
        {
          tempNode1 = gameMap[currentNode->position.x - 1]
                          [currentNode->position.y];
          tempNode2 = gameMap[currentNode->position.x]
                          [currentNode->position.y + 1];
        }
        else if(adjacentX[i] == -1 && adjacentY[i] == -1)
        {
          tempNode1 = gameMap[currentNode->position.x - 1]
                          [currentNode->position.y];
          tempNode2 = gameMap[currentNode->position.x]
                          [currentNode->position.y - 1];
        }
        else
          costToSquare = 32;

        if(costToSquare == 45)
        {
          if(tempNode1.isWalkable == unwalkable ||
             tempNode2.isWalkable == unwalkable)
            continue;
        }

        if(adjacentNode->inOpenList != openListId)
        {
          //G
          adjacentNode->G = currentNode->G + costToSquare;
          //H
          sf::Vector2f tileCoordinates = GetCurrentTilePosition
              (currentNode->position.x + adjacentX[i],
               currentNode->position.y + adjacentY[i]);
          adjacentNode->H = ComputeHCost(tileCoordinates,target);
          //F
          adjacentNode->F= adjacentNode->G + adjacentNode->H;
          //parent
          adjacentNode->parent = currentNode;
          //openlist
          adjacentNode->inOpenList = openListId;
          adjacentNode->position.x = currentNode->position.x + adjacentX[i];
          adjacentNode->position.y = currentNode->position.y + adjacentY[i];
          openList.push_back(*adjacentNode);
        }
        else
        {
          if(currentNode->G + costToSquare < adjacentNode->G)
          {
            adjacentNode->parent = currentNode;
            sf::Vector2f tileCoordinates = GetCurrentTilePosition
              (currentNode->position.x +adjacentX[i],
               currentNode->position.y + adjacentY[i]);
            adjacentNode->H = ComputeHCost(tileCoordinates,target);
            adjacentNode->F= adjacentNode->G + adjacentNode->H;
          }
        }
      }
    }
   PopNodeOpenList(currentNode);
   closedList.push_back(*currentNode);
   if(currentNode->position.x == targetSquare.x &&
      currentNode->position.y == targetSquare.y)
   {
     std::cout<<"A* Found solution"<<std::endl;
     while(currentNode!=NULL)
     {
         sf::Vector2i temp = currentNode->position * tileSize;
         temp.y +=tileSize;
         targetList.push_front(temp);
        currentNode = currentNode->parent;
     }
     targetList.pop_front();
     break;
   }
  }
  openList.clear();
  closedList.clear();
  openListId += 5;
  closedListId += 5;
  return true;
}


void A_star::AddUnwalkables(sf::RectangleShape border)
{
  sf::Vector2f topLeftPosition, bottomRightPosition;
  topLeftPosition.x = border.getPosition().x;
  topLeftPosition.y = border.getPosition().y;
  bottomRightPosition.x = topLeftPosition.x + border.getSize().x;
  bottomRightPosition.y = topLeftPosition.y + border.getSize().y;
  sf::Vector2i topLeftSquare = GetBorderCurrentTile(topLeftPosition);
  sf::Vector2i bottomRightSquare = GetBorderCurrentTile(bottomRightPosition);

  for(int i = topLeftSquare.x; i < bottomRightSquare.x; i++)
  {
    //border can sometimes surpass the size of our map
    if(i < this->mapWidth && i >= 0)
    {
      for(int j = topLeftSquare.y; j < bottomRightSquare.y; j++)
      {
        if( j < this->mapHeight && j >= 0)
          gameMap[i][j].isWalkable = unwalkable;
      }
    }
  }
}

sf::Vector2i A_star::GetBorderCurrentTile(sf::Vector2f p)
{
  sf::Vector2i temp;
  temp.x = p.x/tileSize;
  if(static_cast<int>(p.x) % (tileSize) >=16)
    temp.x++;
  temp.y = p.y/tileSize;
  if(static_cast<int>(p.y) % tileSize >=16)
    temp.y++;
  return temp;
}

sf::Vector2i A_star::GetCurrentTile(sf::Vector2f p)
{
  sf::Vector2i temp;
  temp.x = p.x/tileSize;
  temp.y = p.y/tileSize;
  return temp;
}


sf::Vector2f A_star::GetCurrentTilePosition(int x, int y)
{
  sf::Vector2f temp;
  temp.x = x * tileSize;
  temp.y = y * tileSize;
  return temp;
}

float A_star::ComputeHCost(sf::Vector2f pos, sf::Vector2f tar)
{
  float distanceX = abs(tar.x - pos.x);
  float distanceY = abs(tar.y - pos.y);
  return sqrt(pow(distanceX,2) + pow(distanceY,2));
}

node* A_star::GetNextNode()
{
  if(openList.empty())
    return NULL;
  std::list<node>::iterator it = openList.begin();
  node* lowestFNode = &(*it);
  it++;
  while( it != openList.end())
  {
    if((*it).F < lowestFNode->F)
    {
      lowestFNode = &(*it);
    }
    it++;
  }
  return lowestFNode;
}

void A_star::PopNodeOpenList(node* currentNode)
{
  for(std::list<node>::iterator it = openList.begin(); it!= openList.end(); ++it)
  {
    if((*it).position.x == currentNode->position.x &&
       (*it).position.y == currentNode->position.y)
    {
      openList.erase(it);
      break;
    }
  }
}

bool A_star::objectCollision(node* adjacentNode,sf::Rect<float> boundingRect)
{
  int rectWidth = boundingRect.width/tileSize;
  if(static_cast<int>(boundingRect.width) % tileSize != 0)
    rectWidth += 1;

  int rectHeight = boundingRect.height/tileSize;
  if(static_cast<int>(boundingRect.height) % tileSize != 0)
    rectHeight += 1;

  for(int i = 0; i < rectWidth; i++)
  {
    for(int j = 0; j < rectHeight; j++)
    {
      if(adjacentNode->position.x + i < mapWidth &&
         adjacentNode->position.y + j < mapHeight &&
         adjacentNode->position.x + i >= 0 &&
         adjacentNode->position.y + j >= 0)
      {
        if((gameMap[adjacentNode->position.x + i]
          [adjacentNode->position.y - j]).isWalkable == unwalkable)
        return true;
      }

    }
  }
  return false;
}
