/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.hpp
*/

#ifndef HOME_SCREEN_HPP
#define HOME_SCREEN_HPP

#include <chrono>
#include <optional>

#include "ecs/Registry.hpp"
#include "ecs/EventDispatcher.hpp"
#include "engine/AScene.hpp"
#include "BaseComponents.hpp"

class HomeScreen final : public AScene {
public:
    HomeScreen(RestrictedSceneManager& m, ecs::Registry& r, const std::string& n) : AScene(m, r, n)
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
