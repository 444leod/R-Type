---
sidebar_position: 8
id: 'Modules'
title: 'Game and Scene Modules'
---

# Modules

The game engine is divided into several modules, each responsible for a specific aspect of the engine. This modular design allows for better organization and separation of concerns, making it easier to maintain and extend the engine. There is two types of modules: Game Modules and Scene Modules. The Game Modules control behaviors in the game scope, while the Scene Modules control behaviors in the scene scope.

### Game Module

```cpp
class AGameModule
{
  public:
    explicit AGameModule() = default;
    virtual ~AGameModule() = default;

    virtual void start(AScene& scene) = 0;
    virtual void refresh(AScene& scene) = 0;
    virtual void stop() = 0;
    virtual void update() = 0;
};
```

The `start` method is called when the module is added to the game.
The `refresh` method is called when the module each time the scene changes.
The `stop` method is called when at the end of the game.
The `update` method is called each frame.

### Scene Module

```cpp
class ASceneModule
{
  public:
    explicit ASceneModule(AScene& scene) : _scene(scene) {}
    virtual ~ASceneModule() = default;

  protected:
    AScene& _scene;
};
```

The `ASceneModule` class is a base class for all scene modules. It provides a reference to the scene it is attached to. The scene module can be used to add behaviors to the scene, such as rendering or networking.

### Already implemented modules:

Here is the list of modules that are already implemented in the engine:
- `ANetworkGameModule`
- `ANetworkSceneModule`
- `APacketHandlerSceneModule`
- `ASceneRenderingModule`
- `GameRenderingModule`
- `ASceneAudioModule`
- `GameAudioModule`
