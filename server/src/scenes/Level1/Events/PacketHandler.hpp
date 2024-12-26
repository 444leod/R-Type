/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** PacketHandler.hpp
*/

#ifndef PACKETHANDLER_HPP
#define PACKETHANDLER_HPP

#include "../../UserInput.hpp"
#include "ecs/EventDispatcher.hpp"
#include "ecs/Registry.hpp"
#include "network/UDPPacket.hpp"
#include "network/NetworkAgent.hpp"
#include "engine/RestrictedSceneManager.hpp"
#include "../Systems/ShipMovedSystem.hpp"
#include "../Systems/ShipShootedSystem.hpp"
#include "PacketTypes.hpp"

class PacketHandler final : public ecs::EventHandler<PacketInformation>
{
public:
    explicit PacketHandler(ecs::Registry &registry, RestrictedSceneManager &manager) :
        _registry(registry),
        _manager(manager),
        _shipMovedSystem(_registry, _manager),
        _shipShootedSystem(_registry, _manager)
    {
        _projectileTex.loadFromFile("assets/r-typesheet1.gif", sf::IntRect(0, 91, 48, 16));
    }

    ~PacketHandler() override = default;

    void receive(const PacketInformation &event) override
    {
        switch (event.type)
        {
        case PACKET_TYPE::USER_INPUT:
        {
            UserInput input{};
            event.packet >> input;

            if (input.key >= sf::Keyboard::Left && input.key <= sf::Keyboard::Down)
            {
                _shipMovedSystem.execute(event, input);
            }
            else if (input.key == sf::Keyboard::Space)
            {
                _shipShootedSystem.execute(event, _projectileTex);
            }
        }
        default:
            break;
        }
    }

private:
    ecs::Registry &_registry;
    RestrictedSceneManager &_manager;

    sf::Texture _projectileTex;
    ShipMovedSystem _shipMovedSystem;
    ShipShootedSystem _shipShootedSystem;
};

#endif // PACKETHANDLER_HPP
