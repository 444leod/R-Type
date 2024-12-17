/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** PacketHandler.hpp
*/

#ifndef PACKETHANDLER_HPP
#define PACKETHANDLER_HPP

#include "../Components.hpp"
#include "EventDispatcher.hpp"
#include "Registry.hpp"
#include "UDPPacket.hpp"
#include "NetworkAgent.hpp"
#include "Global.hpp"
#include "ISceneManager.hpp"
#include <cstdint>

struct PacketInformations : public IEvent {
    PACKET_TYPE type;
    UDPPacket& packet;
    ClientInformations& source;
};

class PacketHandler final : public EventHandler<PacketInformations> {
public:
    explicit PacketHandler(Registry& registry, ISceneManager& manager) : _registry(registry), _manager(manager) {}
    ~PacketHandler() override = default;

    void receive(const PacketInformations& event) override
    {
        switch (event.type)
        {
            case PACKET_TYPE::USER_INPUT:
            {
                for (auto [ship_id, ship, vel, pos] : _registry.view<Ship, Velocity, Transform>()) {
                    if (ship.id == event.source.id) {
                        UserInput input{};
                        event.packet >> input;

                        switch (input.key) {
                            case sf::Keyboard::Key::Up:
                                vel.y += input.pressed ? -75 : 75;
                                break;
                            case sf::Keyboard::Key::Down:
                                vel.y += input.pressed ? 75 : -75;
                                break;
                            case sf::Keyboard::Key::Left:
                                vel.x += input.pressed ? -75 : 75;
                                break;
                            case sf::Keyboard::Key::Right:
                                vel.x += input.pressed ? 75 : -75;
                                break;
                            default:
                                break;
                        }

                        UDPPacket packet;
                        packet << PACKET_TYPE::SHIP_MOVEMENT << event.source.id << vel << pos;
                        for (const auto& client : CLIENTS) {
                            _manager.send(client.endpoint, packet);
                        }
                    }
                }
            }
            default:
                break;
        }
    }


private:
    Registry& _registry;
    ISceneManager& _manager;
};



#endif //PACKETHANDLER_HPP
