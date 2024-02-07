#include <iostream>

#include "board.h"

int main()
{
    Board board;
    board.makeMove(Move(board.subBoardIdx(), 0));
    board.makeMove(Move(board.subBoardIdx(), 3));
    board.makeMove(Move(board.subBoardIdx(), 3));
    board.makeMove(Move(board.subBoardIdx(), 4));
    board.makeMove(Move(board.subBoardIdx(), 7));
    board.makeMove(Move(board.subBoardIdx(), 2));
    std::cout << board.stringRep() << std::endl;
    return 0;
}