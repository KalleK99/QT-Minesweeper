#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include "square.hh"
#include <vector>

const int BOARD_SIDE = 6;
const int MINE_PROBABALITY = 20; // in percents

class GameBoard
{
public:
    GameBoard();
    ~GameBoard();

    // Initializes the board, i.e. forms square objects and calculates data
    // needed by them.
    void init(int seed);

    // Prints the game board.
    void print(std::ostream& stream) const;

    // Checks if the game is over.
    bool isGameOver() const;

    // Returns the size of the game board.
    int getSize() const;

    // Returns the square object in the given location.
    Square getSquare(int x, int y) const;

    // Sets the given square object into the given location.
    void setSquare(Square sq, int x, int y);

    // Opens the square in the given location.
    bool openSquare(int x, int y);

    // returns the board_ vector.
    std::vector<std::vector<Square>> getBoard_();

    //  Removes every element from the board_ vector.
    void emptyBoard();

    // counts how many flags have been placed, and returns the flag count.
    int count_flags();

private:
    std::vector<std::vector<Square>> board_;
};

#endif // GAMEBOARD_HH
