/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <thread>
#include "engine/Game.hpp"
#include "scenes/WaitingRoom/WaitingRoom.hpp"
#include "scenes/Level1/Level1.hpp"
#include "engine/modules/GameRenderingModule.hpp"
#include "Modules/NetworkGameModule.hpp"

#include "scenes/WaitingRoom/Modules/PacketHandlerSceneModule.hpp"

#include "NetworkModules/ANetworkSceneModule.hpp"

int main(void)
{
    auto game = game::Game();

    // game.addModule<engine::GameRenderingModule>(800, 600, "R-Type Server");

    const auto networkGameModule = game.addModule<NetworkGameModule>(25565);

    const auto waitingRoom = game.registerScene<WaitingRoom>("main");

    waitingRoom->addModule<SceneRenderingModule>();
    auto waitingRoomNetwork = waitingRoom->addModule<ANetworkSceneModule>(*networkGameModule);
    waitingRoom->addModule<waiting_room::PacketHandlerSceneModule>(game.registry(), game.scenes(), waitingRoomNetwork);

    const auto level1 = game.registerScene<Level1>("game");

    level1->addModule<SceneRenderingModule>();

    game.run();
    return 0;
}
