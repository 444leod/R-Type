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

int main(void) {
    auto game = Game();

    game.addModule<GameRenderingModule>(800, 600, "R-Type");
    game.registerScene<WaitingRoom>("main");
    game.registerScene<Level1>("game");
    game.run();
    return 0;
}
