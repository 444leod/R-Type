/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** WaitingRoomEvents
*/

#pragma once

#include "engine/modules/ASceneEventsModule.hpp"

class WaitingRoomEvents final : public ASceneEventsModule
{
public:
    WaitingRoomEvents(AScene& scene): ASceneEventsModule(scene) {}
    virtual ~WaitingRoomEvents() override = default;

    virtual void onEvent(sf::Event &event)
    {
        if (event.type == sf::Event::KeyPressed)
            std::cout << "Key Pressed: " << (char)(event.key.code + 'A') << std::endl;
    }
};
