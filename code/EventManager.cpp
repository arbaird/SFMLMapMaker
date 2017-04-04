//
//  EventManager.cpp
//  Chapter4 Event Manager
//
//  Created by Austin Baird on 11/7/16.
//  Copyright © 2016 Austin Baird. All rights reserved.
//

#include "EventManager.hpp"
#include "ResourcePath.hpp"
#include <fstream>
#include <sstream>
#include <iostream>


EventManager::EventManager() : hasFocus(true)
{
    loadBindings();
    //std::cout << "EventType::Keyboard is value: " << (int)EventType::Keyboard;
    //std::cout << "\nMouseButtonDown is : " << (int)EventType::MouseButtonDown;
}

EventManager::~EventManager()
{
    for (auto &itr : bindings)
    {
        delete itr.second;
        itr.second = nullptr;
    }
}

bool EventManager::addBinding(Binding* binding)
{
    
    if (binding == NULL)
    {
        return false;
        std::cout << "Binding == NULL" << std::endl;
    }
    if(bindings.find(binding->name) != bindings.end())
    {
        return false; //if bindings container alreader has a binding with the same name, don't add to it
        std::cout << "Binding already in bindings" << std::endl;
    }
    
    return bindings.emplace(binding->name, binding).second;
}

bool EventManager::removeBinding(std::string name)
{
    auto itr = bindings.find(name);
    if(itr == bindings.end())
        return false; //if name wasn't found, return false
    delete itr->second;
    bindings.erase(itr);
    return true;
}

void EventManager::handleEvent(sf::Event& event)
{
    for (auto &bind_itr : bindings)
    {
        Binding* bind = bind_itr.second;
        for(auto &event_itr : bind->events)
        {
            EventType sfmlEvent = (EventType)event.type;
            if(event_itr.first != sfmlEvent)
                continue;
            if(sfmlEvent == EventType::KeyDown || sfmlEvent == EventType::KeyUp)
            {
                if(event_itr.second.code == event.key.code)
                {
                    //matching event/keystroke, increase count
                    if(bind->details.keyCode != -1)
                        bind->details.keyCode = event_itr.second.code;
                    ++(bind->count);
                    break;
                }
            }
            
            else if(sfmlEvent == EventType::MouseButtonDown || sfmlEvent == EventType::MouseButtonUp)
            {
                if(event_itr.second.code == event.mouseButton.button)
                {
                    //matching event/keystroke, increase count
                    bind->details.mouse.x = event.mouseButton.x;
                    bind->details.mouse.y = event.mouseButton.y;
                    if(bind->details.keyCode != -1)
                        bind->details.keyCode = event_itr.second.code;
                    ++(bind->count);
                    break;
                }
            }
            else
            {
                if(sfmlEvent == EventType::MouseWheelMovement)
                    bind->details.mouseWheelDelta = event.mouseWheel.delta;
                else if(sfmlEvent == EventType::WindowResized)
                {
                    bind->details.size.x = event.size.width;
                    bind->details.size.y = event.size.height;
                }
                else if(sfmlEvent == EventType::TextEntered)
                    bind->details.textEntered = event.text.unicode;
                ++(bind->count);
            }
        }
    }
}

void EventManager::update()
{
    if(!hasFocus)
        return;
    for(auto &bind_itr : bindings)
    {
        Binding* bind = bind_itr.second;
        for(auto &event_itr : bind->events)
        {
            switch(event_itr.first)
            {
                case(EventType::Keyboard) :
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(event_itr.second.code)))
                    {
                        
                        if(bind->details.keyCode != -1)
                            bind->details.keyCode = event_itr.second.code;
                        ++(bind->count);
                    }
                    break;
                    
                case(EventType::Mouse) :
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Button(event_itr.second.code)))
                    {
                        if(bind->details.keyCode != -1)
                            bind->details.keyCode = event_itr.second.code;
                        ++(bind->count);
                    }
                    break;
                    
                case(EventType::Joystick) :
                    //add things later
                    break;
            }
            
        }
        
        if(bind->events.size() == bind->count)
        {
            //auto stateCallbacks = callbacks.find(currentState);
            auto otherCallbacks = callbacks.find(StateType(0));
            //statetypes start at 1, so otherCallbacks is used for global callbacks for Window class and
            //others. These are callbacks that get invoked regardless of current state
            /*if (stateCallbacks != callbacks.end())
            {
                auto callItr = stateCallbacks->second.find(bind->name);
                if (callItr != stateCallbacks->second.end())
                    callItr->second(&bind->details); //pass in info about events
            }*/
            
            if (otherCallbacks != callbacks.end())
            {
                //std::cout << "Size of callbacks: " << callbacks.size() << std::endl;
                auto callItr = otherCallbacks->second.find(bind->name);
                if (callItr != otherCallbacks->second.end())
                    callItr->second(&bind->details); //pass in info about events
            }
        }
        bind->count = 0;
        bind->details.clear();
    }
}


sf::Vector2i EventManager::getMousePos(sf::RenderWindow* window)
{
    if(window)
        return sf::Mouse::getPosition(*window);
    else
        return sf::Mouse::getPosition();
    //return either position relative to window if window passed as argument or relative to whole screen if not. See its declaration in header file
}

void EventManager::loadBindings()
{
    std::string delimiter = ":";
    
    std::ifstream bindings;
    bindings.open(resourcePath() + "mapMakerKeys.txt");
    if(!bindings.is_open())
    {
        std::cout << "! Failed to load keys.cfg." << std::endl;
        return;
    }
    std::string line;
    while(std::getline(bindings, line))
    {
        std::stringstream keystream(line);
        std::string callbackName;
        keystream >> callbackName;
        Binding* bind = new Binding(callbackName);
        while (!keystream.eof())
        {
            std::string keyval;
            keystream >> keyval;
            int start = 0;
            int end = keyval.find(delimiter);
            if (end == std::string::npos)
            {
                delete bind;
                bind = nullptr;
                break;
            }
            EventType type = EventType(stoi(keyval.substr(start, end - start)));
            int code = stoi(keyval.substr(end + delimiter.length(),
                                          keyval.find(delimiter, end + delimiter.length())));
            EventInfo eventInfo;
            eventInfo.code = code;
            
            bind->bindEvent(type, eventInfo);
            
        }
        
        if (!addBinding(bind))
            delete bind;
        bind = nullptr;
    }
    
    bindings.close();
    
}

void EventManager::setFocus(const bool& focus)
{
    hasFocus = focus;
}

void EventManager::setCurrentState(StateType newState)
{
    currentState = newState;
}

