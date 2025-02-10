/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** NetworkGameModule.hpp
*/

#ifndef NETWORK_GAME_MODULE_HPP
#define NETWORK_GAME_MODULE_HPP

#include "PremadeModules/Network/ANetworkGameModule.hpp"

#include "PacketTypes.hpp"

/**
 * @class NetworkGameModule
 *
 * @brief The abstract class is enough for the server to handle the Network part of the game
 */
class NetworkGameModule final : public ANetworkGameModule
{
  public:
    ~NetworkGameModule() override = default;

  private:
};

#endif // NETWORK_GAME_MODULE_HPP
