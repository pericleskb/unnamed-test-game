#include "Tileset.hpp"
#include <iostream>
using namespace std;

TileSet::TileSet(){

}

TileSet::TileSet(const TileSet& tileSet)
{
      this->firstgid = tileSet.firstgid;
      this->name = tileSet.name;
      this->tileWidth = tileSet.tileWidth;
      this->tileHeight = tileSet.tileHeight;
      this->source = tileSet.source;
      this->imageWidth = tileSet.imageWidth;
      this->imageHeight = tileSet.imageHeight;
      tileAmountWidth = imageWidth / tileWidth;
      lastgid = tileAmountWidth * (imageHeight / tileHeight) + firstgid - 1;
      sf::Image image;
      if(image.loadFromFile(source) != true){
        throw("Image not found");
      }

      this->tilesetTexture.loadFromImage(image);
      //tilesetTexture.setSmooth(true);
}
