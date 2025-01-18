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
#include "PremadeSystems/Update/ParalaxSystem.hpp"
#include "PremadeSystems/Update/MovementSystem.hpp"
#include "PremadeSystems/Update/AnimateSystem.hpp"

// Game specific
#include "SharedSystems/RemoveOutOfBoundProjectilesSystem.hpp"
#include "SharedSystems/BugsMovementSystem.hpp"

#include <chrono>
#include <memory>

class Level1 final : public engine::AScene
{
public:
    explicit Level1(const std::string& n) : AScene(n) {
        this->_updateSystems.push_back(std::make_unique<ParallaxSystem>());
        this->_updateSystems.push_back(std::make_unique<MovementSystem>());
        this->_updateSystems.push_back(std::make_unique<AnimateSystem>());
        this->_updateSystems.push_back(std::make_unique<RemoveOutOfBoundProjectilesSystem>());
        this->_updateSystems.push_back(std::make_unique<BugsMovementSystem>());
    }

    void initialize() override;

    void update(const double& deltaTime) override;

    void onEnter() override;

    void onEnter(const AScene& lastScene) override;

    void onExit() override;

    void onExit(const AScene& nextScene) override;

  private:
    void addProjectile(const Transform& transform);
    void addBug(const Transform& transform);

public:
private:
};

#endif // GAME_HPP
