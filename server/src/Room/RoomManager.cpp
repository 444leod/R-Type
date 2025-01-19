#include "RoomManager.hpp"
#include "Config.hpp"
#include "PacketTypes.hpp"
#include "PremadeComponents/Hitbox.hpp"
#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Velocity.hpp"
#include "SharedComponents/Parallax.hpp"
#include "SharedComponents/Ship.hpp"
#include "../scenes/Level1/Level1.hpp"
#include "../scenes/Level1/Modules/PacketHandlerSceneModule.hpp"

RoomManager& RoomManager::instance()
{
    static RoomManager instance;
    return instance;
}

void RoomManager::_initializeGameEntities(Room& room)
{
    std::cout << "Initializing game entities for room " << room.id << std::endl;
    for (const auto& player : room.players)
    {
        const auto ship = room.registry.create();
        room.registry.addComponent(ship, Ship{.id = player.id});
        room.registry.addComponent(ship, Transform{.x = 100.0f, .y = static_cast<float>(100 * (player.id + 1)), .z = 1.0f});
        room.registry.addComponent(ship, Velocity{});
        room.registry.addComponent(ship, Hitbox{});
    }

    const auto background = room.registry.create();
    room.registry.addComponent(background, Transform{.x = 0, .y = 0, .z = -1});
    room.registry.addComponent(background, Parallax{.offsetMultiplier = 25});
    std::cout << "Game entities initialized for room " << room.id << std::endl;
}

void RoomManager::startRoom(std::uint32_t roomId, const std::shared_ptr<ANetworkSceneModule>& net)
{
    auto it = _rooms.find(roomId);
    if (it == _rooms.end() || it->second->isStarted)
        return;

    auto& room = *it->second;
    if (room.players.size() < 1)
        return;

    room.isStarted = true;
    room.gameScene = std::make_shared<Level1>("game_" + std::to_string(roomId));
    // room.gameScene->setRegistry(&room.registry);
    room.gameScene->addModule<level1::PacketHandlerSceneModule>(net);
    room.gameScene->initialize();

    // _initializeGameEntities(room);

    ntw::UDPPacket packet;
    packet << PACKET_TYPE::ROOM_STARTED << roomId;

    std::cout << "Sending room started packet to " << room.players.size() << " players" << std::endl;

    for (const auto& player : room.players) {
        net->sendPacket(player.endpoint, packet);
    }
}

void RoomManager::updateRoom(std::uint32_t roomId, double deltaTime)
{
    auto it = _rooms.find(roomId);
    if (it == _rooms.end() || !it->second->isStarted)
        return;

    auto& room = *it->second;

    if (room.gameScene)
    {
        room.gameScene->update(deltaTime);
    }
}

void RoomManager::broadcastRoomState(std::uint32_t roomId, const std::shared_ptr<ANetworkSceneModule>& net)
{
    auto it = _rooms.find(roomId);
    if (it == _rooms.end() || !it->second->isStarted || !it->second->gameScene)
        return;

    auto& room = *it->second;
    if (room.players.empty()) {
        return;
    }

    // broadcast to players of the room the events (bugs, ship movement, etc.)
}

std::shared_ptr<Room> RoomManager::createRoom(const std::string& name, std::uint8_t maxPlayers)
{
    auto room = std::make_shared<Room>();
    room->id = _nextRoomId++;
    room->name = name;
    room->maxPlayers = maxPlayers;
    room->isStarted = false;

    _rooms[room->id] = room;
    std::cout << "Room created: " << name << " (id: " << room->id << ")" << std::endl;
    return room;
}

bool RoomManager::joinRoom(std::uint32_t roomId, const ntw::ClientInformation& client)
{
    auto it = _rooms.find(roomId);
    if (it == _rooms.end() || it->second->isStarted || it->second->players.size() >= it->second->maxPlayers)
    {
        return false;
    }

    it->second->players.push_back(client);
    return true;
}

bool RoomManager::leaveRoom(std::uint32_t roomId, const asio::ip::udp::endpoint& endpoint)
{
    auto it = _rooms.find(roomId);
    if (it == _rooms.end())
        return false;

    auto& players = it->second->players;
    auto playerIt = std::find_if(players.begin(), players.end(), [&endpoint](const auto& p) { return p.endpoint == endpoint; });

    if (playerIt == players.end())
        return false;

    players.erase(playerIt);
    if (players.empty())
    {
        _rooms.erase(it);
    }
    return true;
}

std::vector<Room> RoomManager::getRooms() const
{
    std::vector<Room> rooms;
    for (const auto& [_, room] : _rooms)
    {
        rooms.push_back(*room);
    }
    return rooms;
}

void RoomManager::endRoom(std::uint32_t roomId)
{
    auto it = _rooms.find(roomId);
    if (it == _rooms.end())
        return;

    auto& room = *it->second;
    // clear the room
    // notify the players
}
