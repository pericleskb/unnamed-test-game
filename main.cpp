#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "PugiMapLoader.hpp"
#include "Game.hpp"

using namespace std;

int main(){


  Game::Start();
  return 0;

/*
  vector<TileSet> tileSetVector;
  vector< vector<tileValue> > tileValueVector;
  pugiMapLoader loader;
  loader.loadTileSets(tileSetVector,tileValueVector);
  for(vector<TileSet>::iterator it = tileSetVector.begin();
      it!=tileSetVector.end();++it)
  {
    std::cout << *it <<std::endl;
  }

  /*for(vector<tileValue>::iterator it = tileValueVector.begin();
      it!=tileValueVector.end();++it)
  {
    std::cout << *it <<std::endl;
  }

//just the 1st layer for now
  TileSet groundSet = tileSetVector[0];
  //we will draw the sprites on the RenderTexture object
  sf::RenderTexture ground;
  if(!ground.create(640,640,false))
  {
    return -1;
  }

  for(unsigned int i =0; i<tileValueVector.size(); i++)
  {
    for(unsigned int j =0; j<tileValueVector[i].size(); j++)
    {
      tileValue temp = tileValueVector[i][j];
      int xOfTile = ((temp.val % 12)-1) * 32;
      int yOfTile = (temp.val / 12) * 32;
      sf::Sprite tempSprite = sf::Sprite(*groundSet.getTilesettexture(),
        sf::IntRect(xOfTile,yOfTile,32,32));
      std::cout << "taken from x: " << xOfTile << " y: "<<yOfTile<<std::endl;
      xOfTile = (temp.pos % 20) * 32;
      yOfTile = (temp.pos / 20) * 32;
      std::cout << "taken to x: " << xOfTile << " y: "<<yOfTile<<std::endl;
      //ground.draw(tempSprite);
      tempSprite.setPosition(xOfTile,yOfTile);
      ground.draw(tempSprite);
    }
  }
  //sf::Sprite tileSprite = sf::Sprite(groundSet.getTilesettexture());
  //ground.draw(tileSprite,sf::RenderStates::Default);
 // sf::Sprite tempSprite = sf::Sprite(*(groundSet.getTilesettexture()),
  //   sf::IntRect(32,32,32,32));

  //  ground.draw(tempSprite);
  ground.display();
  sf::Sprite sprite(ground.getTexture());

  sf::RenderWindow window(sf::VideoMode(640,640),"Who Knows?");
  window.setFramerateLimit(60);

  while(window.isOpen())
  {
    sf::Event event;
    while(window.pollEvent(event))
    {
      if(event.type == sf::Event::Closed)
      {
          window.close();
      }
    }
    window.clear();
    window.draw(sprite);
    window.display();

  }*/
}
