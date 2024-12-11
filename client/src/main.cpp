/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "Foo.hpp"
#include "network/Packet.hpp"

struct Positon
{
    long double x, y;
};

std::ostream& operator<<(std::ostream& os, const Positon &pos)
{
    os << "{ " << pos.x << ", " << pos.y << " }";
    return os;
}

int main(void)
{
    Packet packet;

    packet << Positon{1.78367f, 2.3728f};
    Positon a{};
    packet >> a;
    std::cout << a << std::endl;

    packet << "youhou";
    // std::string str;
    // packet >> str;
    // std::cout << str << std::endl;
    return 0;
}


