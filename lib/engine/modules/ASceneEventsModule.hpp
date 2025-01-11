/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ASceneEventsModule
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include <vector>
#include <functional>
#include "ASceneModule.hpp"

class ASceneEventsModule : public ASceneModule
{
public:
    struct PredicateHandler {
        std::function<bool(sf::Event &)> predicate;
        std::function<void(sf::Event &)> handler;
    };

public:
    ASceneEventsModule(AScene& scene): ASceneModule(scene) {}
    ~ASceneEventsModule() override = default;

    virtual void trigger(sf::Event &event)
    {
        for (auto [predicate, handler]: this->_handlers)
            if (predicate(event))
                handler(event);
    }

    virtual void addHandler(std::function<bool(sf::Event &)> predicate, std::function<void(sf::Event &)> handler)
    {
        this->_handlers.push_back(PredicateHandler { predicate, handler });
    }

private:
    std::vector<PredicateHandler> _handlers;
};
