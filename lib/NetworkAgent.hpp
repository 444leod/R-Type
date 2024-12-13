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
    NetworkAgent(asio::io_context& ctx, std::uint32_t port) :
        _socket(ctx, asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
    {
        this->_port = this->_socket.local_endpoint().port();
        this->_receive_packet();
    }
    ~NetworkAgent() = default;

protected:
    virtual void _onPacketReceived(const asio::ip::udp::endpoint& src, const std::string& msg) = 0;

    void _send(const asio::ip::udp::endpoint& dest, const std::string& msg)
    {
        this->_socket.async_send_to(
            asio::buffer(msg), dest,
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
        // Do not proceed in case of error
        if (e)
            return;
        // Limit buffer
        if (bytes < 1024)
            this->_buffer[bytes] = '\0';
        // Call overriden handler
        this->_onPacketReceived(this->_client, std::string(this->_buffer.data()));
        // Add job for another packet
        this->_receive_packet();
    }

    void _handle_send(const std::error_code& e, std::size_t bytes)
    {}

protected:
    std::uint32_t _port = 0;
private:
    asio::ip::udp::socket _socket;
    asio::ip::udp::endpoint _client;
    std::array<char, 1024> _buffer;
};
