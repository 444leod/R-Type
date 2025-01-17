/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "engine/Game.hpp"
#include "scenes/Game/Game.hpp"
#include "scenes/HomeScreen/HomeScreen.hpp"
#include "engine/modules/GameRenderingModule.hpp"

int main() {
    auto game = game::Game();

    game.addModule<engine::GameRenderingModule>(598, 1012, "Orchera");

    const auto homeScreen = game.registerScene<HomeScreen>("home screen");
    {
        homeScreen->addModule<ASceneRenderingModule>();
    }


    const auto gameScene = game.registerScene<Game>("game");
    {
        gameScene->addModule<ASceneRenderingModule>();
    }

    game.run();
    return 0;
}
