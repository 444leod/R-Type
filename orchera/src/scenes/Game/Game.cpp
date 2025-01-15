/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "Game.hpp"

#include "Components.hpp"
#include "BaseComponents.hpp"
#include "ecs/Registry.hpp"
#include "Config.hpp"

#include "Systems/PlayerShotSystem.hpp"

#include "../../Components/Weapon.hpp"

#include <engine/RestrictedGame.hpp>
#include <engine/modules/ASceneEventsModule.hpp>
#include <set>

void Game::initialize()
{
    this->_updateSystems.push_back(std::make_unique<AnimateSystem>(_registry));
}

void Game::update(const double& deltaTime)
{
    _executeUpdateSystems(deltaTime);

    timeSinceLastSpawn -= deltaTime;

    if (timeSinceLastSpawn < 0.0f)
    {
        timeSinceLastSpawn += delayBetweenSpawns;

        const auto entity = _registry.create();
        const auto randX = std::rand() % 600;
        const auto randY = std::rand() % 1000;
        // std::cout << "Spawning monster at " << randX << ", " << randY << std::endl;
        _registry.addComponent(entity, Monster{});
        _registry.addComponent(entity, Transform{
            .x = static_cast<float>(randX),
            .y = static_cast<float>(randY),
            .z = 0,
            .rotation = 0
        });
        _registry.addComponent(entity, Hitbox{
            .shape = shape::Circle{
                .radius = 30,
                .fillColor = {0, 255, 255, 80}
            },
            .onCollision = [](const Entity&) {}
        });
        _registry.addComponent(entity, Debug{});
    }

    if (canShoot)
    {
        const auto [x, y, z, rotation] = _registry.get<Transform>(player);
        std::optional<std::pair<std::pair<float, float>, double>> target = std::nullopt;
        for (const auto& [monster, _, monsterPos] : _registry.view<Monster, Transform>().each())
        {
            if (!target.has_value())
            {
                target = {{monsterPos.x, monsterPos.y}, std::sqrt(std::pow(x - monsterPos.x, 2) + std::pow(y - monsterPos.y, 2))};
                continue;
            }
            const auto dist = std::sqrt(std::pow(x - monsterPos.x, 2) + std::pow(y - monsterPos.y, 2));
            if (dist < target->second)
                target = {{monsterPos.x, monsterPos.y}, dist};
        }
        if (target.has_value())
        {
            // _weaponUseSystem->execute(player, {event.mouseButton.x, event.mouseButton.y});
            _playerShotSystem->execute(player, target.value().first, canShoot);
        }
    }
}

void Game::onEnter()
{
    throw std::runtime_error("Game scene should not be entered without a network module");
}

void Game::onEnter(const AScene& lastScene)
{
    _registry.clear<Button>();
    _registry.clear<Transform>();
    _registry.clear<Sprite>();

    player = _registry.create();
    _registry.addComponent(player, Sprite("assets/orchera/Factions/Knights/Troops/Archer/Red/Archer_Red.png", {1, 1}, {192 / 2, 192 / 2}, sf::IntRect{0, 0, 192, 192}));
    _registry.addComponent(player, Transform{.x = 0, .y = 0, .z = 0});
    _registry.addComponent(player, Animation{
        .elapsedTime = 0,
        .frameSize = {192, 192},
        .frameDuration = .20f,
        .frameCount = 6,
        .loop = true,
        .currentFrame = 0,
        .onEnd = [](Entity) {}
    });
    _registry.addComponent<Velocity>(player, Velocity{.x = 0, .y = 0});
    _registry.addComponent<Hitbox>(player, Hitbox{
        .shape = shape::Circle{
            .radius = 30,
            .fillColor = {100, 100, 100, 80}
        },
        .onCollision = [] (Entity) {}
    });
    _registry.addComponent(player, shape::Circle{
        .radius = 5,
        .fillColor = {255, 0, 0},
    });

    _registry.addComponent(player, Weapon{
        .weapon = Bow{
            .shootDelay = 0.5f,
            .shootCooldown = 0,
            .range = 2000,
            .shoot = [](ecs::Registry& registry, const Entity& player, const std::pair<float, float>& target, Bow& bow)
            {}
        }
    });

    _registry.addComponent(player, Debug{});

    const auto events = this->getModule<ASceneEventsModule>();
    if (!events)
    {
        game::RestrictedGame::instance().stop();
        std::cerr << "No events module found, exiting..." << std::endl;
    }

    _playerShotSystem = std::make_unique<PlayerShotSystem>(_registry);

    events->addHandler(
        [](const sf::Event& event)
        {
            return event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased;
        },
        [this](const sf::Event& event)
        {
            constexpr float speed = 60.0f;
            auto&[x, y] = _registry.get<Velocity>(player);

            if (event.type == sf::Event::KeyPressed)
            {
                _pressedKeys.insert(event.key.code);
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                _pressedKeys.erase(event.key.code);
            }

            x = 0.0f;
            y = 0.0f;

            if (_pressedKeys.contains(sf::Keyboard::Up))
                y -= 1;
            if (_pressedKeys.contains(sf::Keyboard::Down))
                y += 1;
            if (_pressedKeys.contains(sf::Keyboard::Left))
                x -= 1;
            if (_pressedKeys.contains(sf::Keyboard::Right))
                x += 1;
            const float norm = std::sqrt(x * x + y * y);
            x *= speed / norm;
            y *= speed / norm;
        }
    );
}

void Game::onExit()
{
}

void Game::onExit(const AScene& nextScene)
{
}
