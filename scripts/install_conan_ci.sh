#!/bin/bash

echo -e "Welcome to the AER-Type Conan installation script (Special CI Edition)\n\n"

echo -e "\nCreating virtual environment rtype_venv\n"
python3 -m venv rtype_venv

if [ $? -ne 0 ]; then
    echo -e "Failed to create virtual environment :(\n"
    exit 1
fi

echo -e "Activating virtual environment rtype_venv\n"
source rtype_venv/bin/activate

if [ $? -ne 0 ]; then
    echo -e "Failed to activate virtual environment :(\n"
    exit 1
fi

echo -e "Installing Conan\n"
pip install conan

if [ $? -ne 0 ]; then
    echo -e "Failed to install Conan :(\n"
    exit 1
fi

source ~/.profile

echo -e "Conan installed successfully!\n"
