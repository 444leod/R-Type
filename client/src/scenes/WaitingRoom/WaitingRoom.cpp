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
    if (_registry.view<Client>().size() != 0 && this->_connecting)
    {
        this->_connecting = false;
        this->_connected = true;
        auto &text = _registry.get<Text>(this->_statusTextId);
        text.message = "                 Connected !\nWaiting for server to start the game";
        text.color = Color(0, 255, 0);
    }
    else if (this->_connecting)
    {

        if (_connectionAttempts > 5)
        {
            auto &text = _registry.get<Text>(this->_statusTextId);
            text.message = "Failed to connect to the server";
            text.color = Color(255, 0, 0);

            _registry.view<Input>().each([&](Input& input)
            {
                input.disabled = false;
            });
            this->_connecting = false;
            this->_connected = false;
            _connectionAttempts = 0;
            _connectionTimer = 0;
            _pointTimer = 0;
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
            auto &text = _registry.get<Text>(this->_statusTextId);
            text.message = "Connecting to the server" + std::string(_pointNumber, '.');
            text.color = Color(255, 255, 255);
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

    const auto inputIp = _registry.create();
    _registry.addComponent<Input>(
        inputIp, Input{
            .box = shape::Rectangle{
                .width = 250,
                .height = 50,
                .fillColor = Color(0, 111, 255),
                .outlineColor = Color(0, 100, 100),
                .outlineThickness = 5
            },
            .maxLength = 15,
            .placeHolder = "127.0.0.1",
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
    
    _registry.addComponent(inputIp, Transform{.x = 350, .y = 500});

    const auto ipText = _registry.create();
    _registry.addComponent(ipText, Text{.font = "./assets/arial.ttf", .message = "Server IP", .fontSize = 30u, .color = Color(255, 255, 255)});
    _registry.addComponent(ipText, Transform{.x = 425, .y = 445});

    const auto inputPort = _registry.create();
    _registry.addComponent<Input>(
        inputPort, Input{
            .box = shape::Rectangle{
                .width = 100,
                .height = 50,
                .fillColor = Color(0, 111, 255),
                .outlineColor = Color(0, 100, 100),
                .outlineThickness = 5
            },
            .maxLength = 5,
            .placeHolder = "25565",
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

    _registry.addComponent(inputPort, Transform{.x = 650, .y = 500});

    const auto portText = _registry.create();
    _registry.addComponent(portText, Text{.font = "./assets/arial.ttf", .message = "PORT", .fontSize = 30u, .color = Color(255, 255, 255)});
    _registry.addComponent(portText, Transform{.x = 660, .y = 445});

    const auto inputName = _registry.create();
    _registry.addComponent<Input>(
        inputName, Input{
            .box = shape::Rectangle{
                .width = 250,
                .height = 50,
                .fillColor = Color(0, 111, 255),
                .outlineColor = Color(0, 100, 100),
                .outlineThickness = 5
            },
            .maxLength = 20,
            .placeHolder = "Player",
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

    _registry.addComponent(inputName, Transform{.x = 800, .y = 500});

    const auto nameText = _registry.create();
    _registry.addComponent(nameText, Text{.font = "./assets/arial.ttf", .message = "User Name", .fontSize = 30u, .color = Color(255, 255, 255)});
    _registry.addComponent(nameText, Transform{.x = 850, .y = 445});

    const auto statusText = _registry.create();
    _registry.addComponent(statusText, Text{.font = "./assets/arial.ttf", .message = "", .fontSize = 29u, .color = Color(255, 255, 255)});
    _registry.addComponent(statusText, Transform{.x = 560, .y = 200});

    this->_statusTextId = statusText;

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

    const auto button = _registry.create();
    _registry.addComponent<Button>(
        button, Button{
            .shape = shape::Rectangle(200, 50, Color(100, 100, 100), Color(0, 0, 0), 2),
            .onClick = [this]
            {
                std::string ipAddress;
                std::uint32_t port;
                std::string userName;

                _registry.view<Input>().each([&](Input& input)
                {
                    if (input.placeHolder == "127.0.0.1")
                    {
                        ipAddress = !input.value.empty() ? input.value : input.placeHolder;
                        input.disabled = true;
                    }
                    else if (input.placeHolder == "25565")
                    {
                        port = !input.value.empty() ? std::stoi(input.value) : std::stoi(input.placeHolder);
                        input.disabled = true;
                    }
                    else if (input.placeHolder == "Player")
                    {
                        userName = !input.value.empty() ? input.value : input.placeHolder;
                        input.disabled = true;
                    }
                });
                const auto net = this->getModule<ANetworkSceneModule>();
                if (net == nullptr)
                    return;
                net->connect(ipAddress, port, userName);
                this->_connecting = true;
                auto &text = _registry.get<Text>(this->_statusTextId);
                text.message = "Connecting to the server" + std::string(_pointNumber, '.');
                text.color = Color(255, 255, 255);
            },
            .label = Text{.font = "./assets/arial.ttf", .message = "Connect", .fontSize = 30u, .color = Color(255, 255, 255)}
        }
    );
    _registry.addComponent(button, Transform{.x = 100, .y = 100, .rotation = 20});
}

void WaitingRoom::onEnter(const AScene& lastScene) {}

void WaitingRoom::onExit() {}

void WaitingRoom::onExit(const AScene& nextScene) {}
