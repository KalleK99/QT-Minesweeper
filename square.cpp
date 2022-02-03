#include "square.hh"
#include "gameboard.hh"

Square::Square(int x, int y, bool mine, GameBoard* board):
    x_(x), y_(y), adjacent_mines_(0), mine_(mine), open_(false), flag_(false),
    board_(board)
{
}

Square::~Square()
{
}

int Square::countAdjacent()
{
    int min_x = std::max(0, x_ - 1);
    int max_x = std::min(board_->getSize() - 1, x_ + 1);
    int min_y = std::max(0, y_ - 1);
    int max_y = std::min(board_->getSize() - 1, y_ + 1);

    int nb = 0;
    for(int y = min_y; y <= max_y; ++y)
    {
        for(int x = min_x; x <= max_x; ++x)
        {
            if(not(x == x_ and y == y_))
            {
                if(board_->getSquare(x, y).hasMine())
                {
                    ++nb;
                }
            }
        }
    }
    adjacent_mines_ = nb;
    return adjacent_mines_;
}

bool Square::isReady() const
{
    if(mine_ and not flag_)
    {
        return false;
    }
    if(not mine_ and flag_)
    {
        return false;
    }
    return true;
}

bool Square::hasFlag() const
{
    return flag_;
}

bool Square::hasQuestionMark()
{
    return question_mark;
}

void Square::addQuestionMark()
{
    question_mark = true;
}

void Square::removeQuestionMark()
{
    question_mark = false;
}



void Square::addFlag()
{
    flag_ = true;
}

void Square::removeFlag()
{
    flag_ = false;
}

bool Square::isOpen() const
{
    return open_;
}

bool Square::hasMine() const
{
    return mine_;
}

void Square::print(std::ostream & s) const
{
    if(flag_)
    {
        s << FLAG;
    }
    else if(not open_)
    {
        s << CLOSED;
    }
    else if(mine_)
    {
        s << MINE;
    }
    else
    {
        s << adjacent_mines_;
    }
}

void Square::printDebug(std::ostream &s) const
{
    if(mine_)
    {
        s << MINE;
    }
    else
    {
        s << adjacent_mines_;
    }
}

bool Square::open()
{
    // A flagged square cannot be opened.
    if(flag_)
    {
        return true;
    }

    open_ = true;
    if(mine_)
    {
        return false;
    }

    if(adjacent_mines_ == 0)
    {
        int min_x = std::max(0, x_ - 1);
        int max_x = std::min(board_->getSize() - 1, x_ + 1);
        int min_y = std::max(0, y_ - 1);
        int max_y = std::min(board_->getSize() - 1, y_ + 1);


        for(int y = min_y; y <= max_y; ++y)
        {
            for(int x = min_x; x <= max_x; ++x)
            {
                if(not board_->getSquare(x, y).isOpen())
                {
                    board_->openSquare(x, y); // Indirect recursive call
                }
            }
        }
    }

    return true;
}