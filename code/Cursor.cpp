//
//  Cursor.cpp
//  MapMaker
//
//  Created by Austin Baird on 1/2/17.
//  Copyright © 2017 Austin Baird. All rights reserved.
//

#include "Cursor.hpp"


Cursor::Cursor(sf::Vector2f size)
{
    cursor.setSize(size);
    cursor.setPosition(0, 0);
    cursor.setFillColor(sf::Color(255, 255, 255,128));
    cursor.setOutlineColor(sf::Color(90, 90, 90));
    cursor.setOrigin({0, 0});
    position = sf::Vector2f(0, 0);
}

void Cursor::move(Direction dir)
{
    switch(dir)
    {
        case Direction::Up:
            position.y -= 1;
            break;
        case Direction::Down:
            position.y += 1;
            break;
        case Direction::Right:
            position.x +=1;
            break;
        case Direction::Left:
            position.x -= 1;
            break;
            
    }
    cursor.setPosition(position.x * 32, position.y * 32);
    
}

void Cursor::setPosition(const sf::Vector2f& pos)
{
    position = pos;
    cursor.setPosition(pos.x * 32, pos.y * 32);
}

Tile Cursor::getCurrentTile()
{
    return currentTile;
}

sf::Vector2f Cursor::getPosition()
{
    sf::Vector2f screenPos = sf::Vector2f(position.x * 32, position.y * 32);
    return screenPos;
}

sf::Vector2f& Cursor::getCoords()
{
    return position;
}

void Cursor::draw(sf::RenderWindow* window)
{
    window->draw(cursor);
}
