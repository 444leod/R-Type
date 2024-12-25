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
namespace game
{
    class RestrictedGame;
}

class IGameModule
{
public:
    virtual ~IGameModule() = default;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void update(game::RestrictedGame& game) = 0;
};
