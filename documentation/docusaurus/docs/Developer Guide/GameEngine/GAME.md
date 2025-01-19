---
sidebar_position: 9
id: 'Game'
title: 'Game'
---

# Game

In this section we will show you how to create a game using the engine.

## Game Flowchart

This flowchart shows how the `Game` class interacts with other classes in the engine.

```mermaid
flowchart TB
    Start([Game Start]) --> Init[Initialize Game]
    Init --> AddModules[Add Game Modules]
    AddModules --> RegisterScenes[Register Scenes]
    RegisterScenes --> RunGame[Run Game]
    
    subgraph GameLoop[Game Loop]
        direction TB
        CheckScene{Scene Change?} -->|Yes| RefreshModules[Refresh All Modules]
        CheckScene -->|No| UpdateModules[Update Game Modules]
        RefreshModules --> UpdateModules
        UpdateModules --> UpdateScene[Update Current Scene]
        UpdateScene --> FlushRegistry[Flush Registry]
        FlushRegistry --> ComputeDelta[Compute Delta Time]
        ComputeDelta --> Sleep[Sleep if needed]
        Sleep --> CheckScene
    end
    
    RunGame --> GameLoop
    GameLoop --> |Stop Requested| Cleanup[Cleanup]
    Cleanup --> StopScene[Stop Scene]
    StopScene --> StopModules[Stop Modules]
    StopModules --> End([Game End])
```

## Example game:

```cpp
int main()
{
    auto game = engine::Game(); // Create a game instance

    game.addModule<GameRenderingModule>(800, 800, "Example"); // Add a rendering module to the game

    const auto networkGameModule = game.addModule<NetworkGameModule>(); // Add a network module to the game

    const auto levelExample = game.registerScene<LevelExample>("game"); // Register a scene
    {
        const auto net = levelExample->addModule<ANetworkSceneModule>(*networkGameModule); // Add a network module to the scene
        levelExample->addModule<levelExample::PacketHandlerSceneModule>(net); // Add a packet handler module to the scene
        levelExample->addModule<ASceneRenderingModule>(); // Add a rendering module to the scene
    }

    game.run(); // Run the game
    return 0;
}
```
