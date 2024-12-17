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

/**
 * @brief This enum will describe what the packet is for.
 *
 *  - NONE: No packet type
 *  - CONNECT: Connection packet
 *  - DISCONNECT: Disconnection packet
 *  - MESSAGE: Message packet (temporary
 *  - POSITION: Position packet (temporary)
*/
enum class PACKET_TYPE {
    NONE = 0,
    CONNECT,
    DISCONNECT,
    MESSAGE,
    POSITION,
    START
};

/**
 * @brief Class representation of an object connected via socket, that can receive and send packets.
 */
class NetworkAgent
{
public:
    /**
     * @brief Constructor for the NetworkAgent class
     * @param ctx The io_context
     * @param port The port to create the agent at
     */
    NetworkAgent(asio::io_context& ctx, std::uint32_t port = 0) :
        _socket(ctx, asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
    {
        this->_port = this->_socket.local_endpoint().port();
        this->_receivePacket();
        std::cout << "NetworkAgent started, listening on port: " << this->_socket.local_endpoint().address().to_string() << ":" << this->_port << "..." << std::endl;
    }
    virtual ~NetworkAgent() = default;

protected:
    /**
     * @brief Virtual method called when a packet was successfully received
     * @param src Where the packet was received from
     * @param packet The packet received
     */
    virtual void _onPacketReceived(const asio::ip::udp::endpoint& src, UDPPacket& packet) = 0;

    /**
     * @brief Stops any asio work from this agent
     */
    void _stop()
    {
        this->_socket.close();
    }

    /**
     * @brief Try to send a message to an endpoint
     * @param dest The endpoint to send to
     * @param packet The packet to send
     */
    void _send(const asio::ip::udp::endpoint& dest, const UDPPacket& packet)
    {
        auto serialized = packet.serialize();
        std::cout << "Sending packet to " << dest.address().to_string() << ":" << dest.port() << std::endl;
        std::cout << "Packet content: " << packet.payload << std::endl;
        this->_socket.async_send_to(
            asio::buffer(serialized), dest,
            std::bind(
                &NetworkAgent::_handleSend, this,
                asio::placeholders::error,
                asio::placeholders::bytes_transferred
            )
        );
    }

private:
    /**
     * @brief Private method used to start receiving a packet
     */
    void _receivePacket()
    {
        std::cout << "Waiting for packet..." << std::endl;
        this->_socket.async_receive_from(
            asio::buffer(this->_buffer), this->_client,
            std::bind(
                &NetworkAgent::_handleReceive, this,
                asio::placeholders::error,
                asio::placeholders::bytes_transferred
            )
        );
    }

    /**
     * @brief Handler used when receiving a packet, will determined if all went according to plan
     * @param e Error code linked to the reception
     * @param bytes The amount of bytes received
     */
    void _handleReceive(const std::error_code& e, std::size_t bytes)
    {
        if (e)
            return;

        if (bytes > 0) {
            UDPPacket packet(this->_buffer.data(), bytes);
            uint16_t calculated_checksum = packet.calculateChecksum();
            if (calculated_checksum == packet.checksum) {
                this->_onPacketReceived(this->_client, packet); // yipee
            } else {
                std::cerr << "got some corrupted packet :( (checksum mismatch: " << calculated_checksum << " != " << packet.checksum << ")" << std::endl;
            }
        }
        this->_receivePacket();
    }

    /**
     * @brief Handler used after sending a packer
     * @param e Error code linked to the sending
     * @param bytes The amount of bytes sent
     */
    void _handleSend(const std::error_code& e, std::size_t bytes)
    {}

protected:
    std::uint32_t _port = 0;
private:
    asio::ip::udp::socket _socket;
    asio::ip::udp::endpoint _client;
    std::array<char, 1024> _buffer;
};
