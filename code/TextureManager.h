//
//  TextureManager.h
//  Ch6-Animate
//
//  Created by Austin Baird on 11/28/16.
//  Copyright © 2016 Austin Baird. All rights reserved.
//

#ifndef TextureManager_h
#define TextureManager_h

#include "ResourceManager.h"

class TextureManager : public ResourceManager<TextureManager, sf::Texture>
{
public:
    
    TextureManager() : ResourceManager("Textures.cfg.txt") {};
    
    sf::Texture* load(const std::string& path)
    {
        sf::Texture* texture = new sf::Texture();
        if(!texture->loadFromFile(/*resourcePath() +*/ path))
        {
            delete texture;
            texture = nullptr;
            std::cerr << "!TEXTUREMANAGER Failed to load texture : " << path << std::endl;
        }
        return texture;
    }
    
};

#endif /* TextureManager_h */

