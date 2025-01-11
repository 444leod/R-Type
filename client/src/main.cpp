/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "engine/Game.hpp"
#include "scenes/Level1/Level1.hpp"
#include "scenes/WaitingRoom/WaitingRoom.hpp"
#include "engine/modules/GameRenderingModule.hpp"
#include "Modules/NetworkGameModule.hpp"

#include "scenes/WaitingRoom/Modules/PacketHandlerSceneModule.hpp"
#include "scenes/Level1/Modules/PacketHandlerSceneModule.hpp"

#include "NetworkModules/ANetworkSceneModule.hpp"

int main() {
    auto game = game::Game();

    game.addModule<engine::GameRenderingModule>(1700, 900, "R-Type - Client");

    const auto networkGameModule = game.addModule<NetworkGameModule>();

    const auto main = game.registerScene<WaitingRoom>("main");
    main->addModule<ASceneEventsModule>();
    {
        const auto net = main->addModule<ANetworkSceneModule>(*networkGameModule);
        main->addModule<waiting_room::PacketHandlerSceneModule>(game.registry(), game.scenes(), net);
        main->addModule<ASceneEventsModule>();
        // main->addModule<SceneRenderingModule>();
    }


    const auto level1 = game.registerScene<Level1>("game");
    {
        const auto net = level1->addModule<ANetworkSceneModule>(*networkGameModule);
        level1->addModule<level1::PacketHandlerSceneModule>(game.registry(), game.scenes(), net);
        // level1->addModule<SceneRenderingModule>();
        //  level1->addModule<Level1Events>();
    }

    game.run();
    return 0;
}
