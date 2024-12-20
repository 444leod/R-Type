/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Game
*/

#pragma once

#include "Registry.hpp"
#include "EventDispatcher.hpp"
#include "SceneManager.hpp"

class Game
{
public:
    Game() = default;
    ~Game() = default;

    EventDispatcher& eventDispatcher() { return this->_events; }
    const EventDispatcher& eventDispatcher() const { return this->_events; }

    SceneManager& scenes() { return this->_scenes; }
    const SceneManager& scenes() const { return this->_scenes; }

    void run() {
        this->run(this->_scenes.defaultScene());
    }

    void run(const std::string& scene) {
        this->_scenes.load(scene);
        this->_scenes.run();
    }

protected:
    EventDispatcher _events;
    SceneManager _scenes;
private:
};
