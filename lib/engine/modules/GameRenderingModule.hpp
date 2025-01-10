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
#include "AGameModule.hpp"
#include "SceneRenderingModule.hpp"

namespace ecs {
    class Registry;
}

namespace engine
{

    class GameRenderingModule final : public AGameModule
    {
    public:
        GameRenderingModule(game::RestrictedGame& game, const std::uint32_t& width, const std::uint32_t& height, std::string  title) : AGameModule(game),  _title(std::move(title)), _mode(width, height) {}
        ~GameRenderingModule() override = default;

        void start(AScene&) override
        {
            this->_window.create(this->_mode, this->_title);
        }

        void stop() override
        {
            this->_window.close();
        }

        void refresh(AScene& scene) override
        {
            const auto target_module = _game.scenes().current().getModule<SceneRenderingModule>();

            _sceneRenderingModule = target_module;
        }

        void update() override
        {
            _window.clear();
            sf::Event event{};
            while (this->_window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    _game.stop();
                    return;
                }
                if (_sceneRenderingModule == nullptr)
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

        std::shared_ptr<SceneRenderingModule> _sceneRenderingModule;
    };

}

