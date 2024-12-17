---
sidebar_position: 1
id: 'EcsWhat'
title: 'What is an ECS?'
---

# Understanding Entity Component System (ECS)

> 
Entity–component–system (ECS) is a software architectural pattern mostly used in video game development for the representation of game world objects. An ECS comprises entities composed from components of data, with systems which operate on the components.
ECS follows the principle of composition over inheritance, meaning that every entity is defined not by a type hierarchy, but by the components that are associated with it. Systems act globally over all entities which have the required components.


## What is an Entity Component System?

An Entity Component System (ECS) is an architectural pattern commonly used in game development and real-time simulation systems that provides a flexible and efficient way to organize game objects and their behaviors.

### Core Concepts

1. **Entities**
   - Entities are unique identifiers or containers
   - They represent individual objects in the game world
   - Essentially, an entity is just a simple ID

2. **Components**
   - Pure data structures that define attributes
   - Lightweight and focused on storing specific types of data
   - Examples include:
     - `PositionComponent`: Stores x, y, z coordinates
     - `HealthComponent`: Tracks current and maximum health
     - `RenderComponent`: Contains rendering information
   - Only data, no logic

3. **Systems**
   - Contain the logic for processing components
   - Operate on groups of entities with specific components (eg: all entities with `PositionComponent` and `VelocityComponent`)
   - Responsible for updating and manipulating game state
   - Stateless and focus on behavior

### Basic Structure

```
Entity (ID: 42)
├── PositionComponent
├── RenderComponent
└── PhysicsComponent

System: MovementSystem
- Processes all entities with PositionComponent and VelocityComponent
- Updates spatial relationships
```

## Key Characteristics

- **Decoupling**: Separates data from behavior
- **Composition**: Builds complex objects through component combination
- **Performance**: Enables cache-friendly memory layouts
- **Flexibility**: Easy to add or remove functionality
- **Parallelism**: Supports concurrent processing of entities
- **Scalability**: Scales well with large numbers of entities
- **Reusability**: Promotes reusable components and systems
