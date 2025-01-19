#ifndef ROOM_MANAGER_HPP_
#define ROOM_MANAGER_HPP_

#include "PremadeModules/Network/ANetworkSceneModule.hpp"
#include "Room.hpp"
#include <map>
#include <memory>

class RoomManager
{
  public:
    static RoomManager& instance();

    std::shared_ptr<Room> createRoom(const std::string& name, std::uint8_t maxPlayers);
    bool joinRoom(std::uint32_t roomId, const ntw::ClientInformation& client);
    bool leaveRoom(std::uint32_t roomId, const asio::ip::udp::endpoint& endpoint);
    void startRoom(std::uint32_t roomId, const std::shared_ptr<ANetworkSceneModule>& net);
    std::vector<Room> getRooms() const;

    void updateRoom(std::uint32_t roomId, double deltaTime);
    void broadcastRoomState(std::uint32_t roomId, const std::shared_ptr<ANetworkSceneModule>& net);

    std::shared_ptr<Room> getRoom(std::uint32_t roomId)
    {
        auto it = _rooms.find(roomId);
        return it != _rooms.end() ? it->second : nullptr;
    }

    void endRoom(std::uint32_t roomId);

  private:
    RoomManager() = default;
    void _initializeGameEntities(Room& room);

    std::map<std::uint32_t, std::shared_ptr<Room>> _rooms;
    std::uint32_t _nextRoomId = 0;
};

#endif
