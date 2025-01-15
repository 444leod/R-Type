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

#include "engine/AScene.hpp"
#include "ecs/Registry.hpp"
#include "ecs/EventDispatcher.hpp"

// From Game Engine
#include "BaseSystems/Update/ParalaxSystem.hpp"
#include "BaseSystems/Update/MovementSystem.hpp"
#include "BaseSystems/Update/AnimateSystem.hpp"
#include "BaseSystems/Render/DrawSpritesSystem.hpp"

// Game specific
#include "Systems/RemoveOutOfBoundProjectilesSystem.hpp"
#include "Systems/BugsMovementSystem.hpp"

#include <chrono>
#include <memory>

#include "Structures/UserInput.hpp"

class Level1 final : public AScene {
public:
    Level1(RestrictedSceneManager& m, ecs::Registry& r, const std::string& n) : AScene(m, r, n) {
        this->_updateSystems.push_back(std::make_unique<ParallaxSystem>(_registry));
        this->_updateSystems.push_back(std::make_unique<MovementSystem>(_registry));
        this->_updateSystems.push_back(std::make_unique<RemoveOutOfBoundProjectilesSystem>(_registry));
        this->_updateSystems.push_back(std::make_unique<AnimateSystem>(_registry));
        this->_updateSystems.push_back(std::make_unique<BugsMovementSystem>(_registry));
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

#endif //GAME_HPP
