/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NetworkAgent
*/

#pragma once

#include <asio.hpp>
#include <cstdint>
#include <iostream>
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
    ClientInformation(const ClientInformation&) = default;
    ClientInformation& operator=(const ClientInformation&) = default;

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
        this->_thread = std::thread([this]() { this->_ctx.run(); });
        std::cout << "NetworkAgent started, listening on port: " << this->_socket.local_endpoint().address().to_string() << ":" << this->_port << "..." << std::endl;
    }

    virtual ~NetworkAgent() { this->_stop(); }

    virtual void stop() = 0;

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
        auto compressed = packet.compress();
        if (compressed.empty())
        {
            // Uncompressed packet
            // std::cout << "Sending uncompressed packet" << std::endl;
            auto serialized = packet.serialize();
            this->_socket.async_send_to(asio::buffer(serialized), dest, std::bind(&NetworkAgent::_handleSend, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
            return;
        }

        // std::cout << "Sending compressed packet" << std::endl;
        // Add magic number (0xA0CD) and compressed size
        std::vector<std::byte> finalPacket;
        const uint32_t magicNumber = 0xA0CD;
        const uint32_t compressedSize = compressed.size();

        finalPacket.insert(finalPacket.end(), reinterpret_cast<const std::byte*>(&magicNumber), reinterpret_cast<const std::byte*>(&magicNumber) + sizeof(magicNumber));

        finalPacket.insert(finalPacket.end(), reinterpret_cast<const std::byte*>(&compressedSize), reinterpret_cast<const std::byte*>(&compressedSize) + sizeof(compressedSize));
        finalPacket.insert(finalPacket.end(), compressed.begin(), compressed.end());

        this->_socket.async_send_to(asio::buffer(finalPacket), dest, std::bind(&NetworkAgent::_handleSend, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
    }

  private:
    /**
     * @brief Private method used to start receiving a packet
     */
    void _receivePacket() { this->_socket.async_receive_from(asio::buffer(this->_buffer), this->_client, std::bind(&NetworkAgent::_handleReceive, this, asio::placeholders::error, asio::placeholders::bytes_transferred)); }

    /**
     * @brief Handler used when receiving a packet, will determine if all went according to plan
     * @param e Error code linked to the reception
     * @param bytes The amount of bytes received
     */
    void _handleReceive(const std::error_code& e, const std::size_t& bytes)
    {
        if (e || bytes == 0)
            return;

        uint32_t magicNumber = 0;
        std::memcpy(&magicNumber, this->_buffer.data(), sizeof(magicNumber));

        bool isCompressed = (magicNumber == 0xA0CD && bytes > sizeof(magicNumber) + sizeof(uint32_t));

        if (!isCompressed)
        {
            UDPPacket packet(this->_buffer.data(), bytes);
            if (packet.calculateChecksum() == packet.checksum)
            {
                this->_onPacketReceived(this->_client, packet);
            }
            else
            {
                std::cerr << "Checksum mismatch for uncompressed packet" << std::endl;
            }
        }
        else
        {
            uint32_t compressedSize;
            std::memcpy(&compressedSize, this->_buffer.data() + sizeof(magicNumber), sizeof(compressedSize));

            UDPPacket packet;
            if (packet.decompress(reinterpret_cast<const std::byte*>(this->_buffer.data() + sizeof(magicNumber) + sizeof(compressedSize)), compressedSize))
            {
                if (packet.calculateChecksum() == packet.checksum)
                {
                    this->_onPacketReceived(this->_client, packet);
                }
                else
                {
                    std::cerr << "Checksum mismatch for compressed packet" << std::endl;
                    std::cerr << "Got: " << packet.calculateChecksum() << " Expected: " << packet.checksum << std::endl;
                }
            }
        }

        this->_receivePacket();
    }

    /**
     * @brief Handler used after sending a packet
     * @param e Error code linked to the sending
     * @param bytes The amount of bytes sent
     */
    void _handleSend(const std::error_code& e, std::size_t bytes)
    {
        // TODO: handle send errors
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
} // namespace ntw
