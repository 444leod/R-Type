/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** NetworkScene
*/

#pragma once

#include "AScene.hpp"
#include "NetworkAgent.hpp"

class NetworkedScene : public virtual NetworkAgent, public virtual AScene {
  public:
    /**
     * @brief Constructor for the NetworkScene class
     * @param manager The scene manager
     * @param name The name of the scene
     * @param io_context The asio io_context
     * @param port The port to create the network agent at
     */
    NetworkedScene(ISceneManager& manager, const std::string& name, asio::io_context& io_context, std::uint32_t port = 0) : AScene(manager, name), NetworkAgent(io_context, port) {}

    virtual ~NetworkedScene() = default;

    virtual void initialize() = 0;
    virtual void update(double deltaTime, const sf::RenderWindow& window) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void onEvent(sf::Event& event) = 0;
    virtual void onEnter() = 0;
    virtual void onEnter(const AScene& lastScene) = 0;
    virtual void onExit() = 0;
    virtual void onExit(const AScene& nextScene) = 0;

    virtual void _onPacketReceived(const asio::ip::udp::endpoint& src, UDPPacket& packet) = 0;
};
