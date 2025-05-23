/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameRenderingModule
*/

#pragma once

class ResourcesManager;

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <utility>

#include "ResourcesManager.hpp"


#include "PremadeSystems/Render/DebugDrawSystem.hpp"
#include "PremadeSystems/Render/DrawInputsSystem.hpp"
#include "PremadeSystems/Render/DrawButtonsSystem.hpp"
#include "PremadeSystems/Render/DrawShapeSystem.hpp"
#include "PremadeSystems/Render/DrawSpritesSystem.hpp"
#include "PremadeSystems/Render/DrawTextsSystem.hpp"

#include "PremadeSystems/Event/ButtonClickedSystem.hpp"

#include "ASceneRenderingModule.hpp"
#include "Engine/AScene.hpp"
#include "Engine/Game.hpp"
#include "Engine/Modules/AGameModule.hpp"

namespace ecs
{
class Registry;
}

class GameRenderingModule final : public engine::AGameModule
{
  public:
    GameRenderingModule(const std::uint32_t& width, const std::uint32_t& height, std::string title)
        : _title(std::move(title)), _mode(width, height), _spritesSystem(_resourcesManager), _textsSystem(_resourcesManager), _buttonSystem(_resourcesManager), _inputSystem(_resourcesManager), _debugDrawSystem(), _buttonClickedSystem(), _shapeSystem()
    {
    }
    ~GameRenderingModule() override = default;

    void start(engine::AScene&) override { this->_window.create(this->_mode, this->_title); }

    void refresh(engine::AScene& scene) override
    {
        this->_target = scene.getModule<ASceneRenderingModule>();
        if (this->_target == nullptr)
            return;
    }

    void stop() override { this->_window.close(); }

    void update() override
    {
        sf::Event event{};
        while (this->_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                engine::RestrictedGame::instance().stop();
                return;
            }
            _buttonClickedSystem.execute(event);
            if (this->_target == nullptr)
                continue;

            this->_target->trigger(event);
        }

        this->_window.clear();
        // TODO: Should find a cool way of letting the user decide on the ordering of those.
        this->_spritesSystem.execute(this->_window);
        this->_textsSystem.execute(this->_window);
        this->_buttonSystem.execute(this->_window);
        this->_inputSystem.execute(this->_window);
        this->_debugDrawSystem.execute(this->_window);
        this->_shapeSystem.execute(this->_window);
        // Execute additional scene systems
        if (this->_target != nullptr)
            this->_target->executeSystems(this->_window);
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
    DrawInputsSystem _inputSystem;
    DebugDrawSystem _debugDrawSystem;
    DrawButtonsSystem _buttonSystem;
    DrawShapeSystem _shapeSystem;

    ButtonClickedSystem _buttonClickedSystem;
};
