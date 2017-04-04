//
//  CentralManager.hpp
//  MapMaker
//
//  Created by Austin Baird on 1/2/17.
//  Copyright © 2017 Austin Baird. All rights reserved.
//

#ifndef CentralManager_hpp
#define CentralManager_hpp

#include <stdio.h>
#include "Window.hpp"
#include "Grid.hpp"
#include "TextureManager.h"
#include "Cursor.hpp"
#include "SharedContext.h"
#include "Selector.hpp"

class Map;

class CentralManager
{
private:
    Window window;
    Window selectionWindow;
    sf::Clock clock;
    sf::Time elapsed;
   
    Grid grid;
    sf::View view;
    sf::Vector2f position;
    
    Cursor cursor;
    Cursor selectionCursor;
    Map* map;
    Selector selector;
    
    SharedContext* context;
    
    void respondToClick(EventDetails* details);
    void moveCamera(EventDetails* details);
    void select(EventDetails* details);
    void save(EventDetails* details);
    
public:
    CentralManager();
    ~CentralManager();
    
    void handleInput();
    void update();
    void updateCamera();
    void render();
    Window* getWindow();
    
    sf::Time getElapsed();
    void restartClock();
    
    TextureManager textureManager;
};
#endif /* CentralManager_hpp */
