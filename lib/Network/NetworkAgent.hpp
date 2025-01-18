/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NetworkAgent
*/

#pragma once

#include <asio.hpp>
#include <iostream>
#include <cstdint>
#include <thread>
#include <utility>

#include "UDPPacket.hpp"

namespace ntw
{
    struct ClientInformation
    {
        asio::ip::udp::endpoint endpoint;
        std::string name;
        std::uint32_t id = 0;

        explicit ClientInformation(asio::ip::udp::endpoint endpoint, std::string name = "") : endpoint(std::move(endpoint)), name(std::move(name)), id(_lastId++) {}
        ClientInformation(asio::ip::udp::endpoint endpoint, const std::uint32_t& id, std::string name = "") : endpoint(std::move(endpoint)), name(std::move(name)), id(id) {}
        ClientInformation(const ClientInformation &) = default;
        ClientInformation &operator=(const ClientInformation &) = default;

    private:
        static std::uint32_t _lastId;
    };

    inline std::uint32_t ClientInformation::_lastId = 0;

    /**
     * @brief Class representation of an object connected via socket, that can receive and send packets.
     */
    class NetworkAgent
    {
    public:
        /**
         * @brief Constructor for the NetworkAgent class
         * @param port The port to create the agent at
         */
        explicit NetworkAgent(const std::uint32_t& port = 0) : _socket(this->_ctx, asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
        {

            this->_port = this->_socket.local_endpoint().port();
            this->_receivePacket();
            this->_thread = std::thread([this]()
                                        { this->_ctx.run(); });
            std::cout << "NetworkAgent started, listening on port: " << this->_socket.local_endpoint().address().to_string() << ":" << this->_port << "..." << std::endl;
        }

        virtual ~NetworkAgent()
        {
            this->_stop();
        }

        virtual void stop() = 0;

    protected:
        /**
         * @brief Virtual method called when a packet was successfully received
         * @param src Where the packet was received from
         * @param packet The packet received
         */
        virtual void _onPacketReceived(const asio::ip::udp::endpoint &src, UDPPacket &packet) = 0;

        /**
         * @brief Stops any asio work from this agent
         */
        void _stop()
        {
            if (this->_socket.is_open())
                this->_socket.close();
            if (this->_thread.joinable())
                this->_thread.join();
        }

        /**
         * @brief Try to send a message to an endpoint
         * @param dest The endpoint to send to
         * @param packet The packet to send
         */
        void _send(const asio::ip::udp::endpoint& dest, const UDPPacket& packet)
        {
            auto serialized = packet.serialize();
            // std::cout << "Sending packet to " << dest << " (" << serialized.size() << " bytes)" << std::endl;
            // std::cout << "Payload: " << packet.payload << std::endl;
            this->_socket.async_send_to(
                asio::buffer(serialized), dest,
                std::bind(
                    &NetworkAgent::_handleSend, this,
                    asio::placeholders::error,
                    asio::placeholders::bytes_transferred));
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
                    asio::placeholders::bytes_transferred));
        }

        /**
         * @brief Handler used when receiving a packet, will determine if all went according to plan
         * @param e Error code linked to the reception
         * @param bytes The amount of bytes received
         */
        void _handleReceive(const std::error_code &e, const std::size_t& bytes)
        {
            //TODO: handle receive errors
            if (e)
                return;

            if (bytes > 0)
            {
                UDPPacket packet(this->_buffer.data(), bytes);
                uint16_t calculated_checksum = packet.calculateChecksum();
                if (calculated_checksum == packet.checksum)
                {
                    this->_onPacketReceived(this->_client, packet);
                }
                else
                {
                    //TODO: handle corrupted packet
                    std::cerr << "got some corrupted packet :( (checksum mismatch: " << calculated_checksum << " != " << packet.checksum << ")" << std::endl;
                }
            }
            this->_receivePacket();
        }

        /**
         * @brief Handler used after sending a packet
         * @param e Error code linked to the sending
         * @param bytes The amount of bytes sent
         */
        void _handleSend(const std::error_code &e, std::size_t bytes)
        {
            //TODO: handle send errors
        }

    protected:
        std::uint32_t _port = 0;

    private:
        asio::io_context _ctx;
        asio::ip::udp::socket _socket;
        asio::ip::udp::endpoint _client;
        std::array<char, 1024> _buffer{};
        std::thread _thread;
    };
}
