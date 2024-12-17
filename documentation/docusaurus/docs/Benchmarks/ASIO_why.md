---
sidebar_position: 4
id: 'AsioWhy'
title: 'What is Asio and why is it an adapted library?'
---

# What is Asio and why is it an adapted library?

> Asio is a cross-platform C++ library for network and low-level I/O programming that provides developers with a consistent asynchronous model using a modern C++ approach.

[Source](https://think-async.com/Asio/)

### Asio vs Windows Sockets and BSD Sockets

Here’s why using the **Asio library** for networking is generally better compared to relying on OS-specific network APIs, such as **BSD Sockets** on Linux or **Windows Sockets** on Windows:

---

### 1. **Cross-Platform Compatibility**  
   - **Asio** is a **cross-platform** library that abstracts the complexities of using platform-specific APIs.  
   - Instead of writing and maintaining different code for Windows Sockets and BSD Sockets, Asio provides a single, unified interface that works on **Windows, Linux, macOS**, and other systems.  
   - This saves time and reduces errors associated with managing platform-specific quirks.

---

### 2. **Modern C++ Design**  
   - Asio is built with **modern C++ features** like templates, smart pointers, and exception handling, making it easier to integrate into C++ projects.  
   - It avoids the outdated and error-prone approaches of raw pointers and manual resource management often required by OS-specific APIs.

---

### 3. **Asynchronous I/O Support**  
   - Asio is designed to handle **asynchronous I/O** operations effectively using an event-driven model.  
   - Instead of relying on blocking sockets or polling mechanisms, Asio can efficiently manage thousands of connections using asynchronous programming (e.g., `async_read`, `async_write`).  
   - This is particularly useful for applications like **multiplayer games** or servers where responsiveness and scalability are critical.

---

### 4. **Ease of Use and Clean API**  
   - OS-specific APIs like BSD Sockets or Windows Sockets require developers to write a lot of boilerplate code to handle low-level details like socket creation, binding, and cleanup.  
   - **Asio** simplifies these tasks with a cleaner, high-level API that minimizes repetitive code.  
   - Its API is intuitive and consistent, which reduces the learning curve and makes networking code easier to read and maintain.

---

### 5. **Support for Modern Features**  
   - Asio supports modern networking features, such as:  
     - **IPv4 and IPv6**  
     - **Timers** for deadline-based operations  
     - Integration with **C++ coroutines** (since C++20) for cleaner asynchronous code  
     - Efficient handling of large-scale I/O operations  

   - These features are either cumbersome or not available in legacy OS-specific APIs.

---

### 6. **Portability for Future Projects**  
   - If you decide to expand your application to other platforms or networks (e.g., switching from Linux to Windows), Asio ensures minimal refactoring.  
   - OS-specific code ties you to one platform, making portability difficult and expensive.

---

### 7. **Well-Maintained and Documented**  
   - Asio is part of the **Boost library** ecosystem and has been battle-tested in real-world applications.  
   - It is well-documented with a strong community of developers who actively contribute to its maintenance and provide support.

---

### Example: BSD Sockets vs Asio

Using BSD Sockets, a simple TCP client might look like this:
```cpp
int sock = socket(AF_INET, SOCK_STREAM, 0);
struct sockaddr_in server;
server.sin_family = AF_INET;
server.sin_port = htons(8080);
server.sin_addr.s_addr = inet_addr("127.0.0.1");

connect(sock, (struct sockaddr *)&server, sizeof(server));
send(sock, "Hello, server!", 13, 0);
close(sock);
```

With **Asio**, the same functionality is simpler and cleaner:
```cpp
boost::asio::io_context io_context;
boost::asio::ip::tcp::socket socket(io_context);

boost::asio::ip::tcp::resolver resolver(io_context);
auto endpoints = resolver.resolve("127.0.0.1", "8080");

boost::asio::connect(socket, endpoints);
boost::asio::write(socket, boost::asio::buffer("Hello, server!"));
```

---

### Conclusion  
**Asio** is a superior choice because it offers **cross-platform compatibility**, **modern C++ features**, **asynchronous I/O**, and a clean, well-documented API. It simplifies networking code, enhances maintainability, and ensures scalability for complex applications. In contrast, OS-specific APIs like BSD Sockets or Windows Sockets are outdated, platform-bound, and require more effort to implement efficiently.