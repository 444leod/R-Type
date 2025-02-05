/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "Win.hpp"

#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Displayable/Text.hpp"
#include <ECS/Registry.hpp>

#include <iostream>

void Win::initialize() {}

void Win::update(const double& deltaTime)
{
}

void Win::onEnter()
{
}

void Win::onEnter(const AScene& lastScene)
{
    std::cout << "Win scene" << std::endl;

    _registry.flush();
    _registry.clear();

    std::cout << "Registry state after clear:" << std::endl;
    _registry.displaySparse();
    const auto text = _registry.create();
    std::cout << "Created text entity: " << text << std::endl;

    try {
        _registry.addComponent(text, Transform{.x = 100, .y = 100, .z = 1, .rotation = 0});
        std::cout << "Added Transform component" << std::endl;
        _registry.addComponent(text, Text{
            .font = "./assets/arial.ttf",
            .message = "You won!",
            .fontSize = 30u,
            .color = Color(31, 81, 255)
        });
        std::cout << "Added Text component" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error adding components: " << e.what() << std::endl;
    }
}

void Win::onExit() {}

void Win::onExit(const AScene& nextScene) {}
