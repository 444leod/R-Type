/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "Level1.hpp"

#include "PremadeModules/Network/ANetworkGameModule.hpp"
#include "PremadeModules/Network/ANetworkSceneModule.hpp"

#include "PremadeComponents/Displayable/Animation.hpp"
#include "PremadeComponents/Hitbox.hpp"
#include "PremadeComponents/Projectile.hpp"
#include "PremadeComponents/Velocity.hpp"

#include "SharedComponents/Enemy.hpp"

#include "Config.hpp"
#include "PacketTypes.hpp"
#include "Sprites/Level1.hpp"

#include "Systems/NewShipSystem.hpp"

#include <PremadeModules/Rendering/ASceneRenderingModule.hpp>

void Level1::initialize() {}

void Level1::update(const double& deltaTime) { _executeUpdateSystems(deltaTime); }

void Level1::onEnter()
{
    throw std::runtime_error("Game scene should not be entered without a network module");
}

void Level1::onEnter(const AScene& lastScene)
{
    _registry.clear();

    const auto background = _registry.create();

    _registry.addComponent(background, backgroundSprite);
    _registry.addComponent(background, Transform{.x = 0, .y = 0, .z = -1, .rotation = 0});
    _registry.addComponent(background, Parallax{.offsetMultiplier = 25});

    const auto sceneRenderingModule = this->getModule<ASceneRenderingModule>();
    if (!sceneRenderingModule)
    {
        engine::RestrictedGame::instance().stop();
        std::cerr << "No events module found, exiting..." << std::endl;
    }
    sceneRenderingModule->addHandler(
        [this] (const sf::Event& e) {
            return e.type == sf::Event::KeyPressed
                && e.key.code == sf::Keyboard::Space;
        },
        [this] (sf::Event& e) {
            if (this->_spaceClock == nullptr)
                this->_spaceClock = std::make_shared<sf::Clock>();
        }
    );
    sceneRenderingModule->addHandler(
        [this] (const sf::Event& e) {
            return e.type == sf::Event::KeyReleased
                && e.key.code == sf::Keyboard::Space;
        },
        [this] (sf::Event& e) {
            if (!this->_spaceClock)
                return;
            const auto chargePercentage = std::min(static_cast<int>(this->_spaceClock->getElapsedTime().asSeconds() * 100), 100);
            this->addProjectile(Transform{.x = 100, .y = 100, .z = 0, .rotation = 0}, chargePercentage);
            this->_spaceClock = nullptr;
        }
    );
}

void Level1::onExit()
{
    ntw::UDPPacket packet;
    packet << PACKET_TYPE::DISCONNECT;

    const auto net = this->getModule<ANetworkSceneModule>();
    if (net == nullptr)
        return;
    net->sendPacket(packet);
}

void Level1::onExit(const AScene& nextScene) {}

void Level1::addProjectile(const Transform& transform, const int charge) const
{
    const auto projectile = _registry.create();

    auto &sprite = _registry.addComponent(projectile, projectileSprite);
    _registry.addComponent(projectile, transform);
    _registry.addComponent(projectile, Projectile{charge > 20, SCALE * SCREEN_WIDTH + 80, 0});
    if (charge <= 20) {
        sprite.textureRect = IntRect(0, 84, 80, 16);
        _registry.addComponent(projectile, Animation{.frameSize = {80, 16}, .frameDuration = .030, .frameCount = 3, .loop = false, .onEnd = [&](ecs::Entity entity){
            _registry.addComponent(entity, Velocity{.x = 200, .y = 0});
        }});
    } else {
        sprite.textureRect = IntRect(0, 84 + 16 * (charge / 20), 80, 16);
        _registry.addComponent(projectile, Animation{.frameSize = {80, 16}, .frameDuration = .050, .frameCount = 2, .loop = true});
        _registry.addComponent(projectile, Velocity{.x = 200, .y = 0});
    }
#if DEBUG
    _registry.addComponent(projectile, Debug{});
#endif
}

void Level1::addBug(const Transform& transform)
{
    const auto bug = _registry.create();
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
