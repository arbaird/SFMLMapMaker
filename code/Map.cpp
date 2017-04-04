//
//  Map.cpp
//  Ch7-GameDesign
//
//  Created by Austin Baird on 12/3/16.
//  Copyright © 2016 Austin Baird. All rights reserved.
//

#include "Map.hpp"

Map::Map(SharedContext* cont) :
context(cont), defaultTile(cont), maxMapSize(32, 32), tileCount(0), tileSetCount(0),
mapGravity(512.f), loadNextMap(false), playerId(-1)
{
    //context->gameMap = this;
    loadTiles("tiles.cfg.txt");
}

Map::~Map()
{
    purgeMap();
    purgeTileSet();
    //context->gameMap = nullptr;
}

Tile* Map::getTile(unsigned int x, unsigned int y, unsigned int layer)
{
    if(x >= maxMapSize.x || y > maxMapSize.y || layer >= Sheet::Num_Layers)
        return nullptr;
    auto itr = tileMap.find(convertCoords(x, y, layer));
    if(itr != tileMap.end())
        return itr->second;
    else
        return nullptr;
}
unsigned int Map::convertCoords(unsigned int x, unsigned int y, unsigned int layer)
{
    return ((layer*maxMapSize.y + y) * maxMapSize.x + x); //convert 2D grid of map to 3D cube plot, 3D dimmension being the layer
}

TileInfo* Map::getDefaultTile()
{
    return &defaultTile;
}

float Map::getGravity() const
{
    return mapGravity;
}

unsigned int Map::getTileSize() const
{
    return Sheet::Tile_Size;
}

const sf::Vector2u& Map::getMapSize() const
{
    return maxMapSize;
}
const sf::Vector2f& Map::getPlayerStart() const
{
    return playerStart;
}

void Map::loadMap(const std::string& path)
{
    std::ifstream file;
    file.open(resourcePath() + path);
    if(!file.is_open())
    {
        std::cerr << "! Failed to load map file: " << path << std::endl;
        return;
    }
    
    std::cout << "--- Loading a map: " << path << std::endl;
    
    
    std::string line;
    while(std::getline(file, line))
    {
        if(line[0] == '|')
            continue; // | is a comment
        std::stringstream keystream(line);
        std::string type;
        keystream >> type;
        if(type == "TILE")
        {
            int tileId = 0;
            keystream >> tileId;
            if(tileId < 0)
            {
                std::cout << "! Bad tile id: " << tileId << std::endl;
                continue;
            }
            auto itr = tileSet.find(tileId);
            if(itr == tileSet.end())
            {
                std::cout << "! Tile id(: " << tileId << ") was not found in tileset." << std::endl;
                continue;
            }
            sf::Vector2i tileCoords;
            unsigned int tileLayer = 0;
            unsigned int tileSolidity = 0;
            keystream >> tileCoords.x >> tileCoords.y >> tileLayer >> tileSolidity;
            if(tileCoords.x > maxMapSize.x || tileCoords.y > maxMapSize.y || tileLayer >= Sheet::Num_Layers)
            {
                std::cout << "! Tile is out of range: " << tileCoords.x << " " << tileCoords.y << std::endl;
                continue;
            }
            Tile* tile = new Tile();
            //Bind properties of a tile from a set.
            tile->properties = itr->second;
            tile->solid = tileSolidity;
            if(!tileMap.emplace(convertCoords(tileCoords.x, tileCoords.y, tileLayer), tile).second)
            {
                std::cout << "! Duplicate tile: " << tileCoords.x << " " << tileCoords.y << std::endl;
                delete tile;
                tile = nullptr;
                continue;
            }
            std::string warp;
            keystream >> warp;
            tile->warp = false;
            if(warp == "WARP")
                tile->warp = true;
        }
        
        else if(type == "BACKGROUND")
        {
            /*if(backgroundTexture != "") //if bgTexture is already set, continue
            {
                std::cout << "Bg continue" << std::endl;
                continue;
            }
            keystream >> backgroundTexture;
            if(!context->textureManager->requireResource(backgroundTexture))
            {
                std::cout << "Bg not required, continue" << std::endl;
                backgroundTexture = "";
                continue;
            }
            sf::Texture* texture = context->textureManager->getResource(backgroundTexture);
            background.setTexture(*texture);
            sf::Vector2f viewSize = currentState->getView().getSize();
            sf::Vector2u textureSize = texture->getSize();
            sf::Vector2f scaleFactors;
            scaleFactors.x = viewSize.x / textureSize.x;
            scaleFactors.y = viewSize.y / textureSize.y;
            //find scaleFacotr so that backgournd.png will fill entire view and not leave blank spaces
            background.setScale(scaleFactors);*/
        }
        else if(type == "ENTITY")
        {
            /*std::string name;
            keystream >> name;
            if (name == "Player" && playerId != -1)
                continue;
            int entityId = context->entityManager->addEntity(name);
            if (entityId < 0)
            {
                std::cout << "EntityId < 0 after addEntity(name) in map.loadMap()" << std::endl;
                continue;
            }
            std::cout << "entityId in map is: " << entityId << std::endl;
            if(name == "Player")
            {
                playerId = entityId;
                std::cout << "playerId in Map::loadMap() is " << playerId << std::endl;
            }
            
            ComponentBase* position = context->entityManager->getComponent<ComponentPosition>(entityId,Component::Position);
            if(position)
            {
                std::cout << "Reading to position!" <<  position << std::endl;
                keystream >> *position;
                
            }*/
        }
        
        else if(type == "SIZE")
            keystream >> maxMapSize.x >> maxMapSize.y;
        else if(type == "GRAVITY")
            keystream >> mapGravity;
        else if(type == "DEFAULT_FRICTION")
            keystream >> defaultTile.friction.x >> defaultTile.friction.y;
        else if(type == "NEXTMAP")
            keystream >> nextMap;
        
    }
    std::cout << "Map loaded!!!" << std::endl;
}
void Map::loadNext()
{
    loadNextMap = true;
}
void Map::update(float deltaTime)
{
    if(loadNextMap)
    {
        purgeMap();
        loadNextMap = false;
        if(nextMap != "")
            loadMap(nextMap);
        /*else
            currentState->getStateManager()->switchTo(StateType::GameOver);
        */
        
        nextMap = "";
        
    }
    sf::FloatRect viewSpace = context->window->getViewSpace();
    background.setPosition(viewSpace.left, viewSpace.top);
}
void Map::draw(unsigned int layer)
{
    if(layer >= Sheet::Num_Layers)
        return;
    sf::RenderWindow* window = context->window->getRenderWindow();
    sf::FloatRect viewSpace = context->window->getViewSpace();
    //use this viewSpace to know what sprite to draw; if sprite aren't in viewspace, don't bother drawing
    sf::Vector2i tileBegin(floor(viewSpace.left / Sheet::Tile_Size),
                           floor(viewSpace.top / Sheet::Tile_Size));
    sf::Vector2i tileEnd(ceil((viewSpace.left + viewSpace.width) / Sheet::Tile_Size),
                         ceil((viewSpace.top + viewSpace.height) / Sheet::Tile_Size));
    
    unsigned int count = 0;
    for(int x = tileBegin.x; x <= tileEnd.x; x++)
    {
        for(int y = tileBegin.y; y <= tileEnd.y; y++)
        {
            Tile* tile = getTile(x, y, layer);
            if(!tile)
                continue;
            sf::Sprite currentSprite = tile->properties->sprite;
            currentSprite.setPosition(x * Sheet::Tile_Size, y * Sheet::Tile_Size);
            //if(tile->solid)
               // currentSprite.setColor(sf::Color(255, 255, 255, 128));
            window->draw(currentSprite);
            count++;
            
        }
    }
    
}

