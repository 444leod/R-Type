---
sidebar_position: 7
id: 'Systems'
title: 'Systems'
---

# Systems

Systems are an essential part of an ECS. They are responsible for updating the components of entities. A system is a class that inherits from the `ASystem` class and implements logic that interacts with components using `Views`. Systems are pretty modular depending on the game's needs, they are different types of abstract systems that can be used to update components in different ways.

## Abstract Systems

### ASystem

```cpp
class ASystem
{
  public:
    explicit ASystem(std::string name) : _registry(engine::RestrictedGame::instance().registry()), _name(std::move(name)) {}
    virtual ~ASystem() = default;

    /**
     * @brief Disables the system
     */
    void disable() { _enabled = false; }

    /**
     * @brief Enables the system
     */
    void enable() { _enabled = true; }

    /**
     * @brief Returns whether the system is enabled
     * @return Whether the system is enabled
     */
    [[nodiscard]] bool isEnabled() const { return _enabled; }

    /**
     * @brief Returns the system's name
     * @return The system's name
     */
    [[nodiscard]] std::string name() const { return _name; }

  protected:
    ecs::Registry& _registry;
    bool _enabled = true;
    const std::string _name;
};
```

This is the base class for all systems. It provides a way to enable or disable the system and a name for the system.

### AEventSystem

```cpp
class AEventSystem : public engine::ASystem
{
  public:
    explicit AEventSystem(const std::string& name) : ASystem(name) {}

    /**
     * @brief Execute a 'event' system's logic
     *
     * @param event The event to handle
     */
    virtual void execute(sf::Event& event) = 0;
};
```

This is the base class for all event systems. It provides a way to execute the system's logic based on an event.

### AUpdateSystem

```cpp
class AUpdateSystem : public engine::ASystem
{
  public:
    explicit AUpdateSystem(const std::string& name) : ASystem(name) {}

    /**
     * @brief Execute a 'update' system's logic
     * @param deltaTime The time between the last frame and the current one
     */
    virtual void execute(const double& deltaTime) = 0;
};
```

This is the base class for all update systems. It provides a way to execute the system's logic based on the time between the last frame and the current one.

### ARenderSystem

```cpp
class ARenderSystem : public engine::ASystem
{
  public:
    explicit ARenderSystem(const std::string& name) : ASystem(name) {}

    /**
     * @brief Execute a 'render' system's logic
     * @param window The window to render to
     */
    virtual void execute(sf::RenderWindow& window) = 0;
};
```

This is the base class for all render systems. It provides a way to render entities to a window.
