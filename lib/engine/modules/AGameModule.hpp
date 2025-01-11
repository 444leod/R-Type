/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AGameModule
*/

#pragma once

namespace ecs {
    class EventDispatcher;
}
namespace game
{
    class RestrictedGame;
}

class AScene;

class AGameModule
{
public:
    explicit AGameModule(game::RestrictedGame& game): _game(game) {}
    virtual ~AGameModule() = default;

    virtual void start() = 0;
    virtual void refresh(AScene& scene) = 0;
    virtual void stop() = 0;
    virtual void update() = 0;

protected:
    game::RestrictedGame& _game;
};
