//
//  Tile.h
//  MapMaker
//
//  Created by Austin Baird on 1/1/17.
//  Copyright © 2017 Austin Baird. All rights reserved.
//

#ifndef Tile_h
#define Tile_h

typedef unsigned int TileID;

//flyweight pattern
struct TileInfo
{
    TileInfo(CentralManager* centralMgr, const std::string texture = "", TileID ID = 0)
    :  id(0), deadly(false), centralManager(centralMgr)
    {
        TextureManager* textMgr = centralManager->textureManager;
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
        if(centralManager->textureManager->getSize())
            centralManager->textureManager->releaseResource(textureName);
        //without this if check, double deletion may occur at program completion
    }
    
    sf::Sprite sprite;
    
    CentralManager* centralManager;
    
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



#endif /* Tile_h */
