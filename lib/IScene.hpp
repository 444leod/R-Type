/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** IScene.hpp
*/

#ifndef ISCENE_HPP
#define ISCENE_HPP

#include <SFML/Graphics.hpp>

class IScene {
public:
    virtual ~IScene() = default;

    virtual void initialize() = 0;
    virtual void update(double deltaTime) = 0;
    virtual void render(sf::RenderWindow &window) = 0;

    virtual void onEvent(sf::Event &event) = 0;
    virtual void onEnter(const std::string& lastScene) = 0;
    virtual void onExit(const std::string& nextScene) = 0;

protected:
    IScene(std::string& scene, bool& running) : _sceneReference(scene), _running(running) {}

    std::string& _sceneReference;
    bool& _running;
};



#endif //ISCENE_HPP
