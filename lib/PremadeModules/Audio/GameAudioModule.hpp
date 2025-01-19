/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameAudioModule
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <utility>
#include <vector>
#include <cstdint>

#include "Engine/AScene.hpp"
#include "Engine/Modules/AGameModule.hpp"
#include "ASceneAudioModule.hpp"

class GameAudioModule final : public engine::AGameModule
{
public:
    GameAudioModule() {}
    ~GameAudioModule() override = default;

    void start(engine::AScene& scene) override {}

    void refresh(engine::AScene& scene) override
    {
        std::cout << "Playing music in GameModule " << std::endl;
        _sceneAudioModule = scene.getModule<ASceneAudioModule>();
        _sceneAudioModule->playMusic("assets/space.wav");
    }

    void stop() override {}

    void update() override {
        _sceneAudioModule->playSound("assets/laser.wav");
        _sceneAudioModule->update();
    }

protected:
private:
    std::shared_ptr<ASceneAudioModule> _sceneAudioModule;
};

