/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameRenderingModule
*/

#pragma once

namespace
{
    class ResourcesManager;
}

#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>
#include <cstdint>

#include "ResourcesManager.hpp"
#include "BaseSystems/Render/DrawSpritesSystem.hpp"
#include "BaseSystems/Render/DrawTextsSystem.hpp"
#include "BaseSystems/Render/DebugDrawSystem.hpp"
#include "BaseSystems/Render/DrawButtonsSystem.hpp"
#include "BaseSystems/Render/DrawShapeSystem.hpp"

#include "BaseSystems/Event/ButtonClickedSystem.hpp"

#include "../AScene.hpp"
#include "AGameModule.hpp"
#include "ASceneRenderingModule.hpp"
#include "engine/Game.hpp"

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
            _spritesSystem(game.registry(), _resourcesManager),
            _textsSystem(game.registry(), _resourcesManager),
            _buttonSystem(game.registry(), _resourcesManager),
            _debugDrawSystem(game.registry()),
            _buttonClickedSystem(game.registry()),
            _shapeSystem(game.registry())
        {}
        ~GameRenderingModule() override = default;

        void start(AScene&) override
        {
            this->_window.create(this->_mode, this->_title);
        }

        void refresh(AScene& scene) override
        {
            // Dumbly deactivate / reactivate modules to avoid redering dumb stuff when uneeded
            this->_spritesSystem.disable();
            this->_textsSystem.disable();
            this->_debugDrawSystem.disable();
            this->_buttonSystem.disable();
            this->_buttonClickedSystem.disable();
            this->_shapeSystem.disable();
            this->_target = scene.getModule<ASceneRenderingModule>();
            if (this->_target == nullptr)
                return;
            this->_spritesSystem.enable();
            this->_textsSystem.enable();
            this->_debugDrawSystem.enable();
            this->_buttonSystem.enable();
            this->_buttonClickedSystem.enable();
            this->_shapeSystem.enable();
        }

        void stop() override
        {
            this->_window.close();
        }

        void update() override
        {
            sf::Event event{};
            while (this->_window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    game::RestrictedGame::instance().stop();
                    return;
                }
                _buttonClickedSystem.execute(event);
                if (this->_target == nullptr)
                    continue;

                this->_target->trigger(event);
            }

            this->_window.clear();
            this->_spritesSystem.execute(this->_window);
            this->_textsSystem.execute(this->_window);
            this->_buttonSystem.execute(this->_window);
            this->_debugDrawSystem.execute(this->_window);
            this->_shapeSystem.execute(this->_window);
            this->_window.display();
        }

    protected:
    private:
        ResourcesManager _resourcesManager;

        std::shared_ptr<ASceneRenderingModule> _target;

        std::string _title;
        sf::VideoMode _mode;
        sf::RenderWindow _window;

        DrawSpritesSystem _spritesSystem;
        DrawTextsSystem _textsSystem;
        DebugDrawSystem _debugDrawSystem;
        DrawButtonsSystem _buttonSystem;
        DrawShapeSystem _shapeSystem;

        ButtonClickedSystem _buttonClickedSystem;
    };

}

