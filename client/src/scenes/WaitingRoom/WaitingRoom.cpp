/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "WaitingRoom.hpp"
#include "ecs/Registry.hpp"
#include <iostream>

#include "Components.hpp"

#include "NetworkModules/ANetworkSceneModule.hpp"
#include "engine/modules/ASceneEventsModule.hpp"

void WaitingRoom::initialize()
{
}

void WaitingRoom::update(const double& deltaTime)
{
    if (_registry.view<Client>().size() != 0)
    {

    } else
    {

        if (_connectionAttempts > 5) {
            std::cout << "\nFailed to connect to the server" << std::endl;
            game::RestrictedGame::instance().stop();
            return;
        }

        _connectionTimer += deltaTime;
        _pointTimer += deltaTime;

        if (_connectionTimer > 3) {
            _connectionTimer = 0;
            _connectionAttempts++;
            const auto net = this->getModule<ANetworkSceneModule>();
            net->resend();
        }

        if (_pointTimer > 1) {
            _pointTimer = 0;
            _pointNumber = (_pointNumber + 1) % 4;
            std::cout << "\rConnecting to the server" << std::string(_pointNumber, '.') << std::string(3 - _pointNumber, ' ') << std::flush;
        }
    }
}

/*
void WaitingRoom::render(sf::RenderWindow& window)
{
    if (this->_id.has_value()) {
        sf::Text text;
        text.setFont(_font);
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::White);
        text.setString("Connected to the server");
        text.setPosition(10, 10);
        window.draw(text);
    } else {
        sf::Text text;
        text.setFont(_font);
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::White);
        text.setString("Connecting to the server...");
        text.setPosition(10, 10);
        window.draw(text);
    }
}
*/

/*
void WaitingRoom::onEvent(sf::Event &event)
{
    switch (event.type) {
        case sf::Event::KeyPressed:
            switch (event.key.code) {
                case sf::Keyboard::Space:
                    break;
                case sf::Keyboard::B: {
                     break;
                }
                default:
                    // _eventDispatcher.broadcast(movement_event{.key = event.key.code, .pressed = true});
                    break;
            }
            break;
        case sf::Event::KeyReleased:
            switch (event.key.code) {
                default:
                    // _eventDispatcher.broadcast(movement_event{.key = event.key.code, .pressed = false});
                    break;
            }
            break;
        case sf::Event::MouseButtonPressed:
            break;
        default:break;
    }
}
*/

void WaitingRoom::onEnter() {
    _registry.clear();

    auto events = this->getModule<ASceneEventsModule>();
    if (events != nullptr) {
        events->addHandler(
            [&] (sf::Event& e) {
                return e.type == sf::Event::KeyPressed
                    && e.key.code <= sf::Keyboard::Z
                    && e.key.code >= sf::Keyboard::A;
            },
            [&] (sf::Event& e) {
                std::cout << "Pressed key: " << (char)(e.key.code + 'A') << std::endl;
            }
        );
    } else
        std::cout << "No module for type ASceneEventsModule" << std::endl;

    const auto enttext = _registry.create();
    _registry.addComponent(enttext, Text {
        .message = "Connecting to the server...",
        .font = "./assets/arial.ttf",
        .fontSize = 30,
        .color = { 255, 255, 255 }
    });
    _registry.addComponent(enttext, Position { .x = 10, .y = 10 });

    //_manager.send(SERVER, packet); //dead code
}

void WaitingRoom::onEnter(const AScene& lastScene)
{
}

void WaitingRoom::onExit()
{
}

void WaitingRoom::onExit(const AScene& nextScene)
{
}

/*
void WaitingRoom::onPacketReceived(const asio::ip::udp::endpoint& src, ntw::UDPPacket& packet)
{
    const auto payload = packet.payload;

    // std::cout << "Received: " << payload << " (seq: " << packet.sequence_number
    //           << ", ack: " << packet.ack_number << ")" << std::endl;

    PACKET_TYPE packet_type{};
    packet >> packet_type;
    if (_packet_handlers.contains(packet_type))
        _packet_handlers.at(packet_type)(packet);
} */

/*
void WaitingRoom::send(const ntw::UDPPacket& packet)
{
    _manager.send(SERVER, packet);
}
 */

/* void WaitingRoom::_onConnect(ntw::UDPPacket& packet)
{

    std::uint32_t id;
    packet >> id;

    this->_id = id;
    std::cout << "Connected to the server" << std::endl;
}

void WaitingRoom::_onDisconnect(ntw::UDPPacket& packet)
{
    std::cout << "Disconnection from the server" << std::endl;
    _manager.stop();
}

void WaitingRoom::_onMessage(ntw::UDPPacket& packet)
{
    std::string message;
    packet >> message;
    std::cout << "Message from " << SERVER << ": " << message << std::endl;
}

void WaitingRoom::_onStart(ntw::UDPPacket& packet)
{
    std::cout << "Game starting..." << std::endl;
    _manager.load("Level1");
} */
