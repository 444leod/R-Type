/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** InputHandler.hpp
*/

#ifndef PLAYERMOVEMENT_HPP
#define PLAYERMOVEMENT_HPP

#include "../Components.hpp"
#include "EventDispatcher.hpp"
#include "Registry.hpp"

#include "Global.hpp"
#include "ISceneManager.hpp"
#include "NetworkAgent.hpp"
#include <SFML/Window/Keyboard.hpp>

class InputHandler : public EventHandler<UserInput> {
  public:
    explicit InputHandler(Registry& registry, ISceneManager& manager) : _registry(registry), _manager(manager) {}
    ~InputHandler() override = default;

    void receive(const UserInput& event) override {
        if (event.key == sf::Keyboard::Key::Space && event.pressed) {
            UDPPacket packet;
            packet << PACKET_TYPE::USER_INPUT << event;
            _manager.send(SERVER, packet);
            return;
        }
        switch (event.key) {
        case sf::Keyboard::Key::Up:
        case sf::Keyboard::Key::Down:
        case sf::Keyboard::Key::Left:
        case sf::Keyboard::Key::Right: {
            UDPPacket packet;
            packet << PACKET_TYPE::USER_INPUT << event;
            _manager.send(SERVER, packet);
        }
        default:
            break;
        }
    }

  private:
    Registry& _registry;
    ISceneManager& _manager;
};

#endif // PLAYERMOVEMENT_HPP
