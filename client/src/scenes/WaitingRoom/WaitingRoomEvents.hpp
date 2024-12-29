/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** WaitingRoomEvents
*/

#pragma once

#include "engine/modules/ISceneEventsModule.hpp"

class WaitingRoomEvents final : public ISceneEventsModule
{
public:
    WaitingRoomEvents() = default;
    virtual ~WaitingRoomEvents() override = default;

    virtual void onEvent(sf::Event &event)
    {}
};
