/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** PlayerMovement.hpp
*/

#ifndef PLAYERMOVEMENT_HPP
#define PLAYERMOVEMENT_HPP

#include "EventDispatcher.hpp"
#include "Registry.hpp"
#include "../Components.hpp"
#include <SFML/Window/Keyboard.hpp>

struct movement_event : public IEvent {
    sf::Keyboard::Key key;
    bool pressed;
};

class PlayerMovement : public EventHandler<movement_event> {
public:
    explicit PlayerMovement(Registry& registry) : _registry(registry) {}
    ~PlayerMovement() override = default;

    void receive(const movement_event& event) override {
        std::cout << "PlayerMovement received event" << std::endl;
        _registry.view<Self, Velocity>().each([&](const Self&, Velocity& velocity) {
            switch (event.key) {
                case sf::Keyboard::Key::Up:
                    velocity.y += event.pressed ? -200 : 200;
                    break;
                case sf::Keyboard::Key::Down:
                    velocity.y += event.pressed ? 200 : -200;
                    break;
                case sf::Keyboard::Key::Left:
                    velocity.x += event.pressed ? -200 : 200;
                    break;
                case sf::Keyboard::Key::Right:
                    velocity.x += event.pressed ? 200 : -200;
                    break;
                default:
                    break;
            }
        });
    }


private:
    Registry& _registry;
};



#endif //PLAYERMOVEMENT_HPP
