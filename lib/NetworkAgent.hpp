/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NetworkAgent
*/

#pragma once

#include <asio.hpp>

class NetworkAgent
{
public:
    /**
     * @brief Constructor for the NetworkAgent class
     * @param ctx The io_context
     * @param port The port to create the agent at
     */
    NetworkAgent(asio::io_context& ctx, std::uint32_t port) :
        _socket(ctx, asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
    {
        this->_port = this->_socket.local_endpoint().port();
        this->_receivePacket();
    }
    ~NetworkAgent() = default;

protected:
    /**
     * @brief Virtual method called when a packet was successfully received
     * @param src Where the packet was received from
     * @param msg The message received
     */
    virtual void _onPacketReceived(const asio::ip::udp::endpoint& src, const std::string& msg) = 0;

    /**
     * @brief Try to send a message to an endpoint
     * @param dest The endpoint to send to
     * @param msg The message to send
     */
    void _send(const asio::ip::udp::endpoint& dest, const std::string& msg)
    {
        this->_socket.async_send_to(
            asio::buffer(msg), dest,
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
        // Do not proceed in case of error
        if (e)
            return;
        // Limit buffer
        if (bytes < 1024)
            this->_buffer[bytes] = '\0';
        // Call overriden handler
        this->_onPacketReceived(this->_client, std::string(this->_buffer.data()));
        // Add job for another packet
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
