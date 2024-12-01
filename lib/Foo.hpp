/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Foo
*/

#pragma once

#include "Events.hpp"
#include <iostream>

struct foo_message : public IEvent
{
    const std::string& message;
};

class Foo : public EventHandler<foo_message>
{
public:
    void say() noexcept
    {
        std::cout << "Hello world!" << std::endl;
    }

    virtual void receive(const foo_message& event) override {
        std::cout << "Message: " << event.message << std::endl;
    }

protected:
private:
};
