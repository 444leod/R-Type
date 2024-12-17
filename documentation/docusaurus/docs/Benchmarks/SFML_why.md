---
sidebar_position: 4
id: 'SFMLWhy'
title: 'Why is SFML an adapted graphic library?'
---

# Why is SFML an adapted graphic library?'

### SFML: A Modern C++ Graphics Library

> SFML provides a simple interface to the various components of your PC, to ease the development of games and multimedia applications. It is composed of five modules: system, window, graphics, audio and network.

> With SFML, your application can compile and run out of the box on the most common operating systems: Windows, Linux, macOS and Android & iOS (with limitations).


> SFML has official bindings for the C and .Net languages. And thanks to its active community, it is also available in many other languages such as Java, Ruby, Python, Go, and more.

[Source](https://www.sfml-dev.org/index.php)

### SFML vs SDL vs Raylib

To better understand why we chose SFML, let's compare it to two other popular graphic libraries: SDL and Raylib.

| **Feature**              | **SFML**                                      | **SDL**                                       | **Raylib**                                |
|--------------------------|-----------------------------------------------|-----------------------------------------------|--------------------------------------------|
| **Language**             | C++ (Natively)                                | C (Natively)                                  | C (Natively)                               |
| **Ease of Use**          | User-friendly, object-oriented API            | Requires manual management, procedural style  | Simplistic API, ideal for beginners        |
| **Modularity**           | Separated modules (graphics, audio, etc.)     | Monolithic design                             | Modular but focused on simplicity          |
| **Platform Support**     | Windows, Linux, macOS, Android (limited)      | Windows, Linux, macOS, Android, iOS           | Windows, Linux, macOS, Web, Android        |
| **Performance**          | Good performance, modern C++ usage            | Optimized for raw performance                 | Good performance, lightweight              |
| **Community**            | Active community with many bindings           | Large community and long history              | Growing community, beginner-friendly       |
| **Documentation**        | Well-structured, easy to navigate             | Comprehensive but requires effort to digest   | Simple and beginner-focused                |


### Why SFML Stands Out

1. **Modern C++ Design**  
   SFML is built around modern C++ principles, which makes it highly intuitive for developers familiar with object-oriented programming. Its modular structure allows you to use only the components you need, without unnecessary overhead.

2. **Ease of Learning**  
   Compared to SDL's low-level API or Raylib's minimalistic approach, SFML offers a perfect balance of simplicity and functionality. The clear documentation and community support make it accessible. We are also familiar with the library, which makes it easier to implement.

3. **Cross-Platform Compatibility**  
   SFML enables seamless development across major platforms (Windows, Linux, macOS), with some support for Android and iOS. Applications can be compiled and run "out of the box" with minimal configuration.

4. **Versatile Functionality**  
   With its five core modules—System, Window, Graphics, Audio, and Network—SFML covers everything needed for multimedia applications. Whether you're creating a game or a multimedia tool, SFML offers an all-in-one solution.

5. **Extensive Language Bindings**  
   SFML's official support for C and .Net, along with community bindings for Python, Java, Go, and more, that could prouve useful to implement a scripting language into our GameEngine.

### Conclusion

SFML's blend of modern C++ features, ease of use, and cross-platform capabilities makes it an adapted choice for developers working on games and multimedia applications. While SDL and Raylib are strong alternatives, SFML's modular design and clean API set it apart as a versatile and developer-friendly library.

