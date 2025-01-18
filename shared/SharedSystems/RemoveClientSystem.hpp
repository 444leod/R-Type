/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** RemoveClientSystem.hpp
*/

#ifndef REMOVE_CLIENT_SYSTEM_HPP
#define REMOVE_CLIENT_SYSTEM_HPP

#include <Engine/Systems/ASystem.hpp>

#include "PremadeModules/Network/ANetworkSceneModule.hpp"

#include "SharedComponents/Client.hpp"

#include <asio.hpp>

class RemoveClientSystem final : public engine::ASystem
{
public:
    explicit RemoveClientSystem() : ASystem("RemoveClientSystem") {}

    void execute(const asio::ip::udp::endpoint& src, const std::shared_ptr<ANetworkSceneModule>& net) const
    {
        for (auto& [entity, client] : _registry.view<Client>().each())
        {
            if (client.info.endpoint == src)
            {
                _registry.remove(entity);
                break;
            }
        }

        auto clients = net->clients();

        const auto client = std::ranges::find_if(clients, [&src](const auto& actualClient) { return actualClient.endpoint == src; });

        if (client == clients.end())
            return;

        std::cout << "Client disconnected: " << src << std::endl;

        ntw::UDPPacket disconnectPacket;
        disconnectPacket << PACKET_TYPE::CLIENT_DISCONNECTED;
        disconnectPacket << client->id;

        net->removeClient(client->id);

        net->sendPacket(disconnectPacket);
    }
};

#endif // REMOVE_CLIENT_SYSTEM_HPP
