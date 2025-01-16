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

class ASceneEventsModule final : public ASceneModule
{
public:
    struct PredicateHandler {
        std::function<bool(sf::Event &)> predicate;
        std::function<void(sf::Event &)> handler;
    };

public:
    explicit ASceneEventsModule(AScene& scene): ASceneModule(scene) {}
    ~ASceneEventsModule() override = default;

    void trigger(sf::Event &event)
    {
        for (const auto& [predicate, handler]: this->_handlers)
            if (predicate(event))
                handler(event);
    }

    void addHandler(const std::function<bool(sf::Event &)>& predicate, const std::function<void(sf::Event &)>& handler)
    {
        this->_handlers.push_back(PredicateHandler { predicate, handler });
    }

private:
    std::vector<PredicateHandler> _handlers;
};
