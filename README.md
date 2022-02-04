# QT-Minesweeper
This was a school project. We were given the source code for a minesweeper game, and our assignment was to create a GUI for the game using QT.
More specifically, the Square and Gameboard classes were already implemented, and the MainWindow class was implemented by me. 

# INSTRUCTIONS
The player can start the game by pressing the Start button. Clicking it generates the gameboard on the screen and starts the timer.
The player can choose what they want to do using the radio buttons just above the gameboard: They can either open a square or place a 
flag or a question mark on it. (NOTE: the flag/ question mark can only be removed by clicking the square again while "open a square" is selected)
The goal is to open every square that doesn't contain a mine.

If the player opens a square that has a mine in it, they lose. If they open one that's right next to a mine, the square will display the number of mines 
that are nearby. If there are no mines nearby, all the empty squares nearby will be opened automatically to save time. 

When the game ends, the player can either start a new game by pressing the Reset button and then the Start button again, or just close the program by clicking the close button

# Design decisions:
The layout of the UI consists of various smaller layouts nestled nestled into one big vertical box layout. The gameboard is made of PushButtons that are inserted into a GridLayout.The pointers to these PushButtons are stored in a vector that has the same dimensions as the board_ vector. This makes it very simple to syncronize the UI gameboard buttons with the GameBoard object squares, as you can just use the same coordinates.
