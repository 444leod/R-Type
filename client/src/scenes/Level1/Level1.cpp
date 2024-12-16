/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "Level1.hpp"
#include "Registry.hpp"
#include <algorithm>
#include <cmath>
#include <config.h>
#include <ranges>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

sf::Font get_default_font() {
    sf::Font font;
    font.loadFromFile("assets/arial.ttf");
    return font;
}

static const auto font = get_default_font();

void Level1::initialize() {}

void Level1::update(const double deltaTime) {
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
        if (transform.x > WINDOW_WIDTH * SCALE) {
            _registry.remove(entity);
        }
    });

    _registry.view<Animation, sf::Sprite>().each([&](const Entity& entity, Animation & animation, sf::Sprite &sprite) {
        if (animation.currentFrame == animation.frameCount) {
            if (animation.loop) {
                animation.currentFrame = 0;
            } else {
                if (animation.velocity.x != 0 || animation.velocity.y != 0)
                    _registry.addComponent(entity, animation.velocity);
                else
                    _registry.remove(entity);
                _registry.removeComponent<Animation>(entity);
            }
        }
        if (animation.clock.getElapsedTime().asMilliseconds() >= animation.speed) {
            std::cout << "FrameOrigin : (" << animation.frameOrigin.first << ", " << animation.frameOrigin.second << "), FrameSize : (" << animation.frameSize.first << ", " << animation.frameSize.second << ")" << std::endl;
            sprite.setTextureRect(sf::IntRect(animation.frameOrigin.first + animation.currentFrame * animation.frameSize.first, animation.frameOrigin.second, animation.frameSize.first, animation.frameSize.second));
            animation.currentFrame++;
            animation.clock.restart();
        }
    });

    _registry.view<Bug, sf::Sprite, Transform>().each([&](const Entity& entity, Bug& bug, sf::Sprite& sprite, Transform& transform) {
        const auto movementFactor = std::sin(bug.clock.getElapsedTime().asSeconds() / .2);
        transform.y += movementFactor * 8;
        transform.rotation = 90 - 45 * movementFactor;
    });

    _registry.view<Enemy, sf::Sprite, Transform>().each([&](const Entity& enemy, const Enemy&, const sf::Sprite& sprite, const Transform& transform)  {
        _registry.view<Projectile, Transform>().each([&](const Entity& projectile, const Projectile&, const Transform& projectile_transform) {
            if (!sprite.getGlobalBounds().intersects(sf::FloatRect(projectile_transform.x, projectile_transform.y, 16, 16)))
                return;

            const auto explosion = _registry.create();
            auto explosionSprite = sf::Sprite(_explosionTex);
            explosionSprite.setOrigin(16, 16);
            explosionSprite.setScale(SCALE, SCALE);
            explosionSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
            _registry.addComponent(explosion, explosionSprite);
            _registry.addComponent(explosion, Transform{.x = projectile_transform.x, .y = projectile_transform.y, .z = 1, .rotation = 0});
            _registry.addComponent(explosion, Animation{.frameSize = {32, 32}, .frameOrigin = {0, 0}, .speed = 100, .frameCount = 6, .loop = false});
            #if DEBUG
                _registry.addComponent(explosion, Debug{});
            #endif

            _registry.remove(enemy);
            _registry.remove(projectile);
        });
    });

    _registry.view<sf::RectangleShape>().each([&](const Entity& entity, sf::RectangleShape &rectangle) {
        int fillPercent = 0;
        if (this->_spaceClock != nullptr)
            fillPercent = std::min(static_cast<int>(this->_spaceClock->getElapsedTime().asSeconds() * 100), 100);
        rectangle.setSize(sf::Vector2f(fillPercent, 25));
    });

    // auto explosions = _registry.view<Animation, sf::Sprite, Transform>();
    // explosions.displaySets();
}

void Level1::render(sf::RenderWindow& window) {
    auto vec = std::vector<std::tuple<Entity, sf::Sprite, Transform>>{};

    _registry.view<sf::Sprite, Transform>().each([&](const Entity& entity, sf::Sprite &sprite, Transform &transform) {
        vec.emplace_back(entity, sprite, transform);
        sprite.setPosition(transform.x, transform.y);
        sprite.setRotation(transform.rotation);
    });

    std::ranges::sort(vec, [](const auto& a, const auto& b) {
        return std::get<2>(a).z < std::get<2>(b).z;
    });

    for (const auto& [entity, sprite, _] : vec) {
        window.draw(sprite);
    }

    _registry.view<sf::RectangleShape, Transform>().each([&](const Entity& entity, sf::RectangleShape &rectangle, Transform &transform) {
        rectangle.setPosition(transform.x, transform.y);
        rectangle.setRotation(transform.rotation);
        window.draw(rectangle);
    });

    // _registry.view<Hitbox, sf::Sprite>().each([&](const Hitbox&, const sf::Sprite& sprite) {
    //     auto bounds = sprite.getGlobalBounds();
    //     sf::RectangleShape hitbox(sf::Vector2f(bounds.width, bounds.height));
    //     hitbox.setPosition(sprite.getPosition());
    //     hitbox.setRotation(sprite.getRotation());
    //     hitbox.setOutlineColor(sf::Color::Red);
    //     hitbox.setOutlineThickness(1);
    //     // hitbox.setOrigin(sprite.getOrigin());
    //     hitbox.setFillColor(sf::Color::Transparent);
    //     window.draw(hitbox);
    // });

    #if DEBUG
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);
    _registry.view<Debug, Transform>().each([&](const Entity& entity, const Debug&, const Transform& transform) {
        text.setString(std::to_string(entity));
        text.setPosition(transform.x, transform.y);
        window.draw(text);
    });
    #endif
}

