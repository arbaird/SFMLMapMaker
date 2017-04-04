//
//  Grid.cpp
//  MapMaker
//
//  Created by Austin Baird on 1/2/17.
//  Copyright © 2017 Austin Baird. All rights reserved.
//

#include "Grid.hpp"


Grid::Grid(Window& window) : windowSize(window.getWindowSize())
{
    sf::Vector2f mapSize(32, 32);
    int tileSize = 32;
    
    for(int i = 0; i < 32; i++)
    {
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(1, mapSize.y * tileSize));
        
       
        rect.setOrigin(rect.getSize());
        rect.setPosition(sf::Vector2f(i*32, mapSize.y * tileSize));
        gridLines.push_back(rect);
    }
    
    for(int i = 0; i < 32; i++)
    {
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(mapSize.x * tileSize, 1));
        rect.setPosition(mapSize.x * tileSize, i* 32);
        rect.setOrigin(rect.getSize());
        gridLines.push_back(rect);
    }
}

void Grid::draw(sf::RenderWindow* window)
{
    for(auto line : gridLines)
        window->draw(line);
}
