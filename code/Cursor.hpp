//
//  Cursor.hpp
//  MapMaker
//
//  Created by Austin Baird on 1/2/17.
//  Copyright © 2017 Austin Baird. All rights reserved.
//

#ifndef Cursor_hpp
#define Cursor_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "Map.hpp"

struct Tile;

enum class Direction
{
    Up, Down, Left, Right
};
class Cursor
{
public:
    Cursor(sf::Vector2f size);
    
    void move(Direction dir);
    void draw(sf::RenderWindow* window);
    void setPosition(const sf::Vector2f& pos);
    
    Tile getCurrentTile();
    sf::Vector2f getPosition(); //screen position
    sf::Vector2f& getCoords(); //map coordinates
    
private:
    
    sf::Vector2f position;
    sf::RectangleShape cursor;
    Tile currentTile;
};

#endif /* Cursor_hpp */
