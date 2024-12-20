/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** InputHandler.hpp
*/

#ifndef PLAYERMOVEMENT_HPP
#define PLAYERMOVEMENT_HPP

#include "../Components.hpp"
#include "../../UserInput.hpp"
#include "EventDispatcher.hpp"
#include "Registry.hpp"
#include "NetworkAgent.hpp"

#include <SFML/Window/Keyboard.hpp>

class InputHandler : public EventHandler<UserInput> {
public:
    explicit InputHandler(Registry& registry) : _registry(registry) {}
    ~InputHandler() = default;

    void receive(const UserInput& event) override {
    }


private:
    Registry& _registry;
};



#endif //PLAYERMOVEMENT_HPP
