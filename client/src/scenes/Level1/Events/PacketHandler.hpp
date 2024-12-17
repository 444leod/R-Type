/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** PacketHandler.hpp
*/

#ifndef PACKETHANDLER_HPP
#define PACKETHANDLER_HPP

#include "../Components.hpp"
#include "EventDispatcher.hpp"
#include "Registry.hpp"
#include "UDPPacket.hpp"
#include "NetworkAgent.hpp"
#include "config.h"
#include "ISceneManager.hpp"

struct PacketInformations : public IEvent
{
    PACKET_TYPE type;
    UDPPacket &packet;
};

class PacketHandler : public EventHandler<PacketInformations>
{
public:
    explicit PacketHandler(Registry &registry, ISceneManager &manager) : _registry(registry), _manager(manager)
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
        case PACKET_TYPE::YOUR_SHIP:
        {
            std::uint32_t id;
            event.packet >> id;
            const auto self_spaceship = this->_generateSpaceship(Transform{.x = 100, .y = 100, .z = 1, .rotation = 0});
            _registry.addComponent(self_spaceship, Self{});
            _registry.addComponent(self_spaceship, Ship{.id = id});
            return;
        }
        case PACKET_TYPE::NEW_SHIP:
        {
            std::uint32_t id;
            event.packet >> id;
            const auto ally_spaceship = this->_generateSpaceship(Transform{.x = 100, .y = 100, .z = 1, .rotation = 0});
            _registry.addComponent(ally_spaceship, Ship{.id = id});
            return;
        }
        case PACKET_TYPE::DISCONNECT:
        {
            _manager.stop();
            return;
        }
        case PACKET_TYPE::SHIP_MOVEMENT:
        {
            std::uint32_t id;
            Velocity velocity{};
            Transform position{};
            event.packet >> id >> velocity >> position;
            std::cout << "Received movement packet for " << id << ": " << velocity.x << ", " << velocity.y << std::endl;
            for (auto &[entity, ship, vel, pos] : _registry.view<Ship, Velocity, Transform>().each())
            {
                if (ship.id != id)
                    continue;
                vel = velocity;
                pos = position;
                break;
            }
            return;
        }
        case PACKET_TYPE::NEW_PROJECTILE:
        {
            std::uint32_t projectileId;
            std::uint32_t shipId;
            event.packet >> shipId >> projectileId;
            std::cout << "New projecitle emited by: " << shipId << std::endl;
            for (auto [entity, ship, transform] : _registry.view<Ship, Transform>().each())
            {
                if (ship.id != shipId)
                    continue;
                const auto projectile = _registry.create();
                auto projectileSprite = sf::Sprite(_projectileTex);
                projectileSprite.setOrigin(0, 0);
                projectileSprite.setScale(SCALE, SCALE);
                projectileSprite.setPosition(transform.x, transform.y);
                projectileSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
                _registry.addComponent(projectile, Hitbox{});
                _registry.addComponent(projectile, projectileSprite);
                _registry.addComponent(projectile, Transform{.x = transform.x + 33 * SCALE, .y = transform.y + 2 * SCALE, .z = 1, .rotation = 0});
                _registry.addComponent(projectile, Projectile{ .id = projectileId });
                _registry.addComponent(projectile, Animation{.frameSize = {16, 16}, .speed = 20, .frameCount = 3, .loop = false, .velocity = Velocity{.x = 200, .y = 0}});
            }
            return;
        }
        case PACKET_TYPE::NEW_MONSTER:
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
        case PACKET_TYPE::MONSTER_KILLED:
        {
            uint32_t monster_id;
            uint32_t projectile_id;
            Transform transform;
            event.packet >> monster_id >> projectile_id;

            _registry.view<Enemy, sf::Sprite, Transform>().each([&](const Entity& enemy, const Enemy&e_enemy, const sf::Sprite& sprite, const Transform& transform)  {
                _registry.view<Projectile, Transform>().each([&](const Entity& projectile, const Projectile&p_projectile, const Transform& projectile_transform) {
                    if (e_enemy.id != monster_id || p_projectile.id != projectile_id)
                        return;
                    const auto explosion = _registry.create();
                    auto explosionSprite = sf::Sprite(_explosionTex);
                    explosionSprite.setOrigin(16, 16);
                    explosionSprite.setScale(SCALE, SCALE);
                    explosionSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
                    explosionSprite.setPosition(transform.x, transform.y);
                    _registry.addComponent(explosion, explosionSprite);
                    _registry.addComponent(explosion, Transform{.x = transform.x, .y = transform.y, .z = 1, .rotation = 0});
                    _registry.addComponent(explosion, Animation{.frameSize = {32, 32}, .speed = 100, .frameCount = 6, .loop = false});
                    _registry.remove(enemy);
                    _registry.remove(projectile);
                });
            });

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

    Registry &_registry;
    ISceneManager &_manager;
    sf::Texture _spaceshipTex;
    sf::Texture _projectileTex;
    sf::Texture _bugTex;
    sf::Texture _explosionTex;
};

#endif // PACKETHANDLER_HPP
