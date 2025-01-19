/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** PacketTypes.hpp
*/

#ifndef PACKET_TYPES_HPP
#define PACKET_TYPES_HPP

/**
  * @brief This enum will describe what the packet is for.
  *
  *  - NONE: No packet type
  *  - CONNECT: Connection packet
  *  - DISCONNECT: Disconnection packet
  *  - MESSAGE: Message packet (temporary
  *  - POSITION: Position packet (temporary)
  */
enum class PACKET_TYPE
{
    NONE = 0,

    CONNECT,
    DISCONNECT,
    AUTHENTICATED,
    NEW_CLIENT,
    CLIENT_DISCONNECTED,

    MESSAGE,

    START,

    POSITION,
    YOUR_SHIP,
    NEW_SHIP,

    SHIP_MOVEMENT,
    USER_INPUT,

    NEW_PROJECTILE,
    NEW_MONSTER,
    MONSTER_KILLED,

    CREATE_ROOM,
    JOIN_ROOM,
    ROOM_LIST,
    ROOM_JOINED,
    ROOM_LEFT,
    ROOM_STARTED,
    ROOM_STATE,
    ROOM_ERROR
};

#endif //PACKET_TYPES_HPP
