#pragma once
#include <string>
#include "SFML/Graphics.hpp"

class TileSet
{

public:
    TileSet();

    TileSet(const TileSet&);

    friend std::ostream& operator<<(std::ostream &strm, const TileSet &a);

    void setFirstGid(int firstGid)
    {
        this->firstgid = firstGid;
    }

    void setName(std::string name){
        this->name = name;
    }

    void setTileWidth(int tileWidth){
        this->tileWidth = tileWidth;
    }

    void setTileHeight(int tileHeight){
        this->tileHeight = tileHeight;
    }

    void setSource(std::string source){
        this->source = source;
    }

    void setImageWidth(int imageWidth){
        this->imageWidth = imageWidth;
    }

    void setImageHeight(int imageHeight){
        this->imageHeight = imageHeight;
    }

    sf::Texture* getTilesetTexture(){
        return &(this->tilesetTexture);
    }

    int getLastGid(){
      return this->lastgid;
    }

    int getFirstGid(){
      return this->firstgid;
    }

    int getTileAmountWidth(){
      return this->tileAmountWidth;
    }

    int getTileWidth(){
      return this->tileWidth;
    }

    int getTileHeight(){
      return this->tileHeight;
    }

private:

   int firstgid;
   std::string name;
   int lastgid;
   int tileWidth;
   std::string source;
   int tileHeight;
   int imageWidth;
   int imageHeight;
   sf::Texture tilesetTexture;
   int tileAmountWidth;

};

inline std::ostream& operator<<(std::ostream &strm, const TileSet &a) {
    std::string _name("Name: "),_firstgid(" Firstgid: "),_lastgid("Lastgid:");
    _name += a.name;
    _name += _firstgid;
    _name += std::to_string(a.firstgid);
    _name += _lastgid;
    _name += std::to_string(a.lastgid);
    return strm << _name;
}


