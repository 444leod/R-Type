/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SceneEventsModule
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include <vector>
#include "ISceneModule.hpp"

class ISceneEventsModule : public ISceneModule
{
public:
    ~ISceneEventsModule() override = default;

    virtual void onEvent(sf::Event &event) = 0;
};
