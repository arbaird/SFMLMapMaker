//
//  ResourceManager.hpp
//  Ch6-Animate
//
//  Created by Austin Baird on 11/27/16.
//  Copyright © 2016 Austin Baird. All rights reserved.
//

//implementations are all in header file because when using templates, this is required so the complier can have access to the implementation to instantiate the methods with the template arguments
#ifndef ResourceManager_h
#define ResourceManager_h

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include <unordered_map>
#include "ResourcePath.hpp"

template<typename Derived, typename T> //derived is used for polymorphic calls, extendeding this class's use to be beyond textures, T is actual type of resource, i.e Texture

class ResourceManager
{
private:
    
    
    /* struct THash
     {
     //template <typename T> //replace StateType with T for generics
     std::size_t operator()(std::string t) const
     {
     return static_cast<T>(t);
     }
     };*/
    
    std::pair<T*, unsigned int>* find(const std::string& id)
    {
        
        auto itr = resources.find(id);
        if(itr != resources.end())
            return &itr->second;
        else
            return nullptr;
    }
    std::unordered_map<std::string, std::pair<T*, unsigned int>> resources;
    //string is identifier, int is count of places this T resource is being used.
    std::unordered_map<std::string, std::string> paths;
    //first string is pathname, second is actual path
    bool unload(const std::string id)
    {
        auto itr = resources.find(id);
        if(itr == resources.end())
            return false;
        delete itr->second.first; //delete actual T resource
        std::cout << id << " has been deleted" << std::endl;
        resources.erase(itr);
        return true;
    }
    
    void loadPaths(const std::string& pathFile)
    {
        std::ifstream pathReader;
        pathReader.open(resourcePath() + pathFile);
        if(pathReader.is_open())
        {
            std::string line;
            while(std::getline(pathReader, line))
            {
                std::stringstream keystream(line);
                std::string pathName;
                std::string fileName;
                keystream >> pathName;
                keystream >> fileName;
                std::string path = resourcePath() + fileName;
                paths.emplace(pathName, path);
            }
            pathReader.close();
            return;
        }
        else
            std::cerr << "! Failed loading the path file: " << pathFile << std::endl;
    }
    
    
    
public:
    int getSize()
    {
        return resources.size();
    }
    
    ResourceManager(const std::string& pathsFile)
    {
        loadPaths(pathsFile);
    }
    
    virtual ~ResourceManager()
    {
        std::cout << "~ResourceManager destructor called!" << std::endl;
        purgeResources();
    }
    
    T* getResource(const std::string& id)
    {
        auto resource = find(id);
        if(resource)
        {
            //std::cout << "ResourceMgr->getResource() worked for " << id << std::endl;
            return resource->first; //returns pointer to pair of elements of the map
        }
        else
            return nullptr;
    }
    
    std::string getPath(std::string& id)
    {
        auto path = paths.find(id);
        if (path != paths.end())
            return path->second;
        else
            return "";
    }
    
    //guarantee that resources don't get deallocated while its still being used
    bool requireResource(const std::string& id)
    {
        
        auto res = find(id);
        if(res)
        {
            ++res->second;
            
            return true; //id is already being used somewhere, add to its count and return
        }
        
        auto path = paths.find(id);
        if (path == paths.end())
        {
            
            return false;
        }
        T* resource = load(path->second);
        if(!resource)
        {
            
            return false; //if nullptr, return false
        }
        resources.emplace(id, std::make_pair(resource, 1));//add to map, count = 1
        std::cout << "Added " << id << " to resources" << std::endl;
        return true;
    }
    
    bool releaseResource(const std::string& id)
    {
        //if(!resources.empty())
        //  std::cout << "resources is NOT empty, should be able to find(): " << id << std::endl;
        auto resource = find(id);
        if(!resource)
            return false;
        --resource->second;
        if(!resource->second)
            unload(id);//if count for this resource is now 0, call unload to deallocate memory for this resource
        std::cout << "Released " << id << " from resources" << std::endl;
        return true;
    }
    
    void purgeResources()
    {
        std::cout << "Purging resources" << std::endl;
        while(resources.begin() != resources.end())
        {
            std::cout << "Removing: " << resources.begin()->first << std::endl;
            delete resources.begin()->second.first;//delete T resource
            resources.erase(resources.begin());
        }
    }
    
protected:
    T* load(const std::string& path)
    {
        return static_cast<Derived*>(this)->load(path); //curiously recurring template pattern
    }
    
};
#endif /* ResourceManager_h */

