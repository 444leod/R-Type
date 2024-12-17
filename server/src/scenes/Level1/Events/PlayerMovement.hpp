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
#include "NetworkAgent.hpp"

#include <SFML/Window/Keyboard.hpp>

class PlayerMovement : public EventHandler<UserInput> {
public:
    explicit PlayerMovement(Registry& registry) : _registry(registry) {}
    ~PlayerMovement() override = default;

    void receive(const UserInput& event) override {
    }


private:
    Registry& _registry;
};



#endif //PLAYERMOVEMENT_HPP
