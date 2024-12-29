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

int main(void)
{
    auto game = game::Game();

    // game.addModule<engine::GameRenderingModule>(800, 600, "R-Type Server");

    game.addModule<NetworkGameModule>(25565);

    game.registerScene<WaitingRoom>("main");
    game.registerScene<Level1>("game");
    game.run();
    return 0;
}
