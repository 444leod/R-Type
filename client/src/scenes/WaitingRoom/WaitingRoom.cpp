/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "WaitingRoom.hpp"

#include "PremadeComponents/Displayable/Input.hpp"

#include <ECS/Registry.hpp>

#include "PremadeModules/Network/ANetworkSceneModule.hpp"
#include "PremadeModules/Rendering/ASceneRenderingModule.hpp"
#include "PremadeModules/Audio/ASceneAudioModule.hpp"

#include "PremadeComponents/Displayable/Button.hpp"
#include "PremadeComponents/Displayable/Text.hpp"
#include "PremadeComponents/Transform.hpp"

#include "SharedComponents/Client.hpp"

#include <iostream>

void WaitingRoom::initialize() {}

void WaitingRoom::update(const double& deltaTime)
{
    if (_registry.view<Client>().size() != 0)
    {
    }
    else
    {

        if (_connectionAttempts > 5)
        {
            std::cout << "\nFailed to connect to the server" << std::endl;
            engine::RestrictedGame::instance().stop();
            return;
        }

        _connectionTimer += deltaTime;
        _pointTimer += deltaTime;

        if (_connectionTimer > 3)
        {
            _connectionTimer = 0;
            _connectionAttempts++;
            const auto net = this->getModule<ANetworkSceneModule>();
            net->resend();
        }

        if (_pointTimer > 1)
        {
            _pointTimer = 0;
            _pointNumber = (_pointNumber + 1) % 4;
            std::cout << "\rConnecting to the server" << std::string(_pointNumber, '.') << std::string(3 - _pointNumber, ' ') << std::flush;
        }
    }
    this->_executeUpdateSystems(deltaTime);
}

void WaitingRoom::onEnter()
{
    const auto audio = this->getModule<ASceneAudioModule>();
    if (audio != nullptr) {
        audio->playMusic("assets/elevator.wav", true);
    }

    _registry.clear();

    if (this->getModule<ASceneRenderingModule>() == nullptr)
        throw std::runtime_error("No rendering module found");

    const auto enttext = _registry.create();
    _registry.addComponent(enttext, Text{.font = "./assets/arial.ttf", .message = "Connecting to the server...", .fontSize = 30u, .color = Color(255, 255, 255)});
    _registry.addComponent(enttext, Transform{.x = 10, .y = 10});

    const auto button = _registry.create();
    _registry.addComponent<Button>(button, Button{.shape = shape::Rectangle(200, 50, Color(100, 100, 100), Color(0, 0, 0), 2),
                                               .onClick = []() { std::cout << "Button clicked" << std::endl; },
                                               .label = Text{.font = "./assets/arial.ttf", .message = "Click me", .fontSize = 30u, .color = Color(255, 255, 255)}});

    _registry.addComponent(button, Transform{.x = 100, .y = 100, .rotation = 20});

    const auto input = _registry.create();
    _registry.addComponent<Input>(
        input, Input{
            .box = shape::Rectangle{
                .width = 250,
                .height = 50,
                .fillColor = Color(0, 111, 255),
                .outlineColor = Color(0, 100, 100),
                .outlineThickness = 5
            },
            .maxLength = 69,
            .placeHolder = "000.000.000.000",
            .value = "",
            .focused = Color(255, 0, 0),
            .notFocused = Color(0, 100, 100),
            .text = Text{
                .font = "./assets/arial.ttf",
                .message = "",
                .fontSize = 30u,
                .color = Color(255, 255, 255)
            },
            .disabled = false,
            .active = false,
        }
    );

    _registry.addComponent(input, Transform{.x = 100, .y = 150, .rotation = 20});

    const auto sceneRenderingModule = this->getModule<ASceneRenderingModule>();
    if (!sceneRenderingModule)
    {
        engine::RestrictedGame::instance().stop();
        std::cerr << "No events module found, exiting..." << std::endl;
    }

    sceneRenderingModule->addHandler(
        [this] (const sf::Event& e) {
            return e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left;
        },
        [this] (sf::Event& e) {
            _inputActivateSystem.execute(e);
        }
    );

    sceneRenderingModule->addHandler(
        [this] (const sf::Event& e) {
            return e.type == sf::Event::KeyPressed;
        },
        [this] (sf::Event& e) {
            _inputTypeSystem.execute(e);
        }
    );
}

void WaitingRoom::onEnter(const AScene& lastScene) {}

void WaitingRoom::onExit() {}

void WaitingRoom::onExit(const AScene& nextScene) {}
