/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.hpp
*/

#ifndef LEVEL_1_HPP
#define LEVEL_1_HPP

#include <chrono>
#include <memory>

#include <Engine/AScene.hpp>

// From Game Engine
#include "PremadeSystems/Update/AnimateSystem.hpp"
#include "PremadeSystems/Update/MovementSystem.hpp"
#include "PremadeSystems/Update/ParalaxSystem.hpp"

// Game specific
#include "SharedSystems/BugsMovementSystem.hpp"
#include "SharedSystems/RemoveOutOfBoundProjectilesSystem.hpp"

#include <PremadeSystems/Update/CollisionSystem.hpp>
#include <chrono>
#include <memory>

class Level1 final : public engine::AScene
{
  public:
    explicit Level1(const std::string& n) : AScene(n)
    {
        this->_updateSystems.push_back(std::make_unique<ParallaxSystem>());
        this->_updateSystems.push_back(std::make_unique<MovementSystem>());
        this->_updateSystems.push_back(std::make_unique<AnimateSystem>());
        this->_updateSystems.push_back(std::make_unique<RemoveOutOfBoundProjectilesSystem>());
        this->_updateSystems.push_back(std::make_unique<BugsMovementSystem>());
        this->_updateSystems.push_back(std::make_unique<CollisionSystem>());
    }

    void initialize() override;

    void update(const double& deltaTime) override;

    void onEnter() override;

    void onEnter(const AScene& lastScene) override;

    void onExit() override;

    void onExit(const AScene& nextScene) override;

  private:
    void addBug(const Transform& transform);

  public:
  private:
    std::shared_ptr<sf::Clock> _spaceClock;
    double _lastShotTime{0.0};
};

#endif // GAME_HPP
