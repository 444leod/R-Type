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

#include "BaseSystems/Render/DrawSpritesSystem.hpp"
#include "BaseSystems/Render/DrawTextsSystem.hpp"
#include "../AScene.hpp"
#include "AGameModule.hpp"
#include "ASceneEventsModule.hpp"

namespace ecs {
    class Registry;
}

namespace engine
{

    class GameRenderingModule final : public AGameModule
    {
    public:
        GameRenderingModule(game::RestrictedGame& game, const std::uint32_t& width, const std::uint32_t& height, std::string  title):
            AGameModule(game),
            _title(std::move(title)),
            _mode(width, height),
            _spritesSystem(game.registry()),
            _textsSystem(game.registry())
        {}
        ~GameRenderingModule() override = default;

        void start() override
        {
            this->_window.create(this->_mode, this->_title);
        }

        void refresh(AScene& scene) override
        {
            // Dumbly deactivate / reactivate modules to avoid redering dumb stuff when uneeded
            this->_spritesSystem.disable();
            this->_textsSystem.disable();
            this->_target = scene.getModule<ASceneEventsModule>();
            if (this->_target == nullptr)
                return;
            this->_spritesSystem.enable();
            this->_textsSystem.enable();
        }

        void stop() override
        {
            this->_window.close();
        }

        void update() override
        {
            _window.clear();
            sf::Event event{};
            while (this->_window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    this->_game.stop();
                    return;
                }
                if (this->_target == nullptr)
                    continue;
                this->_target->trigger(event);
            }

            this->_window.clear();
            this->_spritesSystem.execute(this->_window);
            this->_textsSystem.execute(this->_window);
            this->_window.display();
        }

    protected:
    private:
        std::shared_ptr<ASceneEventsModule> _target;

        std::string _title;
        sf::VideoMode _mode;
        sf::RenderWindow _window;

        DrawSpritesSystem _spritesSystem;
        DrawTextsSystem _textsSystem;
    };

}

