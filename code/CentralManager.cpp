//
//  CentralManager.cpp
//  MapMaker
//
//  Created by Austin Baird on 1/2/17.
//  Copyright © 2017 Austin Baird. All rights reserved.
//

#include "CentralManager.hpp"
#include "Map.hpp"


float frametime = 1.0f/60.0f;

CentralManager::CentralManager() : selectionWindow("Selections", {256, 608}), window("RPG", sf::Vector2u(800, 608)), grid(window), cursor({32, 32}), selectionCursor({32, 32})
{
    
    //textureManager = new TextureManager();
    context->textureManager = &textureManager;
    context->window = &window;
    position = sf::Vector2f(window.getWindowSize().x / 2, window.getWindowSize().y / 2);
    
    clock.restart();
    srand(time(nullptr));
    
    sf::Vector2u size = window.getWindowSize();
    view.setSize(size.x, size.y);
    view.setCenter(size.x / 2, size.y / 2);
    //view.zoom(0.5f); //play with this number
    window.getRenderWindow()->setView(view);
    
    EventManager* eventMgr = window.getEventManager();
    eventMgr->addCallback(StateType(0), "MoveLeft", &CentralManager::moveCamera, this);
    eventMgr->addCallback(StateType(0), "MoveRight", &CentralManager::moveCamera, this);
    eventMgr->addCallback(StateType(0), "MoveUp", &CentralManager::moveCamera, this);
    eventMgr->addCallback(StateType(0), "MoveDown", &CentralManager::moveCamera, this);
    eventMgr->addCallback(StateType(0), "Select", &CentralManager::select, this);
    eventMgr->addCallback(StateType(0), "Save", &CentralManager::save, this);
    
    EventManager* selectEventMgr = selectionWindow.getEventManager();
    selectEventMgr->addCallback(StateType(0), "Click_Response", &CentralManager::respondToClick, this);
    
    map = new Map(context);
    map->loadMap("map.txt");
    
    selector.setTileSet(map);
    
    selectionWindow.setPosition({150, 75});
}

CentralManager::~CentralManager()
{
    
    restartClock();
}

sf::Time CentralManager::getElapsed()
{
    return elapsed;
}
void CentralManager::restartClock()
{
    elapsed += clock.restart();
    //fixedTimeElapsed += clock.restart();
    //commented out code would implement fixed time step. not using here because we want to count how many seconds pass for our intro "animation". perhaps in future add separate restart function, or separate time variable for fixed timesteps???
}
void CentralManager::update()
{
    
    window.update();
    selectionWindow.update();
    updateCamera();
    restartClock();
}

void CentralManager::updateCamera()
{
    sf::Vector2f mapSize(32, 32);
    int tileSize = 32;
    
    view.setCenter(cursor.getPosition());
    window.getRenderWindow()->setView(view);
    sf::FloatRect viewSpace = window.getViewSpace();
    if(viewSpace.left <= 0)
    {
        position.x += 1;
        //cursor.move(Direction::Right);
        view.setCenter(viewSpace.width / 2, view.getCenter().y);
        window.getRenderWindow()->setView(view);
    }
    else if(viewSpace.left + viewSpace.width > (mapSize.x) * tileSize)
        //viewSpace off of right side of screen
    {
        position.x -= 1;
        //cursor.move(Direction::Left);
        view.setCenter(((mapSize.x) * tileSize) - (viewSpace.width / 2),
                       view.getCenter().y);
        window.getRenderWindow()->setView(view);
    }
    
    if(viewSpace.top <= 0)
    {
        position.y += 1;
        //cursor.move(Direction::Down);
        view.setCenter(view.getCenter().x, viewSpace.height / 2);
        window.getRenderWindow()->setView(view);
    }
    else if(viewSpace.top + viewSpace.height > (mapSize.y) * tileSize)
    {
        position.y -= 1;
        //cursor.move(Direction::Up);
        view.setCenter(view.getCenter().x,
                       ((mapSize.y * tileSize) - (viewSpace.height / 2)));
        window.getRenderWindow()->setView(view);
    }

}




void CentralManager::render()
{
    window.beginDraw(); //clear
    selectionWindow.beginDraw();
    
    for(int i = 0; i < Sheet::Num_Layers; i++)
        map->draw(i);
   
    grid.draw(window.getRenderWindow());
    cursor.draw(window.getRenderWindow());
    
    selector.draw(selectionWindow.getRenderWindow());
    selectionCursor.draw(selectionWindow.getRenderWindow());
    
    window.endDraw(); //display
    selectionWindow.endDraw();
}


Window* CentralManager::getWindow()
{
    return &window;
}

void CentralManager::respondToClick(EventDetails* details)
{
    
    std::cout << details->mouse.x / 32 << details->mouse.y / 32 << std::endl;
    selectionCursor.setPosition(sf::Vector2f(details->mouse.x / 32, details->mouse.y / 32));
    int id = (details->mouse.x / 32) + (8 * (details->mouse.y / 32));
    if(id < 64)
    {
        selector.setSelectedTile(id);
        return;
    }
    if(id <68)
    {
        selector.setLayer(id - 64); //boxes 64 - 68 are layers 0 - 4
        return;
    }
    if(id == 68)
    {
        selector.toggleDeleteMode();
    }
    if(id == 69)
    {
        selector.toggleSolid();
    }
    
}

void CentralManager::moveCamera(EventDetails* details)
{

    if(elapsed.asSeconds() < 0.1f)
        return;
    elapsed = clock.restart();
    
    if(details->name == "MoveLeft")
        //position.x -= 1;
        cursor.move(Direction::Left);
    else if(details->name == "MoveRight")
        //position.x += 1;
        cursor.move(Direction::Right);
    else if(details->name == "MoveUp")
        //position.y -= 1;
        cursor.move(Direction::Up);
    else if(details->name == "MoveDown")
        cursor.move(Direction::Down);
        //position.y += 1;
}

void CentralManager::select(EventDetails* details)
{
    if(elapsed.asSeconds() < 0.1f)
        return;
    elapsed = clock.restart();
    
    if(selector.getSelectedTile() == -1 && !selector.inDeleteMode()) //current tile is nullptr
        return;
    
    sf::Vector2f cursorPos = cursor.getCoords();
   
    if(selector.inDeleteMode())
        map->deleteTile(cursorPos.x, cursorPos.y, selector.getLayer());
    else
    {
        map->deleteTile(cursorPos.x, cursorPos.y, selector.getLayer()); //doesn't delete tile if it does not exist
        TileID tile = selector.getSelectedTile();
        map->addTile(cursorPos.x, cursorPos.y, selector.getLayer(), selector.isSolid(), tile);
    }
 
}

void CentralManager::save(EventDetails* details)
{
    
    if(elapsed.asSeconds() < 0.1f)
        return;
    elapsed = clock.restart();
    
    
    std::cout << "SAVING!" << std::endl;
    int fromX = 0;
    int toX = map->getMapSize().x;
    int fromY = 0;
    int toY = map->getMapSize().y;
    
    std::ofstream outfile;
    outfile.open("newMap.txt");
    if(!outfile.is_open())
        std::cout << "Can't open outfile" << std::endl;
    
    for(int x = fromX; x <= toX; x++)
    {
        for(int y = fromY; y <= toY; y++)
        {
            for(int l = 0; l < Sheet::Num_Layers; l++)
            {

                Tile* tile = map->getTile(x, y, l);
                if(!tile)
                   continue;
                outfile << "TILE " << tile->properties->id << " " << x << " " << y << " " << l << " " << tile->solid << std::endl;
            }
        }
    }
    
    outfile.close();
    
}
