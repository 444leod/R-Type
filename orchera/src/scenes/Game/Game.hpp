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

#include "engine/AScene.hpp"
#include "ecs/Registry.hpp"

// From Game Engine
#include "BaseSystems/Update/MovementSystem.hpp"
#include "BaseSystems/Update/AnimateSystem.hpp"

#include "BaseSystems/Update/CollisionSystem.hpp"

// Game specific
#include "Systems/RemoveOutOfBoundProjectilesSystem.hpp"
#include "Systems/AttackSystem.hpp"
#include "Systems/MonsterUpdateSystem.hpp"
#include "Systems/BorderCollisionSystem.hpp"
#include "Systems/DamageProtectionSystem.hpp"
#include "Systems/DeathUpdateSystem.hpp"

#include <set>

class Game final : public AScene {
public:
    Game(RestrictedSceneManager& m, ecs::Registry& r, const std::string& n) : AScene(m, r, n)
    {
        this->_updateSystems.push_back(std::make_unique<MovementSystem>(_registry));
        this->_updateSystems.push_back(std::make_unique<RemoveOutOfBoundProjectilesSystem>(_registry));
        this->_updateSystems.push_back(std::make_unique<AnimateSystem>(_registry));
        this->_updateSystems.push_back(std::make_unique<CollisionSystem>(_registry));
        this->_updateSystems.push_back(std::make_unique<AttackSystem>(_registry));
        this->_updateSystems.push_back(std::make_unique<MonsterUpdateSystem>(_registry));
        this->_updateSystems.push_back(std::make_unique<BorderCollisionSystem>(_registry));
        this->_updateSystems.push_back(std::make_unique<DamageProtectionSystem>(_registry));
        this->_updateSystems.push_back(std::make_unique<DeathUpdateSystem>(_registry));
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
    Entity player;

};

#endif //GAME_HPP
