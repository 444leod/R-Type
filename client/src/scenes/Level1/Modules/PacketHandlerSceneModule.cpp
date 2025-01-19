/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** PacketHandlerSceneModule.cpp
*/

#include "PacketHandlerSceneModule.hpp"

#include "../Systems/MonsterKilledSystem.hpp"
#include "../Systems/NewMonsterSystem.hpp"
#include "../Systems/NewProjectileSystem.hpp"
#include "../Systems/NewShipSystem.hpp"
#include "../Systems/ShipMovementSystem.hpp"

#include "PremadeComponents/Tags/Self.hpp"
#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Velocity.hpp"

#include "SharedComponents/Client.hpp"

namespace level1
{

void handleShipMovement(ecs::Registry&, const std::shared_ptr<ANetworkSceneModule>&, const asio::ip::udp::endpoint&, ntw::UDPPacket& packet)
{
    static ShipMovementSystem shipMovementSystem{};

    std::uint32_t id;
    Velocity velocity{};
    Transform position{};
    packet >> id >> velocity >> position;

    shipMovementSystem.execute(id, velocity, position);
}

void handleNewProjectile(ecs::Registry&, const std::shared_ptr<ANetworkSceneModule>&, const asio::ip::udp::endpoint&, ntw::UDPPacket& packet)
{
    static NewProjectileSystem newProjectileSystem{};

    std::uint32_t shipId;
    std::uint32_t projectileId;
    Transform transform{};
    Velocity velocity{};
    packet >> shipId >> projectileId >> transform >> velocity;

    newProjectileSystem.execute(shipId, projectileId, transform, velocity);
}

void handleCurrentPlayerShip(ecs::Registry& registry, const std::shared_ptr<ANetworkSceneModule>&, const asio::ip::udp::endpoint&, ntw::UDPPacket& packet)
{
    static NewShipSystem newShipSystem{};

    std::uint32_t id;
    Transform transform{};
    Velocity velocity{};
    packet >> id >> transform >> velocity;

    const auto spaceship = newShipSystem.execute(id, transform, velocity);
    registry.addComponent(spaceship, Self{});
}

void handleNewShip(ecs::Registry&, const std::shared_ptr<ANetworkSceneModule>&, const asio::ip::udp::endpoint&, ntw::UDPPacket& packet)
{
    static NewShipSystem newShipSystem{};

    std::uint32_t id;
    Transform transform{};
    Velocity velocity{};
    packet >> id >> transform >> velocity;

    (void)newShipSystem.execute(id, transform, velocity);
}

void handleNewMonster(ecs::Registry&, const std::shared_ptr<ANetworkSceneModule>&, const asio::ip::udp::endpoint&, ntw::UDPPacket& packet)
{
    static NewMonsterSystem newMonsterSystem{};

    std::uint32_t id;
    std::uint8_t type;
    Transform transform{};
    Velocity velocity{};
    packet >> id >> type >> transform >> velocity;

    newMonsterSystem.execute(id, type, transform, velocity);
}

void handleMonsterKilled(ecs::Registry&, const std::shared_ptr<ANetworkSceneModule>&, const asio::ip::udp::endpoint&, ntw::UDPPacket& packet)
{
    static MonsterKilledSystem monsterKilledSystem{};

    std::uint32_t monsterId;
    std::uint32_t projectileId;
    packet >> monsterId >> projectileId;

    monsterKilledSystem.execute(monsterId, projectileId);
}

void handleDisconnect(ecs::Registry&, const std::shared_ptr<ANetworkSceneModule>&, const asio::ip::udp::endpoint&, ntw::UDPPacket& packet) { engine::RestrictedGame::instance().stop(); }

void handleMessage(ecs::Registry&, const std::shared_ptr<ANetworkSceneModule>&, const asio::ip::udp::endpoint&, ntw::UDPPacket& packet)
{
    std::uint32_t id;
    std::string message;
    packet >> id >> message;
    std::cout << "Message from " << id << ": " << message << std::endl;
}

void handleClientDisconnected(ecs::Registry& registry, const std::shared_ptr<ANetworkSceneModule>&, const asio::ip::udp::endpoint&, ntw::UDPPacket& packet)
{
    std::uint32_t id;
    packet >> id;
    std::cout << "Client disconnected: " << id << std::endl;

    for (auto& [entity, client] : registry.view<Client>().each())
    {
        if (client.info.id == id)
        {
            registry.remove(entity);
            break;
        }
    }
}

PacketHandlerSceneModule::PacketHandlerSceneModule(engine::AScene& scene, const std::shared_ptr<ANetworkSceneModule>& net) : APacketHandlerSceneModule(scene, net)
{
    this->setHandler(PACKET_TYPE::DISCONNECT, handleDisconnect);
    this->setHandler(PACKET_TYPE::CLIENT_DISCONNECTED, handleClientDisconnected);

    this->setHandler(PACKET_TYPE::SHIP_MOVEMENT, handleShipMovement);
    this->setHandler(PACKET_TYPE::NEW_PROJECTILE, handleNewProjectile);
    this->setHandler(PACKET_TYPE::YOUR_SHIP, handleCurrentPlayerShip);
    this->setHandler(PACKET_TYPE::NEW_SHIP, handleNewShip);
    this->setHandler(PACKET_TYPE::NEW_MONSTER, handleNewMonster);
    this->setHandler(PACKET_TYPE::MONSTER_KILLED, handleMonsterKilled);

    this->setHandler(PACKET_TYPE::MESSAGE, handleMessage);

    this->setHandler(PACKET_TYPE::MESSAGE, handleMessage);
}

} // namespace level1
