/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.hpp
*/

#ifndef GAME_HPP
#define GAME_HPP

#include <chrono>
#include <memory>

#include <Engine/AScene.hpp>
#include <ECS/Registry.hpp>

// From Game Engine
#include "PremadeSystems/Update/MovementSystem.hpp"
#include "PremadeSystems/Update/AnimateSystem.hpp"

#include "PremadeSystems/Update/CollisionSystem.hpp"

#include "SharedSystems/RemoveOutOfBoundProjectilesSystem.hpp"

// Game specific
#include "Systems/AttackSystem.hpp"
#include "Systems/MonsterUpdateSystem.hpp"
#include "Systems/BorderCollisionSystem.hpp"
#include "Systems/DamageProtectionSystem.hpp"
#include "Systems/DeathUpdateSystem.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <set>

class Game final : public engine::AScene {
public:
    Game(const std::string& name) : AScene(name)
    {
        this->_updateSystems.push_back(std::make_unique<MovementSystem>());
        this->_updateSystems.push_back(std::make_unique<RemoveOutOfBoundProjectilesSystem>());
        this->_updateSystems.push_back(std::make_unique<AnimateSystem>());
        this->_updateSystems.push_back(std::make_unique<CollisionSystem>());
        this->_updateSystems.push_back(std::make_unique<AttackSystem>());
        this->_updateSystems.push_back(std::make_unique<MonsterUpdateSystem>());
        this->_updateSystems.push_back(std::make_unique<BorderCollisionSystem>());
        this->_updateSystems.push_back(std::make_unique<DamageProtectionSystem>());
        this->_updateSystems.push_back(std::make_unique<DeathUpdateSystem>());
    }

    void initialize() override;

    void update(const double& deltaTime) override;

    void onEnter() override;

    void onEnter(const AScene& lastScene) override;

    void onExit() override;

    void onExit(const AScene& nextScene) override;

private:


public:
private:
    std::set<sf::Keyboard::Key> _pressedKeys;
    ecs::Entity player;

};

#endif //GAME_HPP
