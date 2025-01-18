/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.hpp
*/

#ifndef LEVEL_1_HPP
#define LEVEL_1_HPP

#include <Engine/AScene.hpp>

#include "PremadeSystems/Update/AnimateSystem.hpp"
#include "PremadeSystems/Update/CollisionSystem.hpp"
#include "PremadeSystems/Update/MovementSystem.hpp"
#include "PremadeSystems/Update/ParalaxSystem.hpp"

#include "SharedSystems/BugsMovementSystem.hpp"
#include "SharedSystems/RemoveOutOfBoundProjectilesSystem.hpp"

#include "Sprites/Level1.hpp"

#include <chrono>

class Level1 final : public engine::AScene {
  public:
    explicit Level1(const std::string& name) : AScene(name) {
        this->_updateSystems.push_back(std::make_unique<ParallaxSystem>());
        this->_updateSystems.push_back(std::make_unique<MovementSystem>());
        this->_updateSystems.push_back(std::make_unique<RemoveOutOfBoundProjectilesSystem>());
        this->_updateSystems.push_back(std::make_unique<AnimateSystem>());
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
    std::uint32_t _enemyId = 0;
};

#endif // GAME_HPP
