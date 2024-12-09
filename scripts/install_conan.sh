#!/bin/bash

echo -e "Welcome to the AER-Type Conan installation script\n\n"

# if ! command -v python3 &> /dev/null; then
#     echo -e "Python3 is not installed :( Please install it and try again.\n"
#     exit 1
# fi

read -p "Enter the venv name you want to create: (default rtype_venv)" venv_name
venv_name=${venv_name:-"rtype_venv"}

echo -e "\nCreating virtual environment $venv_name\n"
python3 -m venv $venv_name

if [ $? -ne 0 ]; then
    echo -e "Failed to create virtual environment :(\n"
    exit 1
fi

echo -e "Activating virtual environment $venv_name\n"
source $venv_name/bin/activate

if [ $? -ne 0 ]; then
    echo -e "Failed to activate virtual environment :(\n"
    exit 1
fi

echo -e "Installing Conan\n"
pip install conan

if [ $? -ne 0 ]; then
    echo -e "Failed to install Conan :(\n"
    desactivate
    exit 1
fi

source ~/.profile

echo -e "Conan installed successfully!\n"
