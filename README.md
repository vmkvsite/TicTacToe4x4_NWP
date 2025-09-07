# Tic Tac Toe 4x4

## Project for NWP, a Windows programming class.
---------------------------------------------------------------
## How to play:

- Run 'TicTacToe4x4.exe', no additional libraries are needed. "Restart" will restart the game, "Infinite" will make it so each player can only have 4 symbols at the board (whenever you try to play a 5th, the oldest symbol will turn purple and disappear upon next move), "Reset score" will reset the score, "Exit" leaves the program. Clicking on a status message after a win/draw starts a new game.

---------------------------------------------------------------
# Project structure is as follows:
---------------------------------------------------------------

**Main.cpp** - Handles user input, handles communication between Game and Renderer classes, handles Windows messages


**Game class (Game.cpp/.h)** - Handles entire game logic and state


**Renderer class (Renderer.cpp/.h)** - Draws all of the game state changes using Windows GDI


**Constants.h** - has all of the config info for menus, board dimensions, styling