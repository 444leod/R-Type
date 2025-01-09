/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** InputHandler.hpp
*/

#ifndef PLAYERMOVEMENT_HPP
#define PLAYERMOVEMENT_HPP

#include "../Components.hpp"
#include "Structures/UserInput.hpp"
#include "ecs/EventDispatcher.hpp"
#include "ecs/Registry.hpp"

#include <SFML/Window/Keyboard.hpp>
#include "network/NetworkAgent.hpp"
#include "engine/RestrictedSceneManager.hpp"
#include "engine/RestrictedGame.hpp"
#include "PacketTypes.hpp"

#include <NetworkModules/ANetworkSceneModule.hpp>

class InputHandler final : public ecs::EventHandler<UserInput> {
public:
    explicit InputHandler(ecs::Registry& registry, RestrictedSceneManager& manager) : _registry(registry), _manager(manager) {}
    ~InputHandler() override = default;

    void receive(const UserInput& event) override {
        if (event.key == sf::Keyboard::Key::Space && event.pressed)
        {
            ntw::UDPPacket packet;
            packet << PACKET_TYPE::USER_INPUT << event;
            game::RestrictedGame::instance().scenes().current().getModule<ANetworkSceneModule>()->queuePacket(packet);
            return;
        }
        switch (event.key)
        {
            case sf::Keyboard::Key::Up:
            case sf::Keyboard::Key::Down:
            case sf::Keyboard::Key::Left:
            case sf::Keyboard::Key::Right:
            {
                ntw::UDPPacket packet;
                packet << PACKET_TYPE::USER_INPUT << event;
                game::RestrictedGame::instance().scenes().current().getModule<ANetworkSceneModule>()->queuePacket(packet);

            }
            default:
                break;
        }
    }


private:
    ecs::Registry& _registry;
    RestrictedSceneManager& _manager;
};



#endif //PLAYERMOVEMENT_HPP
