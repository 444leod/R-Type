/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** PacketInformations
*/

#ifndef PACKETINFORMATIONS_HPP_
#define PACKETINFORMATIONS_HPP_

#include "ecs/Events.hpp"
#include "network/NetworkAgent.hpp"

struct PacketInformations : public ecs::IEvent
{
    ntw::PACKET_TYPE type;
    ntw::UDPPacket &packet;
};

#endif /* !PACKETINFORMATIONS_HPP_ */
