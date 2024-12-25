/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RestrictedGame
*/

#pragma once

#include "SceneManager.hpp"
#include "ecs/Registry.hpp"
#include "ecs/EventDispatcher.hpp"

namespace game
{
    class RestrictedGame
    {
    public:
        RestrictedGame() = default;
        virtual ~RestrictedGame() = default;

        [[nodiscard]] virtual const ecs::Registry& registry() const = 0;
        [[nodiscard]] virtual ecs::Registry& registry() = 0;

        [[nodiscard]] virtual const ecs::EventDispatcher& eventDispatcher() const = 0;
        [[nodiscard]] virtual ecs::EventDispatcher& eventDispatcher() = 0;

        [[nodiscard]] virtual const SceneManager& scenes() const = 0;
        [[nodiscard]] virtual SceneManager& scenes() = 0;

        /**
         * @brief Swiftly stops the game from running
         */
        virtual void stop() noexcept = 0;
    };

}