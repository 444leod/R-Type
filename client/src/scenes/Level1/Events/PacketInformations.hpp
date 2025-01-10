/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** PacketInformations
*/

#ifndef PACKETINFORMATIONS_HPP_
#define PACKETINFORMATIONS_HPP_

#include "NetworkAgent.hpp"

struct PacketInformations : public IEvent
{
    PACKET_TYPE type;
    UDPPacket &packet;
};

#endif /* !PACKETINFORMATIONS_HPP_ */
