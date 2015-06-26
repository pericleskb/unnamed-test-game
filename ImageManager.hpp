#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "Animation.hpp"

//This should be available to all without the need to include
//maybe in game???
//See what you include in visible object
struct SpriteData{
  int x;
  int y;
  int width;
  int height;
  //bool rotated;
  std::string spriteSheetPath;
};

typedef std::map<std::string, std::vector<SpriteData> > imageLoadMap_t;

class ImageManager
{
public:
  void init();
  bool createAnimation(Animation& anim,sf::Texture& texture, std::string id);
private:
  int loadSpriteSheetData(std::string spriteSheetName);
  imageLoadMap_t _gameImages;

};
