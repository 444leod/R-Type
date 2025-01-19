/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <Engine/Game.hpp>

#include "PremadeModules/Network/ANetworkSceneModule.hpp"
#include "PremadeModules/Rendering/GameRenderingModule.hpp"
#include "PremadeModules/Audio/GameAudioModule.hpp"

#include "GameModules/NetworkGameModule.hpp"

#include "scenes/Level1/Level1.hpp"
#include "scenes/Level1/Modules/PacketHandlerSceneModule.hpp"

#include "scenes/WaitingRoom/Modules/PacketHandlerSceneModule.hpp"
#include "scenes/WaitingRoom/WaitingRoom.hpp"

#include "Config.hpp"

int main()
{
    auto game = engine::Game();

    game.addModule<GameRenderingModule>(SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE, "R-Type - Client");

    const auto networkGameModule = game.addModule<NetworkGameModule>();

    game.addModule<GameAudioModule>();

    const auto main = game.registerScene<WaitingRoom>("main");
    {
        const auto net = main->addModule<ANetworkSceneModule>(*networkGameModule);
        main->addModule<waiting_room::PacketHandlerSceneModule>(net);
        main->addModule<ASceneRenderingModule>();
        main->addModule<ASceneAudioModule>();
    }

    const auto level1 = game.registerScene<Level1>("game");
    {
        const auto net = level1->addModule<ANetworkSceneModule>(*networkGameModule);
        level1->addModule<level1::PacketHandlerSceneModule>(net);
        level1->addModule<ASceneRenderingModule>();
        main->addModule<ASceneAudioModule>();
    }

    game.run();
    return 0;
}
