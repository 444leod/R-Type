/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NetworkAgent
*/

#pragma once

#include <asio.hpp>
#include <iostream>
#include "UDPPacket.hpp"

class NetworkAgent
{
public:
    NetworkAgent(asio::io_context& ctx, std::uint32_t port = 0) :
        _socket(ctx, asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
    {
        this->_port = this->_socket.local_endpoint().port();
        this->_receive_packet();
    }
    ~NetworkAgent() = default;

protected:
    virtual void _onPacketReceived(const asio::ip::udp::endpoint& src, const UDPPacket& packet) = 0;

    void _stop()
    {
        this->_socket.close();
    }

    void _send(const asio::ip::udp::endpoint& dest, const std::string& msg)
    {
        UDPPacket packet(msg);
        auto serialized = _serialize_packet(packet);
        this->_socket.async_send_to(
            asio::buffer(serialized), dest,
            std::bind(
                &NetworkAgent::_handle_send, this,
                asio::placeholders::error,
                asio::placeholders::bytes_transferred
            )
        );
    }

private:
    void _receive_packet()
    {
        this->_socket.async_receive_from(
            asio::buffer(this->_buffer), this->_client,
            std::bind(
                &NetworkAgent::_handle_receive, this,
                asio::placeholders::error,
                asio::placeholders::bytes_transferred
            )
        );
    }

    void _handle_receive(const std::error_code& e, std::size_t bytes)
    {
        if (e)
            return;

        if (bytes > 0) {
            auto packet = _deserialize_packet(this->_buffer.data(), bytes);
            uint16_t calculated_checksum = packet.calculateChecksum();
            if (calculated_checksum == packet.checksum) {
                this->_onPacketReceived(this->_client, packet); // yipee
            } else {
                std::cerr << "got some corrupted packet :(" << std::endl;
            }
        }
        this->_receive_packet();
    }

    void _handle_send(const std::error_code& e, std::size_t bytes)
    {}

    std::vector<uint8_t> _serialize_packet(const UDPPacket& packet) {
        std::vector<uint8_t> buffer;
        buffer.resize(sizeof(packet.sequence_number) + sizeof(packet.ack_number) +
                     sizeof(packet.payload_length) + sizeof(packet.checksum) +
                     packet.payload.size());

        size_t offset = 0;
        memcpy(buffer.data() + offset, &packet.sequence_number, sizeof(packet.sequence_number));
        offset += sizeof(packet.sequence_number);
        memcpy(buffer.data() + offset, &packet.ack_number, sizeof(packet.ack_number));
        offset += sizeof(packet.ack_number);
        memcpy(buffer.data() + offset, &packet.payload_length, sizeof(packet.payload_length));
        offset += sizeof(packet.payload_length);
        memcpy(buffer.data() + offset, &packet.checksum, sizeof(packet.checksum));
        offset += sizeof(packet.checksum);
        memcpy(buffer.data() + offset, packet.payload.data(), packet.payload.size());

        return buffer;
    }

    UDPPacket _deserialize_packet(const char* data, size_t length) {
        UDPPacket packet;
        size_t offset = 0;

        memcpy(&packet.sequence_number, data + offset, sizeof(packet.sequence_number));
        offset += sizeof(packet.sequence_number);
        memcpy(&packet.ack_number, data + offset, sizeof(packet.ack_number));
        offset += sizeof(packet.ack_number);
        memcpy(&packet.payload_length, data + offset, sizeof(packet.payload_length));
        offset += sizeof(packet.payload_length);
        memcpy(&packet.checksum, data + offset, sizeof(packet.checksum));
        offset += sizeof(packet.checksum);

        packet.payload.resize(packet.payload_length);
        memcpy(packet.payload.data(), data + offset, packet.payload_length);

        return packet;
    }

protected:
    std::uint32_t _port = 0;
private:
    asio::ip::udp::socket _socket;
    asio::ip::udp::endpoint _client;
    std::array<char, 1024> _buffer;
};
