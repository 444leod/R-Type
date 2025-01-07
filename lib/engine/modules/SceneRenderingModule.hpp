/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SceneRenderingModule
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include <vector>
#include "ASceneModule.hpp"

class SceneRenderingModule final : public ASceneModule
{
public:
    explicit SceneRenderingModule(AScene& scene) : ASceneModule(scene) {}
    ~SceneRenderingModule() override = default;

    void registerEvent(const sf::Event& event)
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
