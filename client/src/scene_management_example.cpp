/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** scene_management_example.cpp
*/

#include "SceneManager.hpp"

class FirstScene final : public IScene {
public:
    FirstScene(std::string& scene, bool& running) : IScene(scene, running) {}

    void initialize() override
    {
    }

    void update(double deltaTime) override
    {
        const auto now = std::chrono::high_resolution_clock::now();

        if (const auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - _start).count();duration >= 5) {
            _sceneReference = "SecondScene";
        }
    }

    void render(sf::RenderWindow &window) override
    {
    }

    void onEvent(sf::Event &event) override
    {
    }

    void onEnter(const std::string& lastScene) override
    {
        _start = std::chrono::high_resolution_clock::now();
    }

    void onExit(const std::string& nextScene) override
    {
    }
private:
    std::chrono::time_point<std::chrono::system_clock> _start = std::chrono::high_resolution_clock::now();
};

class SecondScene final : public IScene {
public:
    SecondScene(std::string& scene, bool& running) : IScene(scene, running) {}

    void initialize() override
    {
    }

    void update(double deltaTime) override
    {
        const auto now = std::chrono::high_resolution_clock::now();

        if (const auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - _start).count();duration >= 5) {
            _sceneReference = "FirstScene";
        }
    }

    void render(sf::RenderWindow &window) override
    {
    }

    void onEvent(sf::Event &event) override
    {
    }

    void onEnter(const std::string& lastScene) override
    {
        _start = std::chrono::high_resolution_clock::now();
    }

    void onExit(const std::string& nextScene) override
    {
    }

private:
    std::chrono::time_point<std::chrono::system_clock> _start = std::chrono::high_resolution_clock::now();
};

int main(void)
{
    SceneManager sceneManager;

    sceneManager.registerScene<FirstScene>("FirstScene");
    sceneManager.registerScene<SecondScene>("SecondScene");

    sceneManager.run("FirstScene");
    return 0;
}