void Level1::onEvent(sf::Event &event) {
    switch (event.type) {
        case sf::Event::KeyPressed:
            switch (event.key.code) {
                case sf::Keyboard::Space:
                    if (this->_spaceClock == nullptr) {
                        this->_spaceClock = std::make_unique<sf::Clock>();
                    }
                    break;
                case sf::Keyboard::B: {
                     addBug(Transform{.x = 2000, .y = 250, .z = 1, .rotation = 90});
                     break;
                }
                default:
                    _eventDispatcher.broadcast(movement_event{.key = event.key.code, .pressed = true});
                    break;
            }
            break;
        case sf::Event::KeyReleased:
            switch (event.key.code) {
                case sf::Keyboard::Space:
                    if (this->_spaceClock != nullptr) {
                        _registry.view<Self, Transform>().each([&](const Self&, const Transform& transform) {
                            addProjectile(Transform{.x = transform.x + 33 * SCALE, .y = transform.y + 2 * SCALE, .z = 1, .rotation = 0}, std::min(static_cast<int>(this->_spaceClock->getElapsedTime().asSeconds() * 100), 100));
                        });
                        this->_spaceClock = nullptr;
                    }
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

void Level1::onEnter() {
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
    #if DEBUG
        _registry.addComponent(spaceship, Debug{});
    #endif

    const auto background = _registry.create();

    auto backgroundSprite = sf::Sprite(_backgroundTex);
    backgroundSprite.setScale(SCALE, SCALE);

    _registry.addComponent(background, backgroundSprite);
    _registry.addComponent(background, Transform{.x = 0, .y = 0, .z = -1, .rotation = 0});
    _registry.addComponent(background, Parallax{});
    const auto beamLoading = _registry.create();

    auto beamRectangle = sf::RectangleShape(sf::Vector2f(25, 25));
    beamRectangle.setFillColor(sf::Color::Green);
    beamRectangle.setPosition(0, 0);
    beamRectangle.setOrigin(0, 0);
    beamRectangle.setScale(SCALE, SCALE);
    _registry.addComponent(beamLoading, beamRectangle);
    _registry.addComponent(beamLoading, Transform{.x = 0, .y = 0, .z = 1, .rotation = 0});
    #if DEBUG
        _registry.addComponent(beamLoading, Debug{});
    #endif
}

void Level1::onEnter(const AScene& lastScene) {
}

void Level1::onExit() {
}

void Level1::onExit(const AScene& nextScene) {
}

void Level1::addProjectile(const Transform& transform, const int charge){
    const auto projectile = _registry.create();

    auto projectileSprite = sf::Sprite(_projectileTex);
    projectileSprite.setOrigin(0, 0);
    projectileSprite.setScale(SCALE, SCALE);
    projectileSprite.setPosition(transform.x, transform.y);
    _registry.addComponent(projectile, Hitbox{});
    _registry.addComponent(projectile, transform);
    _registry.addComponent(projectile, Projectile{});
    std::cout << "Charge : " << charge << std::endl;
    if (charge <= 20) {
        projectileSprite.setTextureRect(sf::IntRect(0, 0, 80, 16));
        _registry.addComponent(projectile, Animation{.frameSize = {80, 16}, .frameOrigin = {0, 0}, .speed = 20, .frameCount = 3, .loop = false, .velocity = Velocity{.x = 200, .y = 0}});
    } else {
        const int chargeValue = charge / 20;
        const std::pair frameOrigin = {0, 16 * chargeValue};
        projectileSprite.setTextureRect(sf::IntRect(frameOrigin.first, frameOrigin.second, 80, 16));
        _registry.addComponent(projectile, Animation{.frameSize = {80, 16}, .frameOrigin = frameOrigin, .speed = 75, .frameCount = 2, .loop = true});
        _registry.addComponent(projectile, Velocity{.x = 200, .y = 0});
    }
    _registry.addComponent(projectile, projectileSprite);
    #if DEBUG
        _registry.addComponent(projectile, Debug{});
    #endif
}

void Level1::addBug(const Transform& transform) {
    const auto bug = _registry.create();
    auto bugSprite = sf::Sprite(_bugTex);
    bugSprite.setOrigin(16, 13);
    bugSprite.setScale(SCALE, SCALE);
    bugSprite.setPosition(transform.x, transform.y);
    _registry.addComponent(bug, bugSprite);
    _registry.addComponent(bug, Bug{});
    _registry.addComponent(bug, Enemy{});
    _registry.addComponent(bug, transform);
    _registry.addComponent(bug, Velocity{.x = -100, .y = 0});
    _registry.addComponent(bug, Hitbox{});
    #if DEBUG
        _registry.addComponent(bug, Debug{});
    #endif
}