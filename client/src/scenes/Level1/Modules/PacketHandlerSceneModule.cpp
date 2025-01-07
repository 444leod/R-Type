/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketHandlerSceneModule.cpp
*/

#include "PacketHandlerSceneModule.hpp"

#include "../Systems/ShipMovementSystem.hpp"
#include "../Systems/NewProjectileSystem.hpp"
#include "../Systems/NewShipSystem.hpp"
#include "../Systems/NewMonsterSystem.hpp"
#include "../Systems/MonsterKilledSystem.hpp"

namespace level1
{

void handleShipMovement(ecs::Registry& registry, RestrictedSceneManager& manager, const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
{
    static ShipMovementSystem shipMovementSystem(registry);

    std::uint32_t id;
    Velocity velocity{};
    Transform position{};
    packet >> id >> velocity >> position;

    shipMovementSystem.execute(id, velocity, position);
}

void handleNewProjectile(ecs::Registry& registry, RestrictedSceneManager& manager, const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
{
    static NewProjectileSystem newProjectileSystem(registry);

    std::uint32_t shipId;
    std::uint32_t projectileId;
    Transform transform{};
    Velocity velocity{};
    packet >> shipId >> projectileId >> transform >> velocity;

    newProjectileSystem.execute(shipId, projectileId, transform, velocity);
}

void handleCurrentPlayerShip(ecs::Registry& registry, RestrictedSceneManager& manager, const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
{
    static NewShipSystem newShipSystem(registry);

    std::uint32_t id;
    Transform transform{};
    Velocity velocity{};
    packet >> id >> transform >> velocity;

    auto spaceship = newShipSystem.execute(id, transform, velocity);
    registry.addComponent(spaceship, Self{});
}

void handleNewShip(ecs::Registry& registry, RestrictedSceneManager& manager, const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
{
    static NewShipSystem newShipSystem(registry);

    std::uint32_t id;
    Transform transform{};
    Velocity velocity{};
    packet >> id >> transform >> velocity;

    newShipSystem.execute(id, transform, velocity);
}

void handleNewMonster(ecs::Registry& registry, RestrictedSceneManager& manager, const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
{
    static NewMonsterSystem newMonsterSystem(registry);

    std::uint32_t id;
    std::uint8_t type;
    Transform transform{};
    Velocity velocity{};
    packet >> id >> type >> transform >> velocity;

    newMonsterSystem.execute(id, type, transform, velocity);
}

void handleMonsterKilled(ecs::Registry& registry, RestrictedSceneManager& manager, const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
{
    static MonsterKilledSystem monsterKilledSystem(registry);

    std::uint32_t monsterId;
    std::uint32_t projectileId;
    packet >> monsterId >> projectileId;

    monsterKilledSystem.execute(monsterId, projectileId);
}

void handleDisconnect(ecs::Registry& registry, RestrictedSceneManager& manager, const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
{
    manager.stop();
}

PacketHandlerSceneModule::PacketHandlerSceneModule(AScene& scene, ecs::Registry& registry, RestrictedSceneManager& sceneManager) : APacketHandlerSceneModule(scene, registry, sceneManager) {
    this->setHandler(PACKET_TYPE::SHIP_MOVEMENT, handleShipMovement);
    this->setHandler(PACKET_TYPE::NEW_PROJECTILE, handleNewProjectile);
    this->setHandler(PACKET_TYPE::YOUR_SHIP, handleCurrentPlayerShip);
    this->setHandler(PACKET_TYPE::NEW_SHIP, handleNewShip);
    this->setHandler(PACKET_TYPE::NEW_MONSTER, handleNewMonster);
    this->setHandler(PACKET_TYPE::MONSTER_KILLED, handleMonsterKilled);
    this->setHandler(PACKET_TYPE::DISCONNECT, handleDisconnect);
}

}