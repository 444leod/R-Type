/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** RemoveClientSystem.hpp
*/

#ifndef REMOVE_CLIENT_SYSTEM_HPP
#define REMOVE_CLIENT_SYSTEM_HPP


#include "BaseSystems/Abstracts/ASystem.hpp"
#include "Components.hpp"
#include "NetworkModules/ANetworkSceneModule.hpp"
#include <asio.hpp>

class RemoveClientSystem final : public ASystem
{
public:
    explicit RemoveClientSystem(ecs::Registry &registry) : ASystem(registry, "RemoveClientSystem") {}

    void execute(const asio::ip::udp::endpoint& src, const std::shared_ptr<ANetworkSceneModule>& net) const
    {
        for (auto& [entity, client] : _registry.view<Client>().each() )
        {
            if (client.info.endpoint == src) {
                _registry.remove(entity);
                break;
            }
        }

        auto clients = net->clients();

        const auto client = std::ranges::find_if(clients, [&src](const auto& actualClient) {
            return actualClient.endpoint == src;
        });

        if (client == clients.end())
            return;

        std::cout << "Client disconnected: " << src << std::endl;

        ntw::UDPPacket disconnectPacket;
        disconnectPacket << PACKET_TYPE::CLIENT_DISCONNECTED;
        disconnectPacket << client->id;

        net->removeClient(client->id);

        net->queuePacket(disconnectPacket);

    }
};

#endif //REMOVE_CLIENT_SYSTEM_HPP
