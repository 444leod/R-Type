/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** PacketHandler.hpp
*/

#ifndef PACKET_HANDLER_HPP
#define PACKET_HANDLER_HPP

#include "../Components.hpp"
#include "ecs/EventDispatcher.hpp"
#include "ecs/Registry.hpp"
#include "network/UDPPacket.hpp"
#include "network/NetworkAgent.hpp"
#include "engine/RestrictedSceneManager.hpp"
#include "BaseComponents.hpp"
#include "PacketInformation.hpp"
#include "../Systems/ShipMovementSystem.hpp"
#include "../Systems/NewProjectileSystem.hpp"
#include "../Systems/MonsterKilledSystem.hpp"
#include "PacketTypes.hpp"

#include "Sprites/Level1.hpp"

class PacketHandler final : public ecs::EventHandler<PacketInformation>
{
public:
    explicit PacketHandler(ecs::Registry &registry, RestrictedSceneManager &manager) :
        _registry(registry),
        _manager(manager),
        _newProjectileSystem(_registry),
        _monsterKilledSystem(_registry)
    {
        _spaceshipTex.loadFromFile("assets/r-typesheet42.gif", sf::IntRect(0, 0, 34, 18));
        _projectileTex.loadFromFile("assets/r-typesheet1.gif", sf::IntRect(0, 91, 48, 16));
        _bugTex.loadFromFile("assets/r-typesheet8.gif");
        _explosionTex.loadFromFile("assets/r-typesheet44.gif", sf::IntRect(131, 0, 192, 32));
    }
    ~PacketHandler() override = default;

    void receive(const PacketInformation &event) override
    {
    }

private:
    Entity _generateSpaceship(const Transform &transform) const
    {
        const auto spaceship = _registry.create();

        _registry.addComponent(spaceship, spaceshipSprite);
        _registry.addComponent(spaceship, transform);
        _registry.addComponent(spaceship, Hitbox{});
        _registry.addComponent(spaceship, Velocity{.x = 0, .y = 0});
#if DEBUG
        _registry.addComponent(spaceship, Debug{});
#endif
        return spaceship;
    }

    ecs::Registry &_registry;
    RestrictedSceneManager &_manager;
    sf::Texture _spaceshipTex;
    sf::Texture _projectileTex;
    sf::Texture _bugTex;
    sf::Texture _explosionTex;

    NewProjectileSystem _newProjectileSystem;
    MonsterKilledSystem _monsterKilledSystem;
};

#endif // PACKET_HANDLER_HPP
