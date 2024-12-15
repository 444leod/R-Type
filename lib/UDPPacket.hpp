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

    // String constructor (sending)
    UDPPacket(const std::string& msg = "") :
        sequence_number(0),
        ack_number(0),
        payload_length(msg.length()),
        checksum(0)
    {
        payload.assign(msg.begin(), msg.end());
        checksum = calculateChecksum();
    }

    // Raw data constructor (receiving data and deserializing)
    UDPPacket(const char* data, size_t length) {
        _deserialize(data, length);
    }

    std::vector<uint8_t> serialize() const {
        std::vector<uint8_t> buffer;
        buffer.resize(sizeof(sequence_number) + sizeof(ack_number) +
                     sizeof(payload_length) + sizeof(checksum) +
                     payload.size());

        size_t offset = 0;
        memcpy(buffer.data() + offset, &sequence_number, sizeof(sequence_number));
        offset += sizeof(sequence_number);
        memcpy(buffer.data() + offset, &ack_number, sizeof(ack_number));
        offset += sizeof(ack_number);
        memcpy(buffer.data() + offset, &payload_length, sizeof(payload_length));
        offset += sizeof(payload_length);
        memcpy(buffer.data() + offset, &checksum, sizeof(checksum));
        offset += sizeof(checksum);
        memcpy(buffer.data() + offset, payload.data(), payload.size());

        return buffer;
    }

    uint16_t calculateChecksum() const {
        uint16_t sum = 0;

        // little explaination: sequence and ack are 32 bits, but checksum is 16 bits (for optimization purposes)
        // in order to checkum the whole 32 bits, we need to split them into 16 bits chunks
        // for exemple: 0x12345678 -> 0x1234 + 0x5678
        sum += (sequence_number >> 16) & 0xFFFF; // 0x00001234 => 0x1234
        sum += sequence_number & 0xFFFF; // 0x5678

        sum += (ack_number >> 16) & 0xFFFF;
        sum += ack_number & 0xFFFF;

        sum += payload_length;

        for (const auto& byte : payload) {
            sum += byte;
        }
        return sum;
    }

    private:
        void _deserialize(const char* data, size_t length) {
            size_t offset = 0;

            memcpy(&sequence_number, data + offset, sizeof(sequence_number));
            offset += sizeof(sequence_number);
            memcpy(&ack_number, data + offset, sizeof(ack_number));
            offset += sizeof(ack_number);
            memcpy(&payload_length, data + offset, sizeof(payload_length));
            offset += sizeof(payload_length);
            memcpy(&checksum, data + offset, sizeof(checksum));
            offset += sizeof(checksum);

            payload.resize(payload_length);
            memcpy(payload.data(), data + offset, payload_length);
        }
};
