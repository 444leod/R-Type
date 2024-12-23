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
#include "ISceneManager.hpp"
#include "../Systems/ShipMovedSystem.hpp"
#include "../Systems/ShipShootedSystem.hpp"
#include <cstdint>
#include <config.h>


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
    Registry &_registry;
    ISceneManager &_manager;

    sf::Texture _projectileTex;
    ShipMovedSystem _shipMovedSystem{_registry, _manager};
    ShipShootedSystem _shipShootedSystem{_registry, _manager};
    // std::vector<std::shared_ptr<ASystem>> _systems = {_shipMovedSystem}; 
};

#endif // PACKETHANDLER_HPP
