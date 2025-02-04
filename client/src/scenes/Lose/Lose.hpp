/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.hpp
*/

#ifndef LOSE_HPP
#define LOSE_HPP

#include <chrono>
#include <optional>

#include <Engine/AScene.hpp>
#include <ECS/Registry.hpp>

class Lose final : public engine::AScene
{
  public:
    explicit Lose(const std::string& name) : AScene(name) {}

    void initialize() override;

    void update(const double& deltaTime) override;

    void onEnter() override;

    void onEnter(const engine::AScene& lastScene) override;

    void onExit() override;

    void onExit(const engine::AScene& nextScene) override;

  private:
    bool _entered = false;
};

#endif // LOSE_HPP
