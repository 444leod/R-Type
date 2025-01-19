#ifndef ROOM_HPP_
#define ROOM_HPP_

#include <string>
#include <vector>
#include <memory>
#include "Network/NetworkAgent.hpp"
#include "ECS/Registry.hpp"
#include "../scenes/Level1/Level1.hpp"

struct Room {
    std::uint32_t id;
    std::string name;
    std::uint8_t maxPlayers;
    bool isStarted = false;
    std::vector<ntw::ClientInformation> players;
    ecs::Registry registry;
    std::shared_ptr<Level1> gameScene;
};

#endif
