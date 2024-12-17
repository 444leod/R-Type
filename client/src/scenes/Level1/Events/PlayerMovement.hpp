/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** PlayerMovement.hpp
*/

#ifndef PLAYERMOVEMENT_HPP
#define PLAYERMOVEMENT_HPP

#include "../Components.hpp"
#include "EventDispatcher.hpp"
#include "Registry.hpp"

#include <SFML/Window/Keyboard.hpp>
#include "Global.hpp"
#include "NetworkAgent.hpp"
#include "ISceneManager.hpp"

class PlayerMovement : public EventHandler<UserInput> {
public:
    explicit PlayerMovement(Registry& registry, ISceneManager& manager) : _registry(registry), _manager(manager) {}
    ~PlayerMovement() override = default;

    void receive(const UserInput& event) override {
        switch (event.key)
        {
            case sf::Keyboard::Key::Up:
            case sf::Keyboard::Key::Down:
            case sf::Keyboard::Key::Left:
            case sf::Keyboard::Key::Right:
            {
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



#endif //PLAYERMOVEMENT_HPP
