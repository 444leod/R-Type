#ifndef ROOM_HPP_
#define ROOM_HPP_

#include "SharedComponents/Client.hpp"
#include <ECS/Registry.hpp>
#include <Engine/AScene.hpp>
#include <asio.hpp>
#include <cstdint>
#include <string>
#include <vector>

struct Room
{
    std::uint32_t id;
    std::string name;
    std::vector<ntw::ClientInformation> players;
    std::uint8_t maxPlayers;
    bool isStarted;
    ecs::Registry& registry;
    std::shared_ptr<engine::AScene> gameScene;
};

#endif
