/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShipMovedSystem
*/

#include "ShipMovedSystem.hpp"
#include "BaseComponents.hpp"
#include "../Components.hpp"

#include "network/NetworkAgent.hpp"
#include <SFML/Graphics.hpp>
#include "PacketTypes.hpp"

void ShipMovedSystem::execute(const PacketInformation &event, const UserInput &input) const
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

        ntw::UDPPacket packet;
        packet << PACKET_TYPE::SHIP_MOVEMENT << event.source.id << vel << pos;
        // for (const auto &client : CLIENTS) //TODO: update this
        // {
            // _manager.send(client.endpoint, packet);
        // }
    }
}