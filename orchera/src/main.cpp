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

    game.addModule<engine::GameRenderingModule>(600, 1000, "Orchera");

    const auto homeScreen = game.registerScene<HomeScreen>("home screen");
    {
        homeScreen->addModule<ASceneEventsModule>();
    }


    const auto gameScene = game.registerScene<Game>("game");
    {
        gameScene->addModule<ASceneEventsModule>();
    }

    game.run();
    return 0;
}
