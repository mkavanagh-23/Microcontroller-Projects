# Microcontroller Projects
## A collection of projects for Arduino/ESP32/RPI microcontrollers

To make projects work with [nvim-platformio.lua](https://github.com/anurag3301/nvim-platformio.lua):

 - Ensure you have downloaded platformio-core and poroperly configured nvim-platformio-lua with clangd as its server
 - Create a new project directory, open nvim, and run `:Pioinit` to set up the initial project directory
 - Delete any generated `compile-commands.json` file
 - copy `setup.sh` to the project root and mark as executable
 - Run the setup script to properly configure the project
