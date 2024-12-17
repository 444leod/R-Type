---
sidebar_position: 2
id: 'Buid and Run'
---

# Getting Started !

This section describes how to build and run the different parts of the project.

# R-Type Client and Server

### Prerequisites

- `Make` version 4.4.1 or later
- `CMAKE` version 3.24 later
- A C++ compiler that supports C++23

:::info
Useful links:
- [Make](https://www.gnu.org/software/make/)
- [CMAKE](https://cmake.org/)
:::

### Build

Execute the following commands at the root of the project:

```bash
cmake .
make
```

### Run

`./r-type_server` to run the server and `./r-type_client` for the client.

# Docusaurus Documentation

### Prerequisites

- `npm` version 9.5.6 or later or `yarn` version 4.5.3 or later

### Install dependencies

```bash
cd documentation/docusaurus
npm install || yarn install
```

### Run

While in the `documentation/docusaurus` directory, execute the following command `npm start` or `yarn start` and got to `localhost:3000` on your browser.

:::tip
Add `--watch` after `start` to enable live reload.
:::

# Conan (C++ Package Manager)

### Prerequisites

- `Make` version 4.4.1 or later
- `Python`

### Install Conan

Execute the following commands at the root of the project:

```bash
make conan
source rtype_venv/bin/activate
sudo make deps
```

### Build

Execute the following commands at the root of the project:
```bash
make
```

:::info
Conan support cross-platform compilation (Windows, Linux)
:::
