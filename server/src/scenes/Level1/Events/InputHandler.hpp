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
#include "ecs/EventDispatcher.hpp"
#include "ecs/Registry.hpp"
#include "network/NetworkAgent.hpp"

#include <SFML/Window/Keyboard.hpp>

class InputHandler : public ecs::EventHandler<UserInput> {
public:
    explicit InputHandler(ecs::Registry& registry) : _registry(registry) {}
    ~InputHandler() = default;

    void receive(const UserInput& event) override {
    }


private:
    ecs::Registry& _registry;
};



#endif //PLAYERMOVEMENT_HPP
