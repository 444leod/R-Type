---
sidebar_position: 3
id: 'ConanWhy'
title: 'Why Conan is an adapted packet manager?'
---

# Why Choose Conan for a C++ Game Engine Project Using ECS?

Managing dependencies efficiently is crucial for a C++ project, especially for game engines using an Entity-Component-System (ECS) architecture. Here's a comparison of **Vcpkg**, **CMake CPM**, and **Conan** that highlights why we choose Conan for a game engine with ECS.

### Vcpkg
- **Package Manager**: Microsoft-developed tool for C++ libraries with simple integration.
- **Advantages**:
  - **Easy Setup**: Quick installation and library management.
  - **Cross-Platform**: Supports Windows, Linux, and macOS.
- **Limitations**:
  - **Limited Binaries**: Primarily sources; slower builds on large projects.
  - **Less Flexible**: Harder to customize complex dependency graphs compared to Conan.
- **Use Case**: Good for simpler projects or teams tied to Visual Studio and Windows tooling.

### CMake CPM
- **CMake-based Dependency Manager**: A header-only solution for managing dependencies directly via CMake.
- **Advantages**:
  - **Lightweight**: No external tools; purely CMake-based.
  - **Simple Integration**: Easy to add dependencies using a single CMake script.
- **Limitations**:
  - **No Binaries**: Always builds dependencies from source, leading to longer build times.
  - **Scalability Issues**: Less suitable for large projects with many dependencies.
- **Use Case**: Ideal for lightweight projects or when you prefer to avoid external dependency managers.

### Conan
- **Package Manager**: Advanced C++ package manager supporting binary packages, versioning, and multiple build configurations.
- **Advantages**:
  - **Customizable**: Handles complex dependency graphs with fine-tuned control over compiler settings, platforms, and configurations.
  - **Binary Compatibility**: Pre-built binaries reduce build times.
  - **Flexible**: Supports multiple build systems (CMake, Make, etc.) and integrates seamlessly into CI/CD pipelines.
- **Ideal For**: Projects needing scalability, cross-platform support, and optimized builds (e.g., a game engine with ECS).
- **Why for ECS**: ECS-based engines benefit from Conan’s ability to manage libraries like `Boost`, or custom game components across different platforms.

### Why Conan Stands Out
For a game engine with ECS:
- **Binary Management**: Saves time by reusing pre-built binaries.
- **Scalability**: Handles complex dependency graphs efficiently.
- **Cross-Platform**: Seamlessly supports multiple OSes and architectures.
