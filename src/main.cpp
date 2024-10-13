#include <iostream>
#include <random>

#include "board.h"
#include "movegen.h"

std::string moveToStr(Move move)
{
    int x = move.to.x();
    int y = move.to.y();
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

enum class GameResult
{
    X_WIN, O_WIN, DRAW
};

GameResult playRandomGame(std::mt19937& rng)
{
    Board board;
    while (true)
    {
        MoveList moves;
        genMoves(board, moves);

        /*if (moves.size() == 0)
        {
            std::cout << board.stringRep() << std::endl;
            std::cout << board.completedBoards() << std::endl;
            std::cout << "WHAT?" << std::endl;
            throw 0;
        }*/
        std::uniform_int_distribution<int> dist(0, moves.size() - 1);
        int idx = dist(rng);

        board.makeMove(moves[idx]);

        if (board.isWon() || board.isDrawn())
            break;
    }
    // std::cout << board.stringRep() << std::endl;
    if (board.isWon())
        return board.sideToMove() == Color::X ? GameResult::O_WIN : GameResult::X_WIN;
    // terrible
    if (board.isDrawn())
        return GameResult::DRAW;
    return GameResult::DRAW;
}

int main()
{
    std::mt19937 rng(10483);
    int counts[3] = {};
    for (int i = 0; i < 100/*0000*/; i++)
        counts[static_cast<int>(playRandomGame(rng))]++;
    std::cout << counts[0] << ' ' << counts[1] << ' ' << counts[2] << std::endl;
    Board board;
    //board.setToFen("xox x x  xo x x   ox  o          o x  xox x      o    ooo    x     o    o   o x O");
    std::cout << board.stringRep() << std::endl;
    perft(board, 1);
    return 0;
}