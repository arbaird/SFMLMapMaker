//
//  Map.hpp
//  Ch7-GameDesign
//
//  Created by Austin Baird on 12/3/16.
//  Copyright © 2016 Austin Baird. All rights reserved.
//

#ifndef Map_hpp
#define Map_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "SharedContext.h"

enum Sheet
{
    Tile_Size = 32,
    Sheet_Width = 256,
    Sheet_Height = 256,
    Num_Layers = 4
};

typedef unsigned int TileID;

//flyweight pattern
struct TileInfo
{
    TileInfo(SharedContext* cont, const std::string texture = "", TileID ID = 0)
    :  id(0), deadly(false), context(cont)
    {
        TextureManager* textMgr = context->textureManager;
        if(texture == "")
        {
            id = ID;
            return;
        }
        if(!textMgr->requireResource(texture))
            return;
        textureName = texture;
        id = ID;
        sprite.setTexture(*textMgr->getResource(texture));
        sf::IntRect tileBounds(
                               id % (Sheet::Sheet_Width / Sheet::Tile_Size) * Sheet::Tile_Size,//mod 8, 8 tiles in each row of tileSheet
                               id / (Sheet::Sheet_Height / Sheet::Tile_Size) *Sheet::Tile_Size,
                               Sheet::Tile_Size,
                               Sheet::Tile_Size
                               );
        sprite.setTextureRect(tileBounds);
    }
    
    ~TileInfo()
    {
        if(textureName == "")
            return;
        if(context->textureManager->getSize())
            context->textureManager->releaseResource(textureName);
        //without this if check, double deletion may occur at program completion
    }
    
    sf::Sprite sprite;
    
    SharedContext* context;
    
    TileID id;
    std::string name;
    sf::Vector2f friction;
    bool deadly;
    
    std::string textureName;
};

struct Tile
{
    TileInfo* properties;
    bool warp; //is the tile a warp
    bool solid;
};


typedef std::unordered_map<TileID, Tile*> TileMap;
typedef std::unordered_map<TileID, TileInfo*> TileSet;

class Map
{
private:
    //method for converting 2D coordinates to 1D ints
    unsigned int convertCoords(unsigned int x, unsigned int y, unsigned int layer);
    void loadTiles(const std::string& path);
    void purgeMap();
    void purgeTileSet();
    
    TileSet tileSet;
    TileMap tileMap;
    
    sf::Sprite background;
    TileInfo defaultTile;
    sf::Vector2u maxMapSize;
    sf::Vector2f playerStart;
    unsigned int tileCount;
    unsigned int tileSetCount;
    float mapGravity;
    std::string nextMap;
    bool loadNextMap;
    std::string backgroundTexture;
    
    int playerId;
    
    SharedContext* context;
    
public:
    Map(SharedContext* cont);
    ~Map();
    Tile* getTile(unsigned int x, unsigned int y, unsigned int layer);
    TileInfo* getDefaultTile();
    float getGravity() const;
    unsigned int getTileSize() const;
    const sf::Vector2u& getMapSize() const;
    const sf::Vector2f& getPlayerStart() const;
    void loadMap(const std::string& path);
    void loadNext();
    void update(float deltaTime);
    void draw(unsigned int layer);
    int getPlayerId()const;
    
    TileSet* getTileSet();
    void deleteTile(unsigned int x, unsigned int y, unsigned int layer);
    void addTile(unsigned int x, unsigned int y, unsigned int layer, bool solid, TileID id);
};


#endif /* Map_hpp */

