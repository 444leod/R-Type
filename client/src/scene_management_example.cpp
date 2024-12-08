/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** scene_management_example.cpp
*/

#include <iostream>
#include "SceneManager.hpp"

class FirstScene final : public AScene {
public:
    FirstScene(SceneManager& m, const std::string& n) : AScene(m, n) {}

    void initialize() override
    {
    }

    void update(double deltaTime) override
    {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - _start).count();

        if (duration >= 5) {
            this->_manager.load("SecondScene");
        }
    }

    void render(sf::RenderWindow &window) override {}

    void onEvent(sf::Event &event) override {}

    void onEnter() override
    {
        std::cout << "Entered " << this->name() << std::endl;
        this->_start = std::chrono::high_resolution_clock::now();
    }
    void onEnter(const AScene& lastScene) override { this->onEnter(); }

    void onExit() override
    {
        std::cout << "Exiting from " << this->name() << "..." << std::endl;
    }
    void onExit(const AScene& nextScene) override { this->onExit(); }
private:
    std::chrono::time_point<std::chrono::system_clock> _start = std::chrono::high_resolution_clock::now();
};

class SecondScene final : public AScene {
public:
    SecondScene(SceneManager& m, const std::string& n) : AScene(m, n) {}

    void initialize() override
    {
    }

    void update(double deltaTime) override
    {
        const auto now = std::chrono::high_resolution_clock::now();

        if (const auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - _start).count();duration >= 5) {
            this->_manager.load("FirstScene");
        }
    }

    void render(sf::RenderWindow &window) override
    {
    }

    void onEvent(sf::Event &event) override
    {
    }

    void onEnter() override
    {
        std::cout << "Entered " << this->name() << std::endl;
        this->_start = std::chrono::high_resolution_clock::now();
    }
    void onEnter(const AScene& lastScene) override { this->onEnter(); }

    void onExit() override
    {
        std::cout << "Exiting from " << this->name() << "..." << std::endl;
    }
    void onExit(const AScene& nextScene) override { this->onExit(); }

private:
    std::chrono::time_point<std::chrono::system_clock> _start = std::chrono::high_resolution_clock::now();
};

int main(void)
{
    SceneManager sceneManager;

    sceneManager.registerScene<FirstScene>("FirstScene");
    sceneManager.registerScene<SecondScene>("SecondScene");

    sceneManager.load("FirstScene");
    sceneManager.run();
    return 0;
}
