/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Game.cpp
*/

#include "Level1.hpp"

#include "PremadeModules/Network/ANetworkGameModule.hpp"
#include "PremadeModules/Network/ANetworkSceneModule.hpp"
#include "PremadeModules/Audio/ASceneAudioModule.hpp"

#include "PremadeComponents/Displayable/Animation.hpp"
#include "PremadeComponents/Hitbox.hpp"
#include "PremadeComponents/Projectile.hpp"
#include "PremadeComponents/Velocity.hpp"

#include "SharedComponents/Enemy.hpp"

#include "Config.hpp"
#include "PacketTypes.hpp"
#include "Sprites/Level1.hpp"

#include <PremadeComponents/Tags/Self.hpp>
#include <PremadeModules/Rendering/ASceneRenderingModule.hpp>
#include <Structures/UserInput.hpp>
#include "PremadeComponents/Displayable/Text.hpp"

void Level1::initialize() {}

void Level1::update(const double& deltaTime)
{    
    _executeUpdateSystems(deltaTime);
    _updateBeamBarSystem.execute(_spaceClock);
}

void Level1::onEnter()
{
    throw std::runtime_error("Game scene should not be entered without a network module");
}

void Level1::onEnter(const AScene& lastScene)
{
    const auto audio = this->getModule<ASceneAudioModule>();
    if (audio != nullptr) {
        audio->playMusic("assets/space.wav", true);
        audio->playSound("assets/laser.wav");
    }

    _registry.clear();
    _lastShotTime = 0.0;  // Initialisation du timer de tir

    const auto background = _registry.create();
    _registry.addComponent(background, backgroundSprite);
    _registry.addComponent(background, Transform{.x = 0, .y = 0, .z = -1, .rotation = 0});
    _registry.addComponent(background, Parallax{.offsetMultiplier = 25});

    const auto beamBar = _registry.create();
    const auto beamBarSkeleton = _registry.create();
    const auto beamText = _registry.create();

    _registry.addComponent(beamBar, BeamBar{});
    _registry.addComponent(beamBar, Transform{.x = SCALE * ((SCREEN_WIDTH - 100) / 2), .y = SCALE * (SCREEN_HEIGHT - 50), .z = 1, .rotation = 0});
    _registry.addComponent(beamBar,
        shape::Rectangle{
            .width = 0,
            .height = 12 * SCALE,
            .fillColor = {31, 81, 255, 255},
            .outlineColor = {0, 0, 0, 0},
            .outlineThickness = 0
        }
    );

    _registry.addComponent(beamBarSkeleton, Transform{.x = SCALE * ((SCREEN_WIDTH - 100) / 2), .y = SCALE * (SCREEN_HEIGHT - 50), .z = 1, .rotation = 0});
    _registry.addComponent(beamBarSkeleton,
        shape::Rectangle{
            .width = 100 * SCALE,
            .height = 12 * SCALE,
            .fillColor = {0, 0, 0, 0},
            .outlineColor = {255, 255, 255, 255},
            .outlineThickness = 2
        }
    );

    _registry.addComponent(beamText, Transform{.x = SCALE * ((SCREEN_WIDTH - 150) / 2), .y = SCALE * (SCREEN_HEIGHT - 50), .z = 1, .rotation = 0});
    _registry.addComponent(beamText, 
        Text{
            .font = "./assets/arial.ttf",
            .message = "Beam",
            .fontSize = 30u,
            .color = Color(31, 81, 255)
        }
    );

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

            const auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
            ).count() / 1000.0;

            // Limiter la cadence de tir à 5 tirs par seconde
            if (currentTime - _lastShotTime < 0.2) {
                this->_spaceClock = nullptr;
                return;
            }
            _lastShotTime = currentTime;

            const std::uint32_t chargePercentage = std::min(
                static_cast<int>(this->_spaceClock->getElapsedTime().asSeconds() * 100), 
                100
            );
            ntw::UDPPacket packet;
            packet << PACKET_TYPE::NEW_PROJECTILE << chargePercentage;

            const auto net = this->getModule<ANetworkSceneModule>();
            if (net == nullptr)
                return;
            net->sendPacket(packet);

            this->_spaceClock = nullptr;
        }
    );
    sceneRenderingModule->addHandler(
        [this] (const sf::Event& e) {
            return (e.type == sf::Event::KeyPressed || e.type == sf::Event::KeyReleased) && 
                   (e.key.code == sf::Keyboard::Up || 
                    e.key.code == sf::Keyboard::Down || 
                    e.key.code == sf::Keyboard::Left || 
                    e.key.code == sf::Keyboard::Right);
        },
        [this] (sf::Event& e) {
            ntw::UDPPacket packet;
            packet << PACKET_TYPE::USER_INPUT << UserInput{.key = e.key.code, .pressed = e.type == sf::Event::KeyPressed};

            const auto net = this->getModule<ANetworkSceneModule>();
            if (net == nullptr)
                return;
            net->sendPacket(packet);
        }
    );
    ntw::UDPPacket readyPacket;
    readyPacket << PACKET_TYPE::LOAD_SCENE;
    const auto net = this->getModule<ANetworkSceneModule>();
    if (net == nullptr)
            return;
    net->sendPacket(readyPacket);
}

void Level1::onExit()
{
    std::cout << "onExit" << std::endl;
    ntw::UDPPacket packet;
    packet << PACKET_TYPE::DISCONNECT;

    const auto net = this->getModule<ANetworkSceneModule>();
    if (net == nullptr)
        return;
    net->sendPacket(packet);
}

void Level1::onExit(const AScene& nextScene) {}

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
