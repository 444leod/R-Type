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
#include "Global.hpp"
#include "ISceneManager.hpp"
#include <cstdint>
#include <config.h>

struct PacketInformations : public IEvent
{
    PACKET_TYPE type;
    UDPPacket &packet;
    ClientInformations &source;
};

class PacketHandler final : public EventHandler<PacketInformations>
{
public:
    explicit PacketHandler(Registry &registry, ISceneManager &manager) : _registry(registry), _manager(manager)
    {
        _projectileTex.loadFromFile("assets/r-typesheet1.gif", sf::IntRect(0, 91, 48, 16));
    }

    ~PacketHandler() override = default;

    void receive(const PacketInformations &event) override
    {
        switch (event.type)
        {
        case PACKET_TYPE::USER_INPUT:
        {
            UserInput input{};
            event.packet >> input;

            if (input.key >= sf::Keyboard::Left && input.key <= sf::Keyboard::Down)
            {
                for (auto [ship_id, ship, vel, pos] : _registry.view<Ship, Velocity, Transform>())
                {
                    if (ship.id != event.source.id)
                        continue;

                    switch (input.key)
                    {
                    case sf::Keyboard::Key::Up:
                        vel.y += input.pressed ? -75 : 75;
                        break;
                    case sf::Keyboard::Key::Down:
                        vel.y += input.pressed ? 75 : -75;
                        break;
                    case sf::Keyboard::Key::Left:
                        vel.x += input.pressed ? -75 : 75;
                        break;
                    case sf::Keyboard::Key::Right:
                        vel.x += input.pressed ? 75 : -75;
                        break;
                    default:
                        break;
                    }

                    UDPPacket packet;
                    packet << PACKET_TYPE::SHIP_MOVEMENT << event.source.id << vel << pos;
                    for (const auto &client : CLIENTS)
                    {
                        _manager.send(client.endpoint, packet);
                    }
                }
            }
            else if (input.key == sf::Keyboard::Space)
            {
                for (auto [entity, ship, transform] : _registry.view<Ship, Transform>())
                {
                    if (ship.id != event.source.id)
                        continue;

                    static uint32_t projectileId = 0;
                    const auto projectile = _registry.create();
                    auto projectileSprite = sf::Sprite(_projectileTex);
                    const auto shootTransform = Transform{.x = transform.x + 33 * SCALE, .y = transform.y + 2 * SCALE, .z = 1, .rotation = 0};
                    projectileSprite.setOrigin(0, 0);
                    projectileSprite.setScale(SCALE, SCALE);
                    projectileSprite.setPosition(shootTransform.x, shootTransform.y);
                    projectileSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
                    _registry.addComponent(projectile, Hitbox{});
                    _registry.addComponent(projectile, projectileSprite);
                    _registry.addComponent(projectile, shootTransform);
                    _registry.addComponent(projectile, Projectile{ .id = projectileId });
                    _registry.addComponent(projectile, Animation{.frameSize = {16, 16}, .speed = 20, .frameCount = 3, .loop = false, .velocity = Velocity{.x = 200, .y = 0}});
                    UDPPacket packet;
                    packet << PACKET_TYPE::NEW_PROJECTILE << event.source.id << projectileId;
                    for (const auto &client : CLIENTS)
                        _manager.send(client.endpoint, packet);
                    projectileId++;
                }
            }
        }
        default:
            break;
        }
    }

private:
    Registry &_registry;
    ISceneManager &_manager;

    sf::Texture _projectileTex;
};

#endif // PACKETHANDLER_HPP
