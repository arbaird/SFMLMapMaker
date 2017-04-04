//
//  Window.hpp
//  chapter2
//
//  Created by Austin Baird on 11/2/16.
//  Copyright © 2016 Austin Baird. All rights reserved.
//
//#pragma once
#ifndef Window_hpp
#define Window_hpp

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "EventManager.hpp"


class Window
{
private:
    sf::RenderWindow window;
    sf::Vector2u windowSize;
    std::string windowTitle;
    bool wIsDone;
    bool wIsFullScreen;
    bool wIsFocused;
    EventManager eventManager;
    
    sf::Image icon;
    
    int focusNum;
public:
    
    Window();
    Window(const std::string& wTitle, const sf::Vector2u& wSize);
    ~Window();
    
    void beginDraw(); //clear the window
    void endDraw(); //display the changes
    
    void update();
    
    bool isFocused();
    bool isDone() {return wIsDone;}
    bool isFullScreen() {return wIsFullScreen;}
    sf::Vector2u getWindowSize() {return windowSize;}
    
    void toggleFullScreen(EventDetails* details);
    void close(EventDetails* details = nullptr);
    
    void draw(sf::Drawable& l_drawable) {window.draw(l_drawable);}
    
    void setPosition(const sf::Vector2i&);
    EventManager* getEventManager();
    sf::RenderWindow* getRenderWindow();
    
    sf::FloatRect getViewSpace();
    
    void respondToClick(EventDetails* details);
    
private:
    void Setup(const std::string title, const sf::Vector2u& size);
    void Destroy();
    void Create();
    
    
    
    
    
};

#endif /* Window_hpp */
