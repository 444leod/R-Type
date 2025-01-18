/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** AGameModule
*/

#pragma once

namespace engine {
// Forward declaration
class AScene;
class RestrictedGame;

class AGameModule {
  public:
    explicit AGameModule() = default;
    virtual ~AGameModule() = default;

    virtual void start(AScene& scene) = 0;
    virtual void refresh(AScene& scene) = 0;
    virtual void stop() = 0;
    virtual void update() = 0;
};
} // namespace engine
