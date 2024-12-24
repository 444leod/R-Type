/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** PacketHandler.hpp
*/

#ifndef PACKETHANDLER_HPP
#define PACKETHANDLER_HPP

#include "../Components.hpp"
#include "ecs/EventDispatcher.hpp"
#include "ecs/Registry.hpp"
#include "network/UDPPacket.hpp"
#include "network/NetworkAgent.hpp"
#include "config.h"
#include "engine/ISceneManager.hpp"
#include "BaseComponents.hpp"
#include "PacketInformations.hpp"
#include "../Systems/ShipMovementSystem.hpp"
#include "../Systems/NewProjectileSystem.hpp"
#include "../Systems/MonsterKilledSystem.hpp"

class PacketHandler : public ecs::EventHandler<PacketInformations>
{
public:
    explicit PacketHandler(ecs::Registry &registry, ISceneManager &manager) :
        _registry(registry),
        _manager(manager),
        _shipMovementSystem(_registry),
        _newProjectileSystem(_registry),
        _monsterKilledSystem(_registry)
    {
        _spaceshipTex.loadFromFile("assets/r-typesheet42.gif", sf::IntRect(0, 0, 34, 18));
        _projectileTex.loadFromFile("assets/r-typesheet1.gif", sf::IntRect(0, 91, 48, 16));
        _bugTex.loadFromFile("assets/r-typesheet8.gif");
        _explosionTex.loadFromFile("assets/r-typesheet44.gif", sf::IntRect(131, 0, 192, 32));
    }
    ~PacketHandler() override = default;

    void receive(const PacketInformations &event) override
    {
        switch (event.type)
        {
        case ntw::PACKET_TYPE::YOUR_SHIP:
        {
            std::uint32_t id;
            event.packet >> id;
            const auto self_spaceship = this->_generateSpaceship(Transform{.x = 100, .y = 100, .z = 1, .rotation = 0});
            _registry.addComponent(self_spaceship, Self{});
            _registry.addComponent(self_spaceship, Ship{.id = id});
            return;
        }
        case ntw::PACKET_TYPE::NEW_SHIP:
        {
            std::uint32_t id;
            event.packet >> id;
            const auto ally_spaceship = this->_generateSpaceship(Transform{.x = 100, .y = 100, .z = 1, .rotation = 0});
            _registry.addComponent(ally_spaceship, Ship{.id = id});
            return;
        }
        case ntw::PACKET_TYPE::DISCONNECT:
        {
            _manager.stop();
            return;
        }
        case ntw::PACKET_TYPE::SHIP_MOVEMENT:
        {
            _shipMovementSystem.execute(event);
            return;
        }
        case ntw::PACKET_TYPE::NEW_PROJECTILE:
        {
            _newProjectileSystem.execute(event, _projectileTex);
            return;
        }
        case ntw::PACKET_TYPE::NEW_MONSTER:
        {
            uint32_t id;
            Transform transform{};
            event.packet >> id >> transform;
            const auto bug = _registry.create();
            auto bugSprite = sf::Sprite(_bugTex);
            bugSprite.setOrigin(16, 13);
            bugSprite.setScale(SCALE, SCALE);
            bugSprite.setPosition(transform.x, transform.y);
            _registry.addComponent(bug, bugSprite);
            _registry.addComponent(bug, Bug{});
            _registry.addComponent(bug, Enemy{ .id = id });
            _registry.addComponent(bug, transform);
            _registry.addComponent(bug, Velocity{.x = -100, .y = 0});
            _registry.addComponent(bug, Hitbox{});
            return;
        }
        case ntw::PACKET_TYPE::MONSTER_KILLED:
        {
            _monsterKilledSystem.execute(event, _explosionTex);
            return;
        }
        default:
            break;
        }
    }

private:
    Entity _generateSpaceship(const Transform &transform) const
    {
        const auto spaceship = _registry.create();

        auto spaceshipSprite = sf::Sprite(_spaceshipTex);
        spaceshipSprite.setOrigin(0, 0);
        spaceshipSprite.setScale(SCALE, SCALE);
        spaceshipSprite.setPosition(transform.x, transform.y);

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
    ISceneManager &_manager;
    sf::Texture _spaceshipTex;
    sf::Texture _projectileTex;
    sf::Texture _bugTex;
    sf::Texture _explosionTex;

    ShipMovementSystem _shipMovementSystem;
    NewProjectileSystem _newProjectileSystem;
    MonsterKilledSystem _monsterKilledSystem;
};

#endif // PACKETHANDLER_HPP
