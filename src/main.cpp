#include <iostream>

#include "board.h"
#include "movegen.h"

std::string moveToStr(Move move)
{
    int x = 3 * (move.subBoard % 3) + move.square % 3;
    int y = 3 * (move.subBoard / 3) + move.square / 3;
    std::string result;
    result += static_cast<char>(x + 'a');
    result += static_cast<char>(y + '1');
    return result;
}

template<bool root = true>
uint64_t perft(Board& board, int depth)
{
    if (depth == 0)
        return 1;
    MoveList moves;
    uint64_t nodes = 0;
    genMoves(board, moves);
    for (Move move : moves)
    {
        board.makeMove(move);
        uint64_t subNodes = perft<false>(board, depth - 1);
        board.unmakeMove(move);
        if constexpr (root)
            std::cout << moveToStr(move) << ": " << subNodes << std::endl;
        nodes += subNodes;
    }

    if constexpr (root)
        std::cout << nodes << std::endl;
    return nodes;
}

int main()
{
    Board board;
    // board.makeMove(Move(board.subBoardIdx(), 0));
    // board.makeMove(Move(board.subBoardIdx(), 3));
    // board.makeMove(Move(board.subBoardIdx(), 3));
    // board.makeMove(Move(board.subBoardIdx(), 7));
    // board.makeMove(Move(board.subBoardIdx(), 2));
    board.setToFen("xox x x  xo x x   ox  o          o x  xox x      o    ooo    x     o    o   o x O");
    std::cout << board.stringRep();
    perft(board, 1);
    std::cout << board.stringRep() << std::endl;
    perft(board, 2);
    std::cout << board.stringRep() << std::endl;
    perft(board, 3);
    std::cout << board.stringRep() << std::endl;
    perft(board, 4);
    std::cout << board.stringRep() << std::endl;
    perft(board, 5);
    std::cout << board.stringRep() << std::endl;
    perft(board, 6);
    std::cout << board.stringRep() << std::endl;
    perft(board, 7);
    std::cout << board.stringRep() << std::endl;
    perft(board, 8);
    std::cout << board.stringRep() << std::endl;
    perft(board, 9);
    std::cout << board.stringRep() << std::endl;
    perft(board, 10);
    std::cout << board.stringRep() << std::endl;
    return 0;
}