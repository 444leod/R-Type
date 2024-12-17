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

struct PacketInformations : public IEvent {
    PACKET_TYPE type;
    UDPPacket& packet;
};

class PacketHandler : public EventHandler<PacketInformations> {
public:
    explicit PacketHandler(Registry& registry, ISceneManager& manager) : _registry(registry), _manager(manager)
    {
        _spaceshipTex.loadFromFile("assets/r-typesheet42.gif", sf::IntRect(0, 0, 34, 18));
    }
    ~PacketHandler() override = default;

    void receive(const PacketInformations& event) override
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
                for (auto& [entity, ship, vel, pos] : _registry.view<Ship, Velocity, Transform>().each())
                {
                    if (ship.id == id)
                    {
                        vel = velocity;
                        pos = position;
                        break;
                    }
                }
                return;
            }
            default:
                break;
        }
    }


private:
    Entity _generateSpaceship(const Transform& transform)const
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

    Registry& _registry;
    ISceneManager& _manager;
    sf::Texture _spaceshipTex;
};



#endif //PACKETHANDLER_HPP
