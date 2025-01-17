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

//#include "Systems/PlayerShotSystem.hpp"
#include "Systems/PlayerUpdateSystem.hpp"
#include "Systems/HealthRenderingSystem.hpp"

#include "../../Components/Weapon.hpp"
#include "../../Components/DamageProtection.hpp"
#include "../../Components/Border.hpp"
#include "../../Components/Player.hpp"

#include <engine/RestrictedGame.hpp>
#include <engine/modules/ASceneRenderingModule.hpp>
#include <set>

void Game::initialize()
{
    this->_updateSystems.push_back(std::make_unique<AnimateSystem>(_registry));
}

void Game::update(const double& deltaTime)
{
    _executeUpdateSystems(deltaTime);
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

    const auto topBorder = _registry.create();
    _registry.addComponent(topBorder, Transform{.x = 0, .y = 0, .z = 0});
    _registry.addComponent(topBorder, Border{.width = 598, .height = 155});

    const auto bottomBorder = _registry.create();
    _registry.addComponent(bottomBorder, Transform{.x = 0, .y = 1012 - 50 - 25, .z = 0});
    _registry.addComponent(bottomBorder, Border{.width = 598, .height = 50 + 20});

    const auto leftBorder = _registry.create();
    _registry.addComponent(leftBorder, Transform{.x = 0, .y = 0, .z = 0});
    _registry.addComponent(leftBorder, Border{.width = 50 + 30, .height = 1012});

    const auto rightBorder = _registry.create();
    _registry.addComponent(rightBorder, Transform{.x = 598 - 50 - 30, .y = 0, .z = 0});
    _registry.addComponent(rightBorder, Border{.width = 50 + 30, .height = 1012});

    player = _registry.create();
    _registry.addComponent(player, Sprite("assets/orchera/Factions/Knights/Troops/Archer/Red/Archer_Red.png", {1, 1}, {192 / 2, 192 / 2}, sf::IntRect{0, 0, 192, 192}));
    _registry.addComponent(player, Transform{.x = 322, .y = 736, .z = 0});
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
        .onCollision = [this] (const Entity entity) {
            if (_registry.has_any_of<Monster>(entity) && !_registry.has_any_of<DamageProtection>(entity))
            {
                _registry.get<Health>(player).health -= 10;
                _registry.addComponent(entity, DamageProtection{.duration = 0.3f});
            }
        }
    });
    _registry.addComponent(player, shape::Circle{
        .radius = 5,
        .fillColor = {255, 0, 0},
    });

    _registry.addComponent(player, Weapon{
        .weapon = Bow{
            .shootDelay = 0.5f,
            .shootCooldown = 0,
            .attackSpeed = 0.6f,
            .range = 2000,
            .pierce = 3,
            .damage = 10
        }
    });

    _registry.addComponent(player, Health{.health = 100, .maxHealth = 100});
    _registry.addComponent(player, Player{});

    this->_updateSystems.push_back(std::make_unique<PlayerUpdateSystem>(_registry, player));

    // _registry.addComponent(player, Debug{});

    const auto map = _registry.create();
    _registry.addComponent(map, Sprite("assets/orchera/map.png", {1.4375, 1.4375}));
    _registry.addComponent(map, Transform{.x = 0, .y = 0, .z = -1});

    const auto sceneRenderingModule = this->getModule<ASceneRenderingModule>();
    if (!sceneRenderingModule)
    {
        game::RestrictedGame::instance().stop();
        std::cerr << "No events module found, exiting..." << std::endl;
    }

//    _playerShotSystem = std::make_unique<PlayerShotSystem>(_registry);

    sceneRenderingModule->systems().push_back(std::make_unique<HealthRenderingSystem>(_registry));

    sceneRenderingModule->addHandler(
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
            if (norm == 0)
                return;
            x *= speed / norm;
            y *= speed / norm;
            _registry.removeComponent<Animation>(player);
            _registry.get<Sprite>(player).textureRect = {0, 0, 192, 192};
        }
    );
}

void Game::onExit()
{
}

void Game::onExit(const AScene& nextScene)
{
}
