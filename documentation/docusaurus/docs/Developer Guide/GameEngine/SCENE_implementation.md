---
sidebar_position: 6
id: 'SceneImplementation'
title: 'Scene'
---

# Scene And Scene Manager

The goal of this section is to explain the implementation of the Scene and Scene Manager classes. The Scene class is responsible for managing entities and systems, while the Scene Manager class is responsible for managing scenes (e.g., loading, unloading, and switching scenes).

### Implementations

#### AScene

```cpp
#ifndef ASCENE_HPP
#define ASCENE_HPP

class SceneManager;

#include <SFML/Graphics.hpp>
#include "ISceneManager.hpp"

class AScene {
public:
    AScene(ISceneManager& manager, const std::string& name):
        _manager(manager), _name(name) {}
    virtual~AScene() = default;

    /**
     * @brief Called once at the start of the program
     */
    virtual void initialize() = 0;

    /**
     * @brief Called every frame
     * @param deltaTime The time between this frame and the last
     */
    virtual void update(double deltaTime) = 0;

    /**
     * @brief Used to render the elements in a scene
     * @param window The  window to render to
     */
    virtual void render(sf::RenderWindow &window) = 0;

    /**
     * @brief Gets the name of the scene
     * @return The name of the scene
     */
    virtual const std::string& name() const noexcept { return this->_name; }

    /**
     * @brief Called for every window event.
     * @param event The event to be taken care of
     */
    virtual void onEvent(sf::Event &event) = 0;

    /**
     * @brief Called when the Scene starts without a predecessor
     */
    virtual void onEnter() = 0;

    /**
     * @brief Called when the Scene starts with a predecessor
     * @param lastScene A reference to the previous scene
     */
    virtual void onEnter(const AScene& lastScene) = 0;

    /**
     * @brief Called when the Scene shuts with no successor
     */
    virtual void onExit() = 0;

    /**
     * @brief Called when the Scene shuts with a successor
     * @param nextScene A reference to the successor
     */
    virtual void onExit(const AScene& nextScene) = 0;

protected:
    ISceneManager& _manager;

private:
    const std::string& _name = "";
};

#endif //ASCENE_HPP
```

#### SceneManager

```cpp
#ifndef ASCENE_HPP
#define ASCENE_HPP

class SceneManager;

#include <SFML/Graphics.hpp>
#include "ISceneManager.hpp"

class AScene {
public:
    AScene(ISceneManager& manager, const std::string& name):
        _manager(manager), _name(name) {}
    virtual~AScene() = default;

    /**
     * @brief Called once at the start of the program
     */
    virtual void initialize() = 0;

    /**
     * @brief Called every frame
     * @param deltaTime The time between this frame and the last
     */
    virtual void update(double deltaTime) = 0;

    /**
     * @brief Used to render the elements in a scene
     * @param window The  window to render to
     */
    virtual void render(sf::RenderWindow &window) = 0;

    /**
     * @brief Gets the name of the scene
     * @return The name of the scene
     */
    virtual const std::string& name() const noexcept { return this->_name; }

    /**
     * @brief Called for every window event.
     * @param event The event to be taken care of
     */
    virtual void onEvent(sf::Event &event) = 0;

    /**
     * @brief Called when the Scene starts without a predecessor
     */
    virtual void onEnter() = 0;

    /**
     * @brief Called when the Scene starts with a predecessor
     * @param lastScene A reference to the previous scene
     */
    virtual void onEnter(const AScene& lastScene) = 0;

    /**
     * @brief Called when the Scene shuts with no successor
     */
    virtual void onExit() = 0;

    /**
     * @brief Called when the Scene shuts with a successor
     * @param nextScene A reference to the successor
     */
    virtual void onExit(const AScene& nextScene) = 0;

protected:
    ISceneManager& _manager;

private:
    const std::string& _name = "";
};

#endif //ASCENE_HPP
```

### Explanation

1. **Separation of Concerns**
   - Clear separation between scene logic and scene management
   - Each scene is responsible for its own behavior

2. **Flexibility**
   - Easy to add new scenes
   - Standardized lifecycle methods
   - Supports complex scene transitions

3. **Performance**
   - Minimal overhead
   - Direct method calls
   - No complex inheritance hierarchies

### Example Usage

```cpp
class GameScene : public AScene {
public:
    GameScene(ISceneManager& manager) : AScene(manager, "GameScene") {}

    void initialize() override {
        // Load game resources
    }

    void update(double deltaTime) override {
        // Update game logic
    }

    void render(sf::RenderWindow& window) override {
        // Render game elements
    }

    // Implement other required methods...
};

// In main application
SceneManager sceneManager;
sceneManager.registerScene(std::make_unique<GameScene>(sceneManager));
sceneManager.switchToScene("GameScene");
```
