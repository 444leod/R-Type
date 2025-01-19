/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.hpp
*/

#ifndef HOME_SCREEN_HPP
#define HOME_SCREEN_HPP

#include <chrono>
#include <memory>

#include <Engine/AScene.hpp>
#include <ECS/Registry.hpp>

// Game specific
#include <SFML/Window/Keyboard.hpp>
#include <set>

class HomeScreen final : public engine::AScene {
public:
    HomeScreen(const std::string& name) : AScene(name)
    {
    }

    void initialize() override;

    void update(const double& deltaTime) override;

    void onEnter() override;

    void onEnter(const AScene& lastScene) override;

    void onExit() override;

    void onExit(const AScene& nextScene) override;

private:

public:
private:
};



#endif //HOME_SCREEN_HPP
