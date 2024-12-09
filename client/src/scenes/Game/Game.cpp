/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "Game.hpp"
#include "Registry.hpp"
#include <iostream>
#include <algorithm>

void Game::initialize() {

}

void Game::update(const double deltaTime) {
    _parallaxOffset += static_cast<float>(deltaTime * 25);

    _registry.view<Parallax, Transform>().each([&](const Parallax&, Transform& transform) {
        transform.x = -_parallaxOffset;
    });

    _registry.view<Transform, Velocity>().each([&](Transform& transform, const Velocity& velocity) {
        transform.x += static_cast<float>(velocity.x * deltaTime);
        transform.y += static_cast<float>(velocity.y * deltaTime);
    });

    _registry.view<Projectile, Transform>().each([&](const Entity& entity, const Projectile&, const Transform& transform) {
        if (transform.x > 2200) {
            _registry.remove(entity);
        }
    });
}

void Game::render(sf::RenderWindow& window) {
    auto vec = std::vector<std::tuple<sf::Sprite, Transform>>{};

    _registry.view<sf::Sprite, Transform>().each([&](sf::Sprite &sprite, Transform &transform) {
        vec.emplace_back(sprite, transform);
        sprite.setPosition(transform.x, transform.y);
        sprite.setRotation(transform.rotation);
    });

    std::ranges::sort(vec, [](const auto& a, const auto& b) {
        return std::get<1>(a).z < std::get<1>(b).z;
    });

    for (auto& [sprite, transform] : vec) {
        window.draw(sprite);
    }
}

void Game::onEvent(sf::Event &event) {
    switch (event.type) {
        case sf::Event::KeyPressed:
            switch (event.key.code) {
                case sf::Keyboard::Space:
                    _registry.view<Self, Transform>().each([&](const Self&, const Transform& transform) {
                        addProjectile(transform);
                    });
                    break;
                default:
                    _eventDispatcher.broadcast(movement_event{.key = event.key.code, .pressed = true});
                    break;
            }
            break;
        case sf::Event::KeyReleased:
            switch (event.key.code) {
                default:
                    _eventDispatcher.broadcast(movement_event{.key = event.key.code, .pressed = false});
                    break;
            }
            break;
        case sf::Event::MouseButtonPressed:
            break;
        default:break;
    }
}

void Game::onEnter() {
    _registry.clear();
    const auto spaceship = _registry.create();

    auto spaceshipSprite = sf::Sprite(_spaceshipTex);
    spaceshipSprite.setOrigin(_spaceshipTex.getSize().x / 2, _spaceshipTex.getSize().y / 2);
    spaceshipSprite.setScale(3, 3);

    _registry.addComponent(spaceship, spaceshipSprite);
    _registry.addComponent(spaceship, Transform{.x = 100, .y = 100, .z = 1, .rotation = 0});
    _registry.addComponent(spaceship, Self{});
    _registry.addComponent(spaceship, Hitbox{});
    _registry.addComponent(spaceship, Velocity{.x = 0, .y = 0});

    const auto background = _registry.create();

    auto backgroundSprite = sf::Sprite(_backgroundTex);
    backgroundSprite.setScale(4, 4);

    _registry.addComponent(background, backgroundSprite);
    _registry.addComponent(background, Transform{.x = 0, .y = 0, .z = -1, .rotation = 0});
    _registry.addComponent(background, Parallax{});
}

void Game::onEnter(const AScene& lastScene) {
}

void Game::onExit() {
}

void Game::onExit(const AScene& nextScene) {
}

void Game::addProjectile(const Transform& transform){
    auto projectile = _registry.create();

    auto projectileSprite = sf::Sprite(_projectileTex);
    projectileSprite.setOrigin(_projectileTex.getSize().x / 2, _projectileTex.getSize().y / 2);
    projectileSprite.setScale(2, 2);
    projectileSprite.setPosition(transform.x, transform.y);

    _registry.addComponent(projectile, projectileSprite);

    // random Z between 0 and 1
    float randomZ = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

    _registry.addComponent(projectile, Transform{.x = transform.x, .y = transform.y, .z = randomZ, .rotation = 0});
    _registry.addComponent(projectile, Projectile{});
    _registry.addComponent(projectile, Velocity{.x = 400, .y = 0});
}
