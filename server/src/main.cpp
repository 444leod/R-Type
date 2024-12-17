/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <thread>
// #include "Server.hpp"
#include "SceneManager.hpp"
#include "scenes/WaitingRoom/WaitingRoom.hpp"

    // asio::io_context io_context;
    // SceneManager sceneManager(io_context);

    // sceneManager.registerScene<Level1>("Game");

    // sceneManager.load("Game");
    // sceneManager.run();

int main(void)
{
    asio::io_context ctx;
    std::unique_ptr<std::thread> t;
    try
    {
        SceneManager sceneManager(ctx);
        t = std::make_unique<std::thread>([&ctx](){
            ctx.run();
        });
        sceneManager.registerScene<WaitingRoom>("WaitingRoom");
        sceneManager.load("WaitingRoom");
        sceneManager.run();
        std::cout << "Game ended..." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        if (t && t->joinable()) {
            t->join();
        }
        return 1;
    }
    if (t && t->joinable()) {
        t->join();
    }
    return 0;
}
