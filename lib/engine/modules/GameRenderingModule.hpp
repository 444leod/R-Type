/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameRenderingModule
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>

#include "../AScene.hpp"
#include "IGameModule.hpp"
#include "SceneRenderingModule.hpp"

namespace ecs {
    class Registry;
}

class GameRenderingModule : public IGameModule
{
public:
    GameRenderingModule(std::uint32_t width, std::uint32_t height, const std::string& title) : _title(title), _mode(width, height) {}
    ~GameRenderingModule() = default;

    void start() override
    {
        this->_window.create(this->_mode, this->_title);
    }

    void stop() override
    {
        this->_window.close();
    }

    virtual void update(Game &game) override
    {
        auto *target_module = game.scenes().current().getModule<SceneRenderingModule>();

        _window.clear();
        sf::Event event;
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
