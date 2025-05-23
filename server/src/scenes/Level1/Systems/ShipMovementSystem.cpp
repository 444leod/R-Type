/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ShipMovementSystem
*/

#include "ShipMovementSystem.hpp"

#include "PremadeComponents/Hitbox.hpp"
#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Velocity.hpp"

#include "SharedComponents/Client.hpp"
#include "SharedComponents/Ship.hpp"

#include "PacketTypes.hpp"

static std::optional<ecs::Entity> getEntityBySource(ecs::Registry& registry, const asio::ip::udp::endpoint& source)
{
    try {
        for (auto [entity, info] : registry.view<Client>().each())
        {
            if (info.info.endpoint == source)
                return entity;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in getEntityBySource: " << e.what() << std::endl;
    }
    return std::nullopt;
}

void ShipMovementSystem::execute(const asio::ip::udp::endpoint& source, const UserInput& input) const
{
    const auto entityId = getEntityBySource(_registry, source);
    if (!entityId.has_value())
        return;

    const auto [id] = _registry.get<Ship>(*entityId);
    auto& velocity = _registry.get<Velocity>(*entityId);
    const auto pos = _registry.get<Transform>(*entityId);

    switch (input.key)
    {
    case sf::Keyboard::Key::Up:
        velocity.y = input.pressed ? -75 : 0;
        break;
    case sf::Keyboard::Key::Down:
        velocity.y = input.pressed ? 75 : 0;
        break;
    case sf::Keyboard::Key::Left:
        velocity.x = input.pressed ? -75 : 0;
        break;
    case sf::Keyboard::Key::Right:
        velocity.x = input.pressed ? 75 : 0;
        break;
    default:
        break;
    }

    ntw::UDPPacket packet;
    packet << PACKET_TYPE::SHIP_MOVEMENT << id << velocity << pos;
    _net->sendPacket(packet);
}
