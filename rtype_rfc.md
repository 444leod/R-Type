# R-Type Network Protocol RFC Documentation

## Table of Contents
- [R-Type Network Protocol RFC Documentation](#r-type-network-protocol-rfc-documentation)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Features](#features)
  - [Packet Structure](#packet-structure)
  - [Packet Types](#packet-types)
  - [Network Communication](#network-communication)
    - [Client Information Structure](#client-information-structure)
    - [Connection Flow](#connection-flow)
  - [Packet Validation](#packet-validation)
    - [UDP Packet Structure](#udp-packet-structure)
    - [Packet Byte Schemas](#packet-byte-schemas)
  - [Server Configuration](#server-configuration)
  - [Error Handling](#error-handling)
  - [Game State Synchronization](#game-state-synchronization)
    - [Position Updates](#position-updates)
    - [Entity Types](#entity-types)
  - [Example Packet Flows](#example-packet-flows)
    - [Player Movement](#player-movement)
    - [Weapon Firing](#weapon-firing)
    - [Monster Spawn](#monster-spawn)
  - [Data Serialization](#data-serialization)
    - [Basic Types](#basic-types)
    - [Strings and Arrays](#strings-and-arrays)
    - [Complex Example](#complex-example)

## Introduction
This document outlines the specification of the R-Type network protocol, used for client-server communication in the R-Type game implementation.

## Features
The R-Type network protocol supports the following features:
- UDP-based client-server communication
- Player connection and disconnection management
- Game state synchronization
- Player input handling
- Position updates
- Game events broadcasting

## Packet Structure

All network communication is done through UDP packets. Each packet contains:
- A checksum for packet validation
- A packet type identifier
- A payload containing the actual data

The server validates incoming packets by comparing the calculated checksum with the provided one.

## Packet Types

The protocol defines several packet types for different purposes:

- **NONE** (0): No packet type
- **CONNECT**: Connection packet
- **DISCONNECT**: Disconnection packet
- **MESSAGE**: Message packet
- **START**: Game start packet
- **YOUR_SHIP**: Ship assignment packet
- **NEW_SHIP**: New ship notification
- **SHIP_MOVEMENT**: Ship movement update
- **USER_INPUT**: User input state
- **NEW_PROJECTILE**: New projectile creation
- **NEW_MONSTER**: New monster spawned
- **MONSTER_KILLED**: Monster destruction notification

## Network Communication

### Client Information Structure

Each client in the system is represented by:
- Network endpoint (IP address and port)
- Type (VIEWER or PLAYER)
- Name (client identifier)

### Connection Flow

1. **Initial Connection**
   - Client sends CONNECT packet to server
   - Server assigns unique ID and responds with CONNECT packet

2. **Game Start**
   - Server sends START packet to all clients
   - Server sends YOUR_SHIP to each player with their ship assignment
   - Server broadcasts NEW_SHIP to all clients for each player

3. **Gameplay Communication**
   - Clients send USER_INPUT packets for player actions
   - Server broadcasts SHIP_MOVEMENT updates
   - Server sends NEW_PROJECTILE for weapon firing
   - Server broadcasts NEW_MONSTER and MONSTER_KILLED events

4. **Disconnection**
   - Client sends DISCONNECT packet
   - Server broadcasts disconnection to other clients

## Packet Validation

### UDP Packet Structure

Each packet consists of:
- Sequence number (4 bytes): Incremental number
- Acknowledgment number (4 bytes): Incremental number
- Payload length (2 bytes): Length of the payload
- Checksum (2 bytes): CRC32 checksum of the packet
- Payload (variable length)

### Packet Byte Schemas

1. **CONNECT Packet**
```
[4 bytes] Packet Type (CONNECT)
[4 bytes] User ID
```

2. **SHIP_MOVEMENT Packet**
```
[4 bytes] Packet Type (SHIP_MOVEMENT)
[4 bytes] Entity ID
[4 bytes] X Position (float)
[4 bytes] Y Position (float)
[4 bytes] Z Position (float)
[4 bytes] Rotation (float)
[4 bytes] X Velocity (float)
[4 bytes] Y Velocity (float)
```

3. **USER_INPUT Packet**
```
[4 bytes] Packet Type (USER_INPUT)
[4 bytes] Key (sf::Keyboard::Key)
[1 byte] Is pressed (bool)
```

4. **NEW_PROJECTILE Packet**
```
[4 bytes] Packet Type (NEW_PROJECTILE)
[4 bytes] Projectile ID
[4 bytes] X Position (float)
[4 bytes] Y Position (float)
[4 bytes] Z Position (float)
[4 bytes] Rotation (float)
```

## Server Configuration

- Default port: 25565
- UDP socket buffer size: 1024 bytes

## Error Handling

The server handles various error cases:
- Invalid packet types
- Malformed packet data

## Game State Synchronization

### Position Updates

Each entity's position in the game world is represented by:
- Position coordinates (X, Y, Z)
- Rotation angle
- Movement velocity (X, Y)

### Entity Types
- Ships (player-controlled)
- Projectiles
- Monsters
- Power-ups

## Example Packet Flows

### Player Movement
```
Client -> Server: USER_INPUT {
    key: sf::Keyboard::Key
    is_pressed: bool
}
Server -> All: SHIP_MOVEMENT {
    entity_id: uint32
    position_x: float
    position_y: float
    position_z: float
    rotation: float
    velocity_x: float
    velocity_y: float
}
```

### Weapon Firing
```
Server -> All: NEW_PROJECTILE {
    ship_id: uint32
    projectile_id: uint32
}
```

### Monster Spawn
```
Server -> All: NEW_MONSTER {
    monster_id: uint32
    x: float
    y: float
    z: float
    rotation: float
}
```

## Data Serialization

The protocol uses a specific format for serializing different data types in packet payloads:

### Basic Types
For basic types (int, float, etc.):
```
[1 byte] Data size
[N bytes] Data content
```

Example with int32:
```
[1 byte] Size = 4
[4 bytes] Integer value
```

### Strings and Arrays
For variable-length data (strings, arrays):
```
[2 bytes] Length of the sequence
[1 byte] Size of each element
[N bytes] Data content
```

Example with string "Hello":
```
[2 bytes] Length = 5
[1 byte] Size = 1 (char)
[5 bytes] "Hello"
```

Example with array of 2 int32:
```
[2 bytes] Length = 2
[1 byte] Size = 4 (int32)
[8 bytes] Two integers
```

### Complex Example
Packet containing an int32 followed by a string:
```
[1 byte] Size = 4
[4 bytes] Integer value
[2 bytes] String length
[1 byte] Element size = 1
[N bytes] String content
```
