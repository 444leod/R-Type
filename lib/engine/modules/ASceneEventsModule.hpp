/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ASceneEventsModule
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include <vector>
#include "ASceneModule.hpp"

class ASceneEventsModule : public ASceneModule
{
public:
    ASceneEventsModule(AScene& scene): ASceneModule(scene) {}
    ~ASceneEventsModule() override = default;

    virtual void onEvent(sf::Event &event) = 0;
};
