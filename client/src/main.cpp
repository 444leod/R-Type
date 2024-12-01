/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "EventDispatcher.hpp"
#include "Foo.hpp"

int main(void)
{
    Foo *foo = new Foo();
    EventDispatcher dispatcher;
    dispatcher.link<foo_message>(foo);
    dispatcher.broadcast<foo_message>({ .message = "Hello world!" });
    dispatcher.unlink<foo_message>(foo);
    dispatcher.broadcast<foo_message>({ .message = "If this shows up we're cooked..." });
    return 0;
}
