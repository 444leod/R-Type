/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include <algorithm>
#include <cmath>
#include <config.h>
#include <iostream>
#include <ranges>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "Level1.hpp"
#include "Registry.hpp"
#include "NetworkAgent.hpp"
#include "Global.hpp"

void Level1::initialize() {}

void Level1::update(const double deltaTime, const sf::RenderWindow &window) {
    _sysParalax.execute(deltaTime, window);

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
                if (animation.velocity.x != 0 || animation.velocity.y != 0)
                    _registry.addComponent(entity, animation.velocity);
                else
                    _registry.remove(entity);
                _registry.removeComponent<Animation>(entity);
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

    _registry.view<Enemy, sf::Sprite, Transform>().each([&](const Entity& enemy, const Enemy&e_enemy, const sf::Sprite& sprite, const Transform& transform)  {
        _registry.view<Projectile, Transform>().each([&](const Entity& projectile, const Projectile&p_projectile, const Transform& projectile_transform) {
            if (!sprite.getGlobalBounds().intersects(sf::FloatRect(projectile_transform.x, projectile_transform.y, 16, 16)))
                return;

            UDPPacket packet;
            packet << PACKET_TYPE::MONSTER_KILLED << e_enemy.id << p_projectile.id;
            for (auto client: CLIENTS)
                _manager.send(client.endpoint, packet);

            const auto explosion = _registry.create();
            auto explosionSprite = sf::Sprite(_explosionTex);
            explosionSprite.setOrigin(16, 16);
            explosionSprite.setScale(SCALE, SCALE);
            explosionSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
            explosionSprite.setPosition(projectile_transform.x, projectile_transform.y);
            _registry.addComponent(explosion, explosionSprite);
            _registry.addComponent(explosion, Transform{.x = projectile_transform.x, .y = projectile_transform.y, .z = 1, .rotation = 0});
            _registry.addComponent(explosion, Animation{.frameSize = {32, 32}, .speed = 100, .frameCount = 6, .loop = false});
            #if DEBUG
                _registry.addComponent(explosion, Debug{});
            #endif

            _registry.remove(enemy);
            _registry.remove(projectile);
        });
    });

    if (this->_bugTimer < 0)
    {
        this->_bugTimer = 5.f;
        this->addBug(Transform{.x = 2000, .y = 250, .z = 1, .rotation = 90});
    }

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
                default:
                    _eventDispatcher.broadcast(UserInput{.key = event.key.code, .pressed = true});
                    break;
            }
            break;
        case sf::Event::KeyReleased:
            switch (event.key.code) {
                default:
                    _eventDispatcher.broadcast(UserInput{.key = event.key.code, .pressed = false});
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
}

void Level1::onEnter(const AScene& lastScene)
{
    for (const auto& client : CLIENTS) {
        std::cout << "Client " << client.id << " connected" << std::endl;
        std::cout << "Endpoint: " << client.endpoint << std::endl;

        _manager.send(client.endpoint, UDPPacket{} << PACKET_TYPE::MESSAGE << "Welcome to the game!");
    }

    _registry.clear();

    auto spaceshipSprite = sf::Sprite(_spaceshipTex);
    spaceshipSprite.setOrigin(0, 0);
    spaceshipSprite.setScale(SCALE, SCALE);

    for (const auto& client : CLIENTS) {
        const auto spaceship = _registry.create();
        _registry.addComponent(spaceship, spaceshipSprite);
        _registry.addComponent(spaceship, Transform{.x = 100, .y = 100, .z = 1, .rotation = 0});
        _registry.addComponent(spaceship, Ship{ .id = client.id });
        _registry.addComponent(spaceship, Hitbox{});
        _registry.addComponent(spaceship, Velocity{.x = 0, .y = 0});
        #if DEBUG
            _registry.addComponent(spaceship, Debug{});
        #endif

        {
            UDPPacket packet;
            packet << PACKET_TYPE::NEW_SHIP << client.id;
            for (const auto& subclient : CLIENTS) {
                if (subclient.id == client.id)
                    continue;
                _manager.send(subclient.endpoint, packet);
            }
        }

        UDPPacket self_ship;
        self_ship << PACKET_TYPE::YOUR_SHIP << client.id;
        _manager.send(client.endpoint, self_ship);
    }

    const auto background = _registry.create();

    auto backgroundSprite = sf::Sprite(_backgroundTex);
    backgroundSprite.setScale(SCALE, SCALE);

    _registry.addComponent(background, backgroundSprite);
    _registry.addComponent(background, Transform{.x = 0, .y = 0, .z = -1, .rotation = 0});
    _registry.addComponent(background, Parallax{.offsetMultiplier = 25});
}

void Level1::onExit()
{
    UDPPacket packet;
    packet << PACKET_TYPE::DISCONNECT;

    for (const auto& client : CLIENTS) {
        _manager.send(client.endpoint, packet);
    }
}

void Level1::onExit(const AScene& nextScene)
{
}

void Level1::onPacketReceived(const asio::ip::udp::endpoint& src, UDPPacket& packet) {
        auto type = PACKET_TYPE{};
        packet >> type;

        auto begin = CLIENTS.begin();

        while (begin != CLIENTS.end()) {
            if (begin->endpoint == src) {
                break;
            }
            begin++;
        }

        if (begin == CLIENTS.end()) {
            return;
        }

        _eventDispatcher.broadcast(PacketInformations{.type = type, .packet = packet, .source = *begin});
}

void Level1::addBug(const Transform& transform) {

    const auto bug = _registry.create();
    auto bugSprite = sf::Sprite(_bugTex);
    bugSprite.setOrigin(16, 13);
    bugSprite.setScale(SCALE, SCALE);
    bugSprite.setPosition(transform.x, transform.y);
    _registry.addComponent(bug, bugSprite);
    _registry.addComponent(bug, Bug{});
    _registry.addComponent(bug, Enemy{ .id = _enemyId });
    _registry.addComponent(bug, transform);
    _registry.addComponent(bug, Velocity{.x = -100, .y = 0});
    _registry.addComponent(bug, Hitbox{});
    #if DEBUG
        _registry.addComponent(bug, Debug{});
    #endif

    UDPPacket packet;
    packet << PACKET_TYPE::NEW_MONSTER << _enemyId << transform;
    for (auto client: CLIENTS)
        _manager.send(client.endpoint,  packet);
    _enemyId++;
}
