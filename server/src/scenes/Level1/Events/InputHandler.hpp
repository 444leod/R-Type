/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** InputHandler.hpp
*/

#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include "ecs/EventDispatcher.hpp"
#include "ecs/Registry.hpp"

#include "Structures/UserInput.hpp"

class InputHandler final : public ecs::EventHandler<UserInput> {
public:
    explicit InputHandler(ecs::Registry& registry) : _registry(registry) {}
    ~InputHandler() override = default;

    void receive(const UserInput& event) override {
    }


private:
    ecs::Registry& _registry;
};



#endif //INPUT_HANDLER_HPP
