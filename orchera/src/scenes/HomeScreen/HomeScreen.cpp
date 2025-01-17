/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include <iostream>

#include "HomeScreen.hpp"
#include "ecs/Registry.hpp"

#include "Components.hpp"

#include "engine/modules/ASceneRenderingModule.hpp"

#include <engine/RestrictedGame.hpp>

void HomeScreen::initialize()
{
}

void HomeScreen::update(const double& deltaTime)
{
}

void HomeScreen::onEnter()
{
    _registry.clear();

    const auto background = _registry.create();
    _registry.addComponent<Transform>(background, Transform{.x = 0, .y = 0, .z = -1});
    _registry.addComponent<Sprite>(background, Sprite("./assets/orchera/background.png", {1.5f, 1.5f}));

    const auto start = _registry.create();
    _registry.addComponent<Transform>(start, Transform{.x = 20, .y = 500, .z = 0});
    _registry.addComponent<Button>(start, Button
    {
        .shape = Sprite{"assets/orchera/UI/Buttons/Button_Blue_3Slides.png", {2.85, 2}},
        .onClick = [&]()
        {
            game::RestrictedGame::instance().scenes().load("game");
        },
        .label = Text{
            .font = "assets/arial.ttf",
            .message = "Start",
            .fontSize = 40,
            .color = Color{255, 255, 255, 255}}
    });

    const auto options = _registry.create();
    _registry.addComponent<Transform>(options, Transform{.x = 20, .y = 650, .z = 0});
    _registry.addComponent<Button>(options, Button
    {
        .shape = Sprite{"assets/orchera/UI/Buttons/Button_Blue_3Slides.png", {2.85, 2}},
        .onClick = [&]()
        {
            // game::RestrictedGame::instance().scenes().load("options");
        },
        .label = Text{
            .font = "assets/arial.ttf",
            .message = "Options",
            .fontSize = 40,
            .color = Color{255, 255, 255, 255}}
    });

    const auto quit = _registry.create();
    _registry.addComponent<Transform>(quit, Transform{.x = 20, .y = 800, .z = 0});
    _registry.addComponent<Button>(quit, Button
    {
        .shape = Sprite{"assets/orchera/UI/Buttons/Button_Blue_3Slides.png", {2.85, 2}},
        .onClick = [&]()
        {
            game::RestrictedGame::instance().stop();
        },
        .label = Text{
            .font = "assets/arial.ttf",
            .message = "Quit",
            .fontSize = 40,
            .color = Color{255, 255, 255, 255}}
    });
}

void HomeScreen::onEnter(const AScene& lastScene)
{
    _registry.clear();
}

void HomeScreen::onExit()
{
}

void HomeScreen::onExit(const AScene& nextScene)
{
}