void Map::loadTiles(const std::string& path)
{
    std::ifstream file;
    file.open(resourcePath() + path);
    if(!file.is_open())
    {
        std::cerr << "! Failed to load tileset file: " << path << std::endl;
        return;
    }
    
    std::string line;
    while(std::getline(file, line))
    {
        if(line[0] == '|')
            continue; // | is a comment
        std::stringstream keystream(line);
        int tileId;
        keystream >> tileId;
        if(tileId < 0)
            continue;
        TileInfo* tile = new TileInfo(context, "TileSheet", tileId);
        keystream >> tile->name >> tile->friction.x >> tile->friction.y >> tile->deadly;
        if(!tileSet.emplace(tileId, tile).second)
        {
            std::cerr << "! Duplicate tile type: " << tile->name << std::endl;
            delete tile;
        }
    }
    file.close();
    
}

void Map::purgeTileSet()
{
    for(auto &itr : tileSet)
        delete itr.second;
    tileSet.clear();
    tileSetCount = 0;
}

void Map::purgeMap()
{
    tileCount = 0;
    for(auto &itr : tileMap)
        delete itr.second;
    tileMap.clear();
    /*if(context->entityManager != NULL)
        context->entityManager->purge();*/
    
    if(backgroundTexture == "")
        return;
    std::cout << "PURGING MAP" << std::endl;
    if(context->textureManager->getSize()) //without this check, double deletion occured when program was done
        context->textureManager->releaseResource(backgroundTexture);
    backgroundTexture = "";
}

int Map::getPlayerId()const
{
    return playerId;
}

void Map::addTile(unsigned int x, unsigned int y, unsigned int layer, bool solid, TileID id)
{
    auto itr = tileSet.find(id);
    
    Tile* tile = new Tile();
    tile->properties = itr->second;
    tile->solid = solid;
    //tile->solid = tileSolidity;
    tileMap.emplace(convertCoords(x, y, layer), tile);
}

void Map::deleteTile(unsigned int x, unsigned int y, unsigned int layer)
{
    auto itr = tileMap.find(convertCoords(x, y, layer));
    if(itr == tileMap.end())
        return;
    delete itr->second;
    TileID tile = convertCoords(x, y, layer);
    tileMap.erase(tile);
}

TileSet* Map::getTileSet()
{
    return &tileSet;
}
