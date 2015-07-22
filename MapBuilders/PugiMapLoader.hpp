#pragma once
#include "Tileset.hpp"
#include "../ObjectManager.hpp"
#include "../A-star.hpp"
#include <vector>

struct tileData{
    int pos;
    int val;
};

struct layerData{
  std::string layerName;
  std::vector<tileData> data;
};

class pugiMapLoader
{
public:
    int loadMap(std::string ,std::vector<TileSet>&,std::vector <layerData>&,
                ObjectManager&,sf::Vector2i*,sf::Vector2i*,A_star*);

private:

};


inline std::ostream& operator<<(std::ostream &strm, const layerData &a) {
    std::string print = " name: ";
    print.append(a.layerName);
    return strm << print;
}
