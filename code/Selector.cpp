//
//  Selector.cpp
//  MapMaker
//
//  Created by Austin Baird on 1/2/17.
//  Copyright © 2017 Austin Baird. All rights reserved.
//

#include "Selector.hpp"
//#include "Map.hpp"

Selector::Selector()
{
   
    currentTile.properties = nullptr;
    font.loadFromFile(resourcePath() + "sansation.ttf");
    content.setFont(font);
    content.setString("");
    content.setCharacterSize(16);
    content.setColor(sf::Color::White);
    content.setOrigin(-8, -4);
    layer = 0;
    deleteMode = false;
    solid = false;
    //content.setPosition(screenPosition + offset);
}

void Selector::draw(sf::RenderWindow* window)
{
    int row = 0;
    int col = 0;
    for(int i = 0; i < tileSet->size(); i++)
    {
        auto itr = tileSet->find(i);
        TileInfo* info = itr->second;
        sf::Sprite sprite = itr->second->sprite;
        sprite.setPosition(col*32, row*32);
        window->draw(sprite);
        col++;
        if(col >= 8)
        {
            col = 0;
            row++;
        }
    }
    //draw layer options
    for(int i = 0; i < Sheet::Num_Layers; i++)
    {
        content.setString(std::to_string(i));
        content.setPosition(col*32, row*32);
        col++;
        window->draw(content);
    }
    //draw delete option
    content.setPosition(col*32, row*32);
    content.setString("X");
    window->draw(content);
    
    //draw solid option
    col++;
    content.setPosition(col*32, row*32);
    content.setString("S");
    window->draw(content);
    
    //diplay selections
    if(deleteMode)
    {
        content.setPosition(7*32, 8*32);
        content.setString("X");
        window->draw(content);
    }
    
    else if(currentTile.properties != nullptr)
    {
        sf::Sprite sprite = currentTile.properties->sprite;
        window->draw(sprite);
    }
    if(solid)
    {
        content.setPosition(7*32, 10*32);
        content.setString("S");
        window->draw(content);
    }
    
    
    
    
    content.setPosition(7 * 32, 9 * 32);
    content.setString(std::to_string(layer));
    window->draw(content);
    
}

void Selector::setTileSet(Map* map)
{
    tileSet = map->getTileSet();
}

void Selector::setSelectedTile(TileID id)
{
    deleteMode = false;
    auto itr = tileSet->find(id);
    if(itr == tileSet->end())
        std::cout << "! Tile id(: " << id << ") was not found in tileset." << std::endl;
    TileInfo* properties = itr->second;
    
    currentTile.properties = properties;
    sf::Sprite sprite = currentTile.properties->sprite;
    sprite.setPosition(7 * 32, 8 * 32);
    currentTile.properties->sprite = sprite;
    
    if(currentTile.properties == nullptr)
        std::cout << "currentTile->properties is NULL!" << std::endl;
    std::cout << currentTile.properties->name << currentTile.properties->friction.x << std::endl;
}

TileID Selector::getSelectedTile()
{
    if(currentTile.properties == nullptr)
        return -1;
    return currentTile.properties->id;
}

int Selector::getLayer()
{
    return layer;
}

void Selector::setLayer(int lay)
{
    layer = lay;
}

bool Selector::inDeleteMode()
{
    return deleteMode;
}

void Selector::toggleDeleteMode()
{
    deleteMode = !deleteMode;
    if(!deleteMode)
        currentTile.properties = nullptr;
}

void Selector::toggleSolid()
{
    solid = !solid;
}

bool Selector::isSolid()
{
    return solid;
}
