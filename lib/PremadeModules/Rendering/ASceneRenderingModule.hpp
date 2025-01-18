/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ASceneRenderingModule
*/

#pragma once

#include "Engine/Modules/ASceneModule.hpp"

#include "PremadeSystems/Abstracts/ARenderSystem.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>
#include <memory>
#include <vector>

class ASceneRenderingModule final : public engine::ASceneModule {
  public:
    struct PredicateHandler {
        std::function<bool(const sf::Event&)> predicate;
        std::function<void(sf::Event&)> handler;
    };

  public:
    explicit ASceneRenderingModule(engine::AScene& scene) : ASceneModule(scene) {}
    ~ASceneRenderingModule() override = default;

    void executeSystems(sf::RenderWindow& window) const noexcept {
        for (const auto& system : this->_systems)
            system->execute(window);
    }

    void trigger(sf::Event& event) {
        for (const auto& [predicate, handler] : this->_handlers)
            if (predicate(event))
                handler(event);
    }

    void addHandler(const std::function<bool(const sf::Event&)>& predicate, const std::function<void(sf::Event&)>& handler) { this->_handlers.push_back(PredicateHandler{predicate, handler}); }

    std::vector<std::unique_ptr<ARenderSystem>>& systems() noexcept { return this->_systems; }

  private:
    std::vector<PredicateHandler> _handlers;
    std::vector<std::unique_ptr<ARenderSystem>> _systems;
};
