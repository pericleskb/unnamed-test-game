#include "PugiMapLoader.hpp"
#include "../pugixml/pugixml.hpp"
#include <iostream>
#include <string.h>
#include "../Objects/Player.hpp"
#include "../Objects/EnemyOrc.hpp"

using namespace std;

int pugiMapLoader::loadMap(std::string mapName,std::vector<TileSet>& tileSetVector,
  std::vector<layerData>& layerVector, ObjectManager& objectManager,
      sf::Vector2i* mapSize,sf::Vector2i* mapDimensions, A_star* pathFinder)
{
  objectManager.borders.clear();

  pugi::xml_document doc;
  if (!doc.load_file(mapName.c_str())) return -1;

  pugi::xml_node tileMap = doc.child("map");
  *mapSize = sf::Vector2i(atoi(tileMap.attribute("width").value()),
                         atoi(tileMap.attribute("height").value()));

  mapDimensions->x = mapSize->x * atoi(tileMap.attribute("tilewidth").value());
  mapDimensions->y = mapSize->y * atoi(tileMap.attribute("tileheight").value());

  pathFinder->SetSize(mapSize->x,mapSize->y);

  // tag::basic[]
  pugi::xml_node node = tileMap.first_child();
  while( node )
  {
    while(strcmp(node.name(),"tileset")==0)
    {
      TileSet tempTileSet = TileSet();
      tempTileSet.setFirstGid(atoi(node.attribute("firstgid").value()));
      tempTileSet.setName(node.attribute("name").value());
      tempTileSet.setTileWidth(atoi(node.attribute("tilewidth").value()));
      tempTileSet.setTileHeight(atoi(node.attribute("tileheight").value()));
      pugi::xml_node image = node.child("image");
      tempTileSet.setSource(image.attribute("source").value());
      tempTileSet.setImageWidth(atoi(image.attribute("width").value()));
      tempTileSet.setImageHeight(atoi(image.attribute("height").value()));

      TileSet newTileSet = TileSet(tempTileSet);
      tileSetVector.push_back(newTileSet);
      node = node.next_sibling();
    }

    while(strcmp(node.name(),"layer")==0)
    {
      layerData layer;
      layer.layerName = node.attribute("name").value();
      pugi::xml_node data = node.child("data");
      int pos = 0;
      int val;
      for(pugi::xml_node tile = data.first_child(); tile;
        tile = tile.next_sibling("tile"))
      {
        val = atoi(tile.attribute("gid").value());
        if(val != 0)
        {
          tileData gid;
          gid.pos = pos;
          gid.val = val;
          layer.data.push_back(gid);
        }
        pos++;
      }
      layerVector.push_back(layer);
      node = node.next_sibling();
    }

    if(strcmp(node.attribute("name").value(),"Collision")==0)
    {
      for(pugi::xml_node collisionObject = node.first_child(); collisionObject;
        collisionObject = collisionObject.next_sibling("object"))
      {
        sf::RectangleShape rec;
        int x,y,width,height;
        x = atoi(collisionObject.attribute("x").value());
        y = atoi(collisionObject.attribute("y").value());
        width = atoi(collisionObject.attribute("width").value());
        height = atoi(collisionObject.attribute("height").value());
        rec.setPosition(sf::Vector2f(x,y));
        rec.setSize(sf::Vector2f(width,height));

        //the AddUnwalkables function could return a new rec that defines
        //the borders as it interprets them so that there would be a
        //sychronization between those 2 functions
        pathFinder->AddUnwalkables(rec);
        objectManager.borders.push_back(rec);

      }
      node = node.next_sibling();
    }

    if(strcmp(node.attribute("name").value(),"NextLevel")==0)
    {
        pugi::xml_node nextLevelNode = node.first_child();
        int x,y,width,height;
        x = atoi(nextLevelNode.attribute("x").value());
        y = atoi(nextLevelNode.attribute("y").value());
        width = atoi(nextLevelNode.attribute("width").value());
        height = atoi(nextLevelNode.attribute("height").value());
        objectManager.nextLevelRect.left = x;
        objectManager.nextLevelRect.top = y;
        objectManager.nextLevelRect.width = width;
        objectManager.nextLevelRect.height = height;
    }

    if(strcmp(node.name(),"player")==0)
    {
      int x = atoi(node.attribute("x").value());
      int y = atoi(node.attribute("y").value());
      VisibleObject *object = objectManager.Get("player");
      if( object!= NULL)
      {
        object->SetPosition(x,y);
        object->SetStartingPosition(x,y);
      }
      else
      {
        Player* player = new Player(x,y);
        objectManager.Add(node.attribute("id").value(),player);
      }
    }

    if(strcmp(node.name(),"orc")==0)
    {
      int x = atoi(node.attribute("x").value());
      int y = atoi(node.attribute("y").value());
      int width = atoi(node.attribute("width").value());
      int height = atoi(node.attribute("height").value());
      EnemyOrc* enemyOrc = new EnemyOrc(x,y);
      enemyOrc->addGuardingPoint(sf::Vector2f(x,y));
      enemyOrc->addGuardingPoint(sf::Vector2f(x + width,y));
      enemyOrc->addGuardingPoint(sf::Vector2f(x + width,y + height));
      enemyOrc->addGuardingPoint(sf::Vector2f(x ,y + height));

      objectManager.Add(node.attribute("id").value(),enemyOrc);
    }

    node = node.next_sibling();
  }

   return 0;
}





