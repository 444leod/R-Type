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
        if (_sceneAudioModule != nullptr) {
            _sceneAudioModule->stop();
        }
        _sceneAudioModule = scene.getModule<ASceneAudioModule>();
    }

    void stop() override {
        if (_sceneAudioModule != nullptr) {
            _sceneAudioModule->stop();
        }
    }

    void update() override {
        if (_sceneAudioModule != nullptr) {
            _sceneAudioModule->update();
        }
    }

protected:
private:
    std::shared_ptr<ASceneAudioModule> _sceneAudioModule;
};

