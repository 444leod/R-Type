#!/bin/sh

echo -e "Welcome to the AER-Type Conan installation script (CI Special Edition)\n\n"

echo -e "Installing Conan\n"
apt install pipx
pipx ensurepath

if [ $? -ne 0 ]; then
    echo -e "Failed to install Conan :(\n"
    exit 1
fi

echo -e "Conan installed successfully!\n"
