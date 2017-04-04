//
//  SharedContext.h
//  MapMaker
//
//  Created by Austin Baird on 1/2/17.
//  Copyright © 2017 Austin Baird. All rights reserved.
//

#ifndef SharedContext_h
#define SharedContext_h


#include "Window.hpp"
#include "TextureManager.h"

class Map;

struct SharedContext
{
    SharedContext() : eventManager(nullptr), window(nullptr),  textureManager(nullptr),
    gameMap(nullptr) {}
    EventManager* eventManager;
    Window* window;
    TextureManager* textureManager;
    Map* gameMap;

};

#endif /* SharedContext_h */
