/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SceneRenderingModule
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include <vector>
#include "ISceneModule.hpp"

class SceneRenderingModule final : public ISceneModule
{
public:
    SceneRenderingModule() = default;
    ~SceneRenderingModule() = default;

    void registerEvent(sf::Event event)
    {
        this->_events.push_back(event);
    }

    bool pollEvent(sf::Event &event)
    {
        if (this->_events.empty())
            return false;
        event = this->_events.back();
        this->_events.pop_back();
        return true;
    }

private:
    std::vector<sf::Event> _events;
};
