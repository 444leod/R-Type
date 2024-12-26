/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** PacketTypes.hpp
*/

#ifndef PACKETTYPES_HPP
#define PACKETTYPES_HPP

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
     MESSAGE,
     POSITION,
     START,
     YOUR_SHIP,
     NEW_SHIP,
     SHIP_MOVEMENT,
     USER_INPUT,
     NEW_PROJECTILE,
     NEW_MONSTER,
     MONSTER_KILLED
 };

#endif //PACKETTYPES_HPP
