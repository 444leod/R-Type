/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameRenderingModule
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>
#include <cstdint>

#include "../AScene.hpp"
#include "IGameModule.hpp"
#include "SceneRenderingModule.hpp"

namespace ecs {
    class Registry;
}

namespace engine
{

    class GameRenderingModule final : public IGameModule
    {
    public:
        GameRenderingModule(const std::uint32_t& width, const std::uint32_t& height, std::string  title) : _title(std::move(title)), _mode(width, height) {}
        ~GameRenderingModule() override = default;

        void start() override
        {
            this->_window.create(this->_mode, this->_title);
        }

        void stop() override
        {
            this->_window.close();
        }

        void update(game::RestrictedGame &game) override
        {
            const auto *target_module = game.scenes().current().getModule<SceneRenderingModule>();

            _window.clear();
            sf::Event event{};
            while (this->_window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    game.stop();
                    return;
                }
                if (target_module == nullptr)
                    continue;
            }
            // call system with registry
            _window.display();
        }

    protected:
    private:
        std::string _title;
        sf::VideoMode _mode;
        sf::RenderWindow _window;
        std::vector<sf::Event> _events;
    };

}

