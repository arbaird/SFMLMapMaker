//
//  Grid.hpp
//  MapMaker
//
//  Created by Austin Baird on 1/2/17.
//  Copyright © 2017 Austin Baird. All rights reserved.
//

#ifndef Grid_hpp
#define Grid_hpp

#include <stdio.h>
#include "Window.hpp"


class Grid
{
public:
    Grid(Window& window);
    
    void draw(sf::RenderWindow* window);
    
private:
    //Window window;
    std::vector<sf::RectangleShape> gridLines;
    sf::Vector2f windowSize;
};

#endif /* Grid_hpp */
