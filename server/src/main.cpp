/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <Engine/Game.hpp>
#include <thread>

// #include "PremadeModules/Rendering/GameRenderingModule.hpp"
#include "GameModules/NetworkGameModule.hpp"

#include "PremadeModules/Network/ANetworkSceneModule.hpp"

#include "scenes/WaitingRoom/Modules/PacketHandlerSceneModule.hpp"
#include "scenes/WaitingRoom/WaitingRoom.hpp"

#include "scenes/Level1/Level1.hpp"
#include "scenes/Level1/Modules/PacketHandlerSceneModule.hpp"

int main()
{
    auto game = engine::Game();

    // game.addModule<Engine::GameRenderingModule>(800, 600, "R-Type Server");

    const auto networkGameModule = game.addModule<NetworkGameModule>();

    const auto waitingRoom = game.registerScene<WaitingRoom>("main");

    // waitingRoom->addModule<SceneRenderingModule>();
    auto waitingRoomNetwork = waitingRoom->addModule<ANetworkSceneModule>(*networkGameModule);
    waitingRoom->addModule<waiting_room::PacketHandlerSceneModule>(waitingRoomNetwork);

    const auto level1 = game.registerScene<Level1>("game");

    // level1->addModule<SceneRenderingModule>();
    auto level1Network = level1->addModule<ANetworkSceneModule>(*networkGameModule);
    level1->addModule<level1::PacketHandlerSceneModule>(level1Network);

    game.run();


    std::vector<int> a = {1, 2, 3, 4, 5};
    std::find(a.begin(), a.end(), 8);
    
    return 0;
}
