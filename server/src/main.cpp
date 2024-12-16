/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <thread>
#include "Server.hpp"

int main(void)
{
    try
    {
        asio::io_context ctx;
        Server server(ctx);
        std::thread t([&ctx]() {
            ctx.run();
        });
        server.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
