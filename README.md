# 2D_game
This is my first attempt to create simple 2D game while learning C++. This game uses SFML library for rendering, playing audio and input handling.

# Technical information
OS: Win_64

# Contents
main.cpp

The entry point and the main cycle of the program. Here the configuration data is loaded, the resolution is set, the main window is created and an infinite loop is started [while (main_window.isOpen())].

menu.cpp

This object stores all the menus that are displayed both during the start of the game and when pressing ESC during the gameplay (PAUSE).

game.cpp

The GAME object stores the global data of the game (resources, improvements made, the current planet, etc.). This data is recorded in the save file at the exit and loaded from the file when you click "Continue the game".
In this mode, the player is on an orbital station and can upgrade equipment and move between planets.

mission.cpp

This object is created when landing on the planet and contains the data of a separate mission (resources, etc.) before its completion and lifting into orbit. Here the landscape is rendered (procedural generation), monsters, explosions, etc.
