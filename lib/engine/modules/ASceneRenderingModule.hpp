/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ASceneRenderingModule
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include <vector>
#include <functional>
#include "ASceneModule.hpp"

class ASceneRenderingModule final : public ASceneModule
{
public:
    struct PredicateHandler {
        std::function<bool(sf::Event &)> predicate;
        std::function<void(sf::Event &)> handler;
    };

public:
    explicit ASceneRenderingModule(AScene& scene): ASceneModule(scene) {}
    ~ASceneRenderingModule() override = default;

    void executeSystems(sf::RenderWindow& window) const noexcept
    {
        for (const auto& system : this->_systems)
            system->execute(window);
    }

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

    std::vector<std::unique_ptr<ARenderSystem>>& systems() noexcept { return this->_systems; }

private:
    std::vector<PredicateHandler> _handlers;
    std::vector<std::unique_ptr<ARenderSystem>> _systems;
};
