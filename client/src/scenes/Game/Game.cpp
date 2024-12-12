/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "Game.hpp"
#include "Registry.hpp"
#include <algorithm>
#include <cmath>
#include <config.h>
#include <iostream>

void Game::initialize() {

}

void Game::update(const double deltaTime, const sf::RenderWindow &window) {
    _parallaxOffset += static_cast<float>(deltaTime * 25);

    _registry.view<Parallax, Transform>().each([&](const Parallax&, Transform& transform) {
        transform.x = -_parallaxOffset;
    });

    auto view = _registry.view<Transform, Velocity>();

    view.each([&](const Entity& entity, Transform& transform, const Velocity& velocity) {
        transform.x += static_cast<float>((velocity.x * SCALE) * deltaTime);
        transform.y += static_cast<float>((velocity.y * SCALE) * deltaTime);
    });

    _registry.view<Projectile, Transform>().each([&](const Entity& entity, const Projectile&, const Transform& transform) {
        if (transform.x > window.getSize().x) {
            _registry.remove(entity);
        }
    });

    _registry.view<Animation, sf::Sprite>().each([&](const Entity& entity, Animation & animation, sf::Sprite &sprite) {
        if (animation.currentFrame == animation.frameCount) {
            if (animation.loop) {
                animation.currentFrame = 0;
            } else {
                _registry.removeComponent<Animation>(entity);
                if (animation.velocity.x != 0 || animation.velocity.y != 0)
                    _registry.addComponent(entity, animation.velocity);
                else
                    _registry.remove(entity);
            }
        }
        if (animation.clock.getElapsedTime().asMilliseconds() >= animation.speed) {
            sprite.setTextureRect(sf::IntRect(animation.currentFrame * animation.frameSize.first, 0, animation.frameSize.first, animation.frameSize.second));
            animation.currentFrame++;
            animation.clock.restart();
        }
    });

    _registry.view<Bug, sf::Sprite, Transform>().each([&](const Entity& entity, Bug& bug, sf::Sprite& sprite, Transform& transform) {
        const auto movementFactor = std::sin(bug.clock.getElapsedTime().asSeconds() / .2);
        transform.y += movementFactor * 8;
        transform.rotation = 90 - 45 * movementFactor;
    });

    _registry.view<Enemy, sf::Sprite, Transform>().each([&](const Entity& entity, Enemy&, const sf::Sprite& sprite, const Transform& transform) {
        _registry.view<Projectile, Transform>().each([&](const Entity& projectile, const Projectile&, const Transform& projectileTransform) {
            // FIX HITBOX
            if (sprite.getGlobalBounds().intersects(sf::FloatRect(projectileTransform.x, projectileTransform.y, 16, 16))) {
                const auto explosion = _registry.create();
                auto explosionSprite = sf::Sprite(_explosionTex);
                explosionSprite.setOrigin(16, 16);
                explosionSprite.setScale(SCALE, SCALE);
                explosionSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
                _registry.addComponent(explosion, explosionSprite);
                _registry.addComponent(explosion, Transform{.x = transform.x, .y = transform.y, .z = 1, .rotation = 0});
                _registry.addComponent(explosion, Animation{.frameSize = {32, 32}, .speed = 100, .frameCount = 6, .loop = false});
                _registry.remove(projectile);
                _registry.remove(entity);
            }
        });
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
                case sf::Keyboard::B: {
                    const auto bug = _registry.create();
                    auto bugSprite = sf::Sprite(_bugTex);
                    bugSprite.setOrigin(16, 13);
                    bugSprite.setScale(SCALE, SCALE);
                    bugSprite.setPosition(2000, 250);
                    _registry.addComponent(bug, bugSprite);
                    _registry.addComponent(bug, Bug{});
                    _registry.addComponent(bug, Enemy{});
                    _registry.addComponent(bug, Transform{.x = 2000, .y = 250, .z = 1, .rotation = 90});
                    _registry.addComponent(bug, Velocity{.x = -100, .y = 0});
                    break;
                }
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
    spaceshipSprite.setOrigin(0, 0);
    spaceshipSprite.setScale(SCALE, SCALE);

    _registry.addComponent(spaceship, spaceshipSprite);
    _registry.addComponent(spaceship, Transform{.x = 100, .y = 100, .z = 1, .rotation = 0});
    _registry.addComponent(spaceship, Self{});
    _registry.addComponent(spaceship, Hitbox{});
    _registry.addComponent(spaceship, Velocity{.x = 0, .y = 0});

    const auto background = _registry.create();

    auto backgroundSprite = sf::Sprite(_backgroundTex);
    backgroundSprite.setScale(SCALE, SCALE);

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
    const auto projectile = _registry.create();
    const auto spriteTransform = Transform{.x = transform.x + 33 * SCALE, .y = transform.y + 2 * SCALE, .z = 1, .rotation = 0};

    auto projectileSprite = sf::Sprite(_projectileTex);
    projectileSprite.setOrigin(0, 0);
    projectileSprite.setScale(SCALE, SCALE);
    projectileSprite.setPosition(spriteTransform.x, spriteTransform.y);
    projectileSprite.setTextureRect(sf::IntRect(0, 0, 16, 16));

    _registry.addComponent(projectile, projectileSprite);

    _registry.addComponent(projectile, spriteTransform);
    _registry.addComponent(projectile, Projectile{});
    _registry.addComponent(projectile, Animation{.frameSize = {16, 16}, .speed = 20, .frameCount = 3, .loop = false, .velocity = Velocity{.x = 200, .y = 0}});
}
