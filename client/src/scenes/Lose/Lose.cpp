/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "Lose.hpp"
#include <Network/UDPPacket.hpp>

#include "PremadeComponents/Transform.hpp"
#include "PremadeComponents/Displayable/Text.hpp"
#include "PremadeModules/Network/ANetworkSceneModule.hpp"

#include "PacketTypes.hpp"
#include <ECS/Registry.hpp>

#include <iostream>

void Lose::initialize() {}

void Lose::update(const double& deltaTime)
{
//    if (!_entered) {
//    }
}

void Lose::onEnter()
{
}

void Lose::onEnter(const AScene& lastScene)
{
        std::cout << "Lose scene" << std::endl;
        _registry.clear();
        _registry.flush();
        const auto text = _registry.create();

        _registry.addComponent(text, Text{.font = "./assets/arial.ttf", .message = "You lost...", .fontSize = 30u, .color = Color(31, 81, 255)});
        _registry.addComponent(text, Transform{.x = 100, .y = 100, .z = 1, .rotation = 0});
        _entered = true;
}

void Lose::onExit() {}

void Lose::onExit(const AScene& nextScene) {}
