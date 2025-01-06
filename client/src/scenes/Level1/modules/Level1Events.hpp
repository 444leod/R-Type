/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Level1Events
*/

#pragma once

#include "engine/modules/ASceneEventsModule.hpp"

class Level1Events final : public ASceneEventsModule
{
public:
    Level1Events(AScene& scene): ASceneEventsModule(scene) {}
    virtual ~Level1Events() override = default;

    virtual void onEvent(sf::Event &event)
    {}
};

