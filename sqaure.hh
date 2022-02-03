#ifndef SQUARE_HH
#define SQUARE_HH

#include <iostream>

const char CLOSED = '.';
const char FLAG = 'P';
const char MINE = '*';

class GameBoard; // Forward declaration

class Square
{
public:

    Square(int x, int y, bool mine, GameBoard* board);
    ~Square();

    // Opens a square.
    // Returns false, if the square has been opened and it has a mine.
    // If the adjacent squares have no mines, also these squares will be opened.
    bool open();

    // Prints the square onto the given stream based on the current game
    // situation.
    void print(std::ostream& s) const;

    // Prints either the number of adjacent mines or the mine.
    void printDebug(std::ostream& s) const;

    // Returns false, if the square has a mine and has not a flag
    //                or if the square has a flag and has not a mine
    bool isReady() const;

    // Returns true, if the square has been opened.
    bool isOpen() const;

    // Returns true, if the square has a mine.
    bool hasMine() const;

    // Returns true, if the square has a flag.
    bool hasFlag() const;

    // returns true if the square has a question mark
    bool hasQuestionMark();

    // adds a question mark on the square
    void addQuestionMark();

    // removes the question mark from a square
    void removeQuestionMark();

    // Adds a flag on the square.
    void addFlag();

    // Removes the flag from the square.
    void removeFlag();

    // Counts the number of mines in the adjacent squares.
    int countAdjacent();

private:

    // Location, number of adjacent mines, and the status of the square.
    int x_;
    int y_;
    int adjacent_mines_;
    bool mine_;
    bool open_;
    bool flag_;
    bool question_mark;

    // Pointer to the board, where the square lies.
    GameBoard* board_;
};

#endif // SQUARE_HH
