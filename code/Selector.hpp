//
//  Selector.hpp
//  MapMaker
//
//  Created by Austin Baird on 1/2/17.
//  Copyright © 2017 Austin Baird. All rights reserved.
//

#ifndef Selector_hpp
#define Selector_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "Map.hpp"

class Selector
{
public:
    
    Selector();
    
    void setTileSet(Map* map);
    void draw(sf::RenderWindow* window);
    
    void setSelectedTile(TileID id);
    TileID getSelectedTile();
    int getLayer();
    void setLayer(int lay);
    bool inDeleteMode();
    void toggleDeleteMode();
    void toggleSolid();
    bool isSolid();
    
private:
    TileSet* tileSet;
    Tile currentTile;
    
    sf::Font font;
    sf::Text content;
    
    int layer;
    bool deleteMode;
    bool solid;
};

#endif /* Selector_hpp */
