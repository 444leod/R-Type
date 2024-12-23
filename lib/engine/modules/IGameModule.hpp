/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** IGameModule
*/

#pragma once

namespace ecs {
    class EventDispatcher;
}
class Game;

class IGameModule
{
public:
    virtual ~IGameModule() = default;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void update(Game &game) = 0;
};
