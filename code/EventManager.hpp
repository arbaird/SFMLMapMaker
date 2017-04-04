//
//  EventManager.hpp
//  Chapter4 Event Manager
//
//  Created by Austin Baird on 11/7/16.
//  Copyright © 2016 Austin Baird. All rights reserved.
//
#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>



enum class EventType
{
    KeyDown = sf::Event::KeyPressed,
    KeyUp = sf::Event::KeyReleased,
    MouseButtonDown = sf::Event::MouseButtonPressed,
    MouseButtonUp =sf::Event::MouseButtonReleased,
    MouseWheelMovement = sf::Event::MouseWheelMoved,
    WindowResized = sf::Event::Resized,
    GainedFocus = sf::Event::GainedFocus,
    LostFocus = sf::Event::LostFocus,
    MouseEntered = sf::Event::MouseEntered,
    MouseLeft = sf::Event::MouseLeft,
    Closed = sf::Event::Closed,
    TextEntered = sf::Event::TextEntered,
    Keyboard = sf::Event::Count + 1, Mouse, Joystick
    
};

struct EventInfo
{
    EventInfo()
    {
        code = 0;
    }
    EventInfo(int event)
    {
        code = event;
    }
    union
    {
        int code;
    };
};

typedef std::vector<std::pair<EventType, EventInfo>> Events;

struct EventDetails
{
    EventDetails(const std::string& bindingName) : name(bindingName)
    {
        clear();
    }
    std::string name;
    
    sf::Vector2i size;
    sf::Uint32 textEntered;
    sf::Vector2i mouse;
    int mouseWheelDelta;
    int keyCode;
    
    void clear()
    {
        size = sf::Vector2i(0,0);
        textEntered = 0;
        mouse = sf::Vector2i(0,0);
        mouseWheelDelta = 0;
        keyCode = -1;
    }
    
};

struct Binding
{
    Binding(const std::string& bindName) : name(bindName), details(bindName), count(0)
    {
        
    }
    
    void bindEvent(EventType type, EventInfo info = EventInfo())
    {
        events.emplace_back(type, info);
    }
    
    Events events;
    std::string name;
    int count;  //count of events that are 'happening'
    
    EventDetails details;
};

//using Bindings = std::unordered_map<std::string, Binding*>;
typedef std::unordered_map<std::string, Binding*> Bindings;

enum class StateType;

struct StateTypeHash
{
    //template <typename T> //replace StateType with T for generics
    std::size_t operator()(StateType t) const
    {
        return static_cast<std::size_t>(t);
    }
};//unorderded map needs type that can hash without crashes, enums don't guarantee this so we need this thing. Needs to overide () operator.
// google 'C++ unordered_map using a custom class type as the key' for more info

typedef std::unordered_map<std::string, std::function<void(EventDetails*)>> CallbackContainer; //this used to just be Callbacks, but now with different states, we tie a state to the old callback definition

typedef std::unordered_map<StateType, CallbackContainer, StateTypeHash> Callbacks;



class EventManager
{
private:
    void loadBindings();
    
    Bindings bindings;
    Callbacks callbacks;
    bool hasFocus;
    StateType currentState;
    
public:
    EventManager();
    ~EventManager();
    
    bool addBinding(Binding* binding);
    bool removeBinding(std::string name);
    
    void setFocus(const bool& focus);
    
    template<class T> //template functions must be implemented in header file
    bool addCallback(StateType state, const std::string& name, void(T::*function)(EventDetails*), T* instance)
    {
        auto itr = callbacks.emplace(state, CallbackContainer()).first;
        auto temp = std::bind(function, instance, std::placeholders::_1);
        return itr->second.emplace(name, temp).second;
    }
    //implemented here for consisteny with addCallbacks
    bool removeCallback(StateType state, const std::string& name)
    {
        auto itr = callbacks.find(state);
        if (itr == callbacks.end())
            return false;
        auto itr2 = itr->second.find(name); //itr->second is callback container, string function pair vector
        if (itr2 == itr->second.end())
            return false;
        itr->second.erase(name);
    }
    
    void handleEvent(sf::Event& event);
    void update();
    
    sf::Vector2i getMousePos(sf::RenderWindow* window = nullptr);
    
    void setCurrentState(StateType newState);
    
};
//#endif /* EventManager_hpp */

