/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "WaitingRoom.hpp"
#include "ecs/Registry.hpp"
#include <iostream>

#include "Components.hpp"

#include "NetworkModules/ANetworkSceneModule.hpp"
#include "engine/modules/ASceneEventsModule.hpp"

void WaitingRoom::initialize()
{
}

void WaitingRoom::update(const double& deltaTime)
{
    if (_registry.view<Client>().size() != 0)
    {

    } else
    {

        if (_connectionAttempts > 5) {
            std::cout << "\nFailed to connect to the server" << std::endl;
            game::RestrictedGame::instance().stop();
            return;
        }

        _connectionTimer += deltaTime;
        _pointTimer += deltaTime;

        if (_connectionTimer > 3) {
            _connectionTimer = 0;
            _connectionAttempts++;
            const auto net = this->getModule<ANetworkSceneModule>();
            net->resend();
        }

        if (_pointTimer > 1) {
            _pointTimer = 0;
            _pointNumber = (_pointNumber + 1) % 4;
            std::cout << "\rConnecting to the server" << std::string(_pointNumber, '.') << std::string(3 - _pointNumber, ' ') << std::flush;
        }
    }
    this->_executeUpdateSystems(deltaTime);
}

void WaitingRoom::onEnter() {
    _registry.clear();

    auto events = this->getModule<ASceneEventsModule>();
    if (events != nullptr) {
        events->addHandler(
            [&] (sf::Event& e) {
                return e.type == sf::Event::KeyPressed
                    && e.key.code <= sf::Keyboard::Z
                    && e.key.code >= sf::Keyboard::A;
            },
            [&] (sf::Event& e) {
                std::cout << "Pressed key: " << (char)(e.key.code + 'A') << std::endl;
            }
        );
    } else
        std::cout << "No module for type ASceneEventsModule" << std::endl;

    const auto enttext = _registry.create();
    _registry.addComponent(enttext, Text {
        .font = "./assets/arial.ttf",
        .message = "Connecting to the server...",
        .fontSize = 30u,
        .color = Color( 255, 255, 255 )
    });
    _registry.addComponent(enttext, Position { .x = 10, .y = 10 });



    const auto button = _registry.create();
    _registry.addComponent<Button>(button, Button {
        .shape = shape::Rectangle(200, 50, Color(100, 100, 100), Color(0, 0, 0), 2),
        .onClick = [] () { std::cout << "Button clicked" << std::endl; },
        .label = Text{
            .font = "./assets/arial.ttf",
            .message = "Click me",
           .fontSize = 30u,
           .color = Color(255, 255, 255)
        }
    });

    _registry.addComponent(button, Transform { .x = 100, .y = 100, .rotation = 20 });
}

void WaitingRoom::onEnter(const AScene& lastScene)
{
}

void WaitingRoom::onExit()
{
}

void WaitingRoom::onExit(const AScene& nextScene)
{
}