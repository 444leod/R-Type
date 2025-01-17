/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RestrictedGame
*/

#pragma once

#include <stdexcept>

// Forward declaration
namespace ecs
{
    class Registry;
    class EventDispatcher;
};

namespace engine
{
    // Forward declaration
    class RestrictedSceneManager;

    class RestrictedGame
    {
    public:
        RestrictedGame()
        {
            if (_instance == nullptr)
                _instance = this;
        };
        virtual ~RestrictedGame() = default;

        static RestrictedGame& instance()
        {
            if (_instance == nullptr)
                throw std::runtime_error("No instance of RestrictedGame available");
            return  *_instance;
        }

        [[nodiscard]] virtual const ecs::Registry& registry() const = 0;
        [[nodiscard]] virtual ecs::Registry& registry() = 0;

        [[nodiscard]] virtual const ecs::EventDispatcher& eventDispatcher() const = 0;
        [[nodiscard]] virtual ecs::EventDispatcher& eventDispatcher() = 0;

        [[nodiscard]] virtual const RestrictedSceneManager& scenes() const = 0;
        [[nodiscard]] virtual RestrictedSceneManager& scenes() = 0;

        /**
         * @brief Swiftly stops the game from running
         */
        virtual void stop() noexcept = 0;

    protected:
        static RestrictedGame *_instance;
    };

    inline RestrictedGame *RestrictedGame::_instance = nullptr;

}