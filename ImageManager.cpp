#include "ImageManager.hpp"
#include "pugixml/pugixml.hpp"
#include <fstream>
#include <string.h>
#include "SFML/Graphics.hpp"

//sprite_sheets.txt contains all the xml files with meta-data
//for loading the sprites. We load all of them when the game starts
void ImageManager::init()
{
  std::cout << "Init" << std::endl;
  std::string line;
  std::ifstream myfile("res/sprite_sheets.txt");
  if (myfile.is_open())
  {
  std::cout << "file is open" << std::endl;
    while ( getline (myfile,line) )
    {
      if(!loadSpriteSheetData(line))
      {
        throw("Error reading XML.");
        exit(1);
      }
    }
    myfile.close();
  }
}

int ImageManager::loadSpriteSheetData(std::string spriteSheetName)
{
  pugi::xml_document doc;
  if (!doc.load_file(spriteSheetName.c_str())) return -1;

  pugi::xml_node atlas = doc.child("TextureAtlas");
  std::string texturePath;
  texturePath = atlas.attribute("imagePath").value();

  //for every Animation
  pugi::xml_node animation = atlas.first_child();
  while( animation )
  {
    std::string animationName;
    animationName = animation.attribute("name").value();
    std::cout << "Animation Name: " << animationName << std::endl;
    std::vector<SpriteData> spritesVec;
    int width = atoi(animation.attribute("width").value());
    int height = atoi(animation.attribute("height").value());

    if(strcmp(animation.name(),"Animation")==0)
    {
      int positionY = atoi(animation.attribute("positionY").value());
      int numOfSprites = atoi(animation.attribute("amount").value());
      //get node attribute "name"
      for(int i=0; i < numOfSprites; i++)
      {
        SpriteData sprite;
        sprite.spriteSheetPath = texturePath;
        //every sprite of an animation is at the same line
        sprite.y = positionY * height;
        sprite.width = width;
        sprite.height = height;
        sprite.x = i * width;
        spritesVec.push_back(sprite);
      }
    }
    if(strcmp(animation.name(),"One_Animation")==0)
    {
      int rows = atoi(animation.attribute("rows").value());
      int amountPerRow = atoi(animation.attribute("amountPerRow").value());
      for(int i=0; i < rows; i++)
      {
        for(int j = 0; j < amountPerRow; j++)
        {
          SpriteData sprite;
          sprite.spriteSheetPath = texturePath;
          //every sprite of an animation is at the same line
          sprite.x = j * width;
          sprite.y = i * height;
          sprite.width = width;
          sprite.height = height;
          spritesVec.push_back(sprite);

        }
      }
    }


    _gameImages.insert(std::pair< std::string, std::vector<SpriteData> >
                       (animationName,spritesVec));

    animation = animation.next_sibling();
  }

  return 1;
}


bool ImageManager::createAnimation(Animation& anim,sf::Texture& texture,
                                    std::string id)
{
  SpriteData spriteData;
  std::vector<SpriteData> animations = _gameImages.find(id)->second;
  //check for empty animations is unnecessary
  if(texture.loadFromFile(animations[0].spriteSheetPath) == false)
  {
    return false;
  }
  anim.setSpriteSheet(texture);

  for(unsigned int i =0; i < animations.size(); i++)
  {
    anim.addFrame(sf::IntRect(animations[i].x,animations[i].y,
                  animations[i].width,animations[i].height));
  }
  return true;
}

