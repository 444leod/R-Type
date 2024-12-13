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
    asio::io_context ctx;
    Server server(ctx);
    std::thread t([&ctx]() {
        ctx.run();
    });
    server.run();
    return 0;
}
