/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** NetworkGameModule.hpp
*/

#ifndef NETWORK_GAME_MODULE_HPP
#define NETWORK_GAME_MODULE_HPP

#include "NetworkModules/ANetworkGameModule.hpp"

/**
* @class NetworkGameModule
*
* @brief The abstract class is enough for the server to handle the network part of the game
*/
class NetworkGameModule final : public ANetworkGameModule {
public:
    explicit NetworkGameModule(game::RestrictedGame& game, const std::uint32_t& port = 25565) : ANetworkGameModule(game, port) {}
    ~NetworkGameModule() override = default;

private:

};

#endif //NETWORK_GAME_MODULE_HPP
