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
    _registry.clear();
    const auto text = _registry.create();

    _registry.addComponent(text, Text{.font = "./assets/arial.ttf", .message = "You won!", .fontSize = 30u, .color = Color(31, 81, 255)});
    _registry.addComponent(text, Transform{.x = 100, .y = 100, .z = 1, .rotation = 0});
}

void Win::onExit() {}

void Win::onExit(const AScene& nextScene) {}
