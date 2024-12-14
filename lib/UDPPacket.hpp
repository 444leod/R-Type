/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** UDPPacket
*/

#pragma once

#include <vector>
#include <string>

struct UDPPacket {
    uint32_t sequence_number;
    uint32_t ack_number;
    uint16_t payload_length;
    uint16_t checksum;
    std::vector<uint8_t> payload;

    UDPPacket(const std::string& msg = "") :
        sequence_number(0),
        ack_number(0),
        payload_length(msg.length()),
        checksum(0)
    {
        payload.assign(msg.begin(), msg.end());
        checksum = calculateChecksum();
    }

public:
    // TODO: implement something better
    uint16_t calculateChecksum() const {
        uint16_t sum = 0;
        for (const auto& byte : payload) {
            sum += byte;
        }
        return sum;
    }
};
