/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Client
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include "NetworkAgent.hpp"

#pragma once

/**
 * @brief Client representation of a network Client, receives updates from a Server and sends it some
 */
class Client: public NetworkAgent
{
public:
    /**
     * @brief Constructor for the Client class
     * @param ctx The io_context to add the client's work to
     */
    Client(asio::io_context& ctx): NetworkAgent(ctx) {}
    ~Client() = default;

    /**
     * @brief Connect to a server & starts the Client's game loop
     * @param host The server's address
     * @param port The server's port
     */
    void run(const std::string& host, std::uint32_t port)
    {
        const auto addr = asio::ip::address::from_string(host);
        this->_server = asio::ip::udp::endpoint(addr, port);
        this->_send(this->_server, "CONNECT");

        this->_window.create(sf::VideoMode::getDesktopMode(), "R-Type");
        this->_window.setFramerateLimit(60);
    }

    /**
     * @brief Stops the client
     */
    void stop()
    {
        this->_stop();
        this->_window.close();
    }

    /**
     * @brief Gets the Client's window
     * @return A reference to the game window
     */
    sf::RenderWindow &window() noexcept { return this->_window; }
    /**
     * @brief Check if the Client is still running
     * @return `true` is the Client is running, `false` otherwise
     */
    bool running() const noexcept { return this->_window.isOpen(); }

private:
    virtual void _onPacketReceived(const asio::ip::udp::endpoint& src, const std::string& msg)
    {
        std::cout << "Received: " << msg << std::endl;
    }

protected:
private:
    asio::ip::udp::endpoint _server;
    bool _running;
    sf::RenderWindow _window;
};
