/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** PacketInformation
*/

#ifndef PACKET_INFORMATION_HPP_
#define PACKET_INFORMATION_HPP_

#include "ecs/Events.hpp"
#include "network/NetworkAgent.hpp"
#include "PacketTypes.hpp"

struct PacketInformation : public ecs::IEvent
{
    PACKET_TYPE type;
    ntw::UDPPacket &packet;
    ntw::ClientInformation &source;
};

#endif /* !PACKET_INFORMATION_HPP_ */
