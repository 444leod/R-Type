/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "WaitingRoom.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <ranges>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "network/NetworkAgent.hpp"
#include "ecs/Registry.hpp"
#include "PacketTypes.hpp"
#include "engine/modules/ASceneEventsModule.hpp"

void WaitingRoom::initialize()
{
}

void WaitingRoom::update(const double& deltaTime)
{
    this->_executeUpdateSystems(deltaTime);
}

/*
void WaitingRoom::render(sf::RenderWindow& window)
{
    if (this->_id.has_value()) {
        sf::Text text;
        text.setFont(_font);
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::White);
        text.setString("Connected to the server");
        text.setPosition(10, 10);
        window.draw(text);
    } else {
        sf::Text text;
        text.setFont(_font);
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::White);
        text.setString("Connecting to the server...");
        text.setPosition(10, 10);
        window.draw(text);
    }
}
*/

/*
void WaitingRoom::onEvent(sf::Event &event)
{
    switch (event.type) {
        case sf::Event::KeyPressed:
            switch (event.key.code) {
                case sf::Keyboard::Space:
                    break;
                case sf::Keyboard::B: {
                     break;
                }
                default:
                    // _eventDispatcher.broadcast(movement_event{.key = event.key.code, .pressed = true});
                    break;
            }
            break;
        case sf::Event::KeyReleased:
            switch (event.key.code) {
                default:
                    // _eventDispatcher.broadcast(movement_event{.key = event.key.code, .pressed = false});
                    break;
            }
            break;
        case sf::Event::MouseButtonPressed:
            break;
        default:break;
    }
}
*/

void WaitingRoom::onEnter() {
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

    auto enttext = _registry.create();
    _registry.addComponent(enttext, Text {
        .message = "Connecting to the server...",
        .font = "./assets/arial.ttf",
        .fontSize = 30,
        .color = Color( 255, 255, 255 )
    });
    _registry.addComponent(enttext, Position { .x = 10, .y = 10 });

    //_manager.send(SERVER, packet); //dead code
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