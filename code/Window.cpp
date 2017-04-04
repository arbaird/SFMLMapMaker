//
//  Window.cpp
//  chapter2
//
//  Created by Austin Baird on 11/2/16.
//  Copyright © 2016 Austin Baird. All rights reserved.
//

#include "Window.hpp"
#include "ResourcePath.hpp"

Window::Window()
{
    Setup("Window", sf::Vector2u(640, 480));
}

Window::Window(const std::string& wTitle, const sf::Vector2u& wSize)
{
    Setup(wTitle, wSize);
}

Window::~Window()
{
    Destroy();
}

void Window::Setup(const std::string title, const sf::Vector2u& size)
{
    
    wIsFocused = true;
    eventManager.addCallback(StateType(0), "Window_close", &Window::close, this);
    eventManager.addCallback(StateType(0), "Fullscreen_toggle", &Window::toggleFullScreen, this);
    
    windowTitle = title;
    windowSize = size;
    wIsFullScreen = false;
    wIsDone = false;
    
    Create();
}

void Window::Destroy()
{
    window.close();
}

void Window::Create()
{
    auto style = sf::Style::Default;
    if(wIsFullScreen)
        style = sf::Style::Fullscreen;
    sf::VideoMode mode(windowSize.x, windowSize.y, 32);
    window.create(mode, windowTitle, style);
    icon.loadFromFile(resourcePath() + "Mushroom.png");
    
    //window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    //window.create({windowSize.x, windowSize.y, 32}, windowTitle, style) would do the same with uniform initialization
    
}

void Window::update()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        //std::cout << event.type << std::endl;
        if(event.type == sf::Event::Closed)
           wIsDone = true;
        if(event.type == sf::Event::LostFocus)
        {
            wIsFocused = false;
            eventManager.setFocus(false);
        }
        else if(event.type == sf::Event::GainedFocus)
        {
            wIsFocused = true;
            eventManager.setFocus(true);
        }
        
        eventManager.handleEvent(event);
    }
    eventManager.update();
}

void Window::toggleFullScreen(EventDetails* details)
{
    wIsFullScreen = !wIsFullScreen;
    if(wIsFullScreen)
    {
        windowSize.x = sf::VideoMode::getDesktopMode().width;
        windowSize.y = sf::VideoMode::getDesktopMode().height;
    }
    else
    {
        windowSize.x = window.getSize().x;
        windowSize.x = window.getSize().y;
    }
    Destroy();
    Create();
}

void Window::beginDraw()
{
    window.clear();
}

void Window::endDraw()
{
    window.display();
}

void Window::close(EventDetails* details)
{
    wIsDone = true;
}

EventManager* Window::getEventManager()
{
    return &eventManager;
}
sf::RenderWindow* Window::getRenderWindow()
{
    return &window;
}

sf::FloatRect Window::getViewSpace()
{
    sf::Vector2f viewCenter = window.getView().getCenter();
    sf::Vector2f viewSize = window.getView().getSize();
    sf::Vector2f viewSizeHalf(viewSize.x / 2, viewSize.y / 2);
    sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);
    return viewSpace;
}

void Window::setPosition(const sf::Vector2i& pos)
{
    window.setPosition(pos);
}

bool Window::isFocused()
{
    return wIsFocused;
}

