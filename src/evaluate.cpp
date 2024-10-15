#include "evaluate.h"
#include <array>

namespace eval
{

// a1 b1 c1
// a2 b2 c2
// a3 b3 c3
constexpr std::array<int, 9> psqt =
{
    10, 20, 10,
    20, 30, 20,
    10, 20, 10
};

template<Color c>
int evaluatePsqt(const Board& board)
{
    int eval = 0;
    for (int subBoard = 0; subBoard < 9; subBoard++)
    {
        Bitboard bb = board.subBoard(c, subBoard);
        while (bb.any())
            eval += psqt[bb.poplsb()];
    }
    return eval;
}

template<Color c>
int evaluateWonSubBoards(const Board& board)
{
    int eval = 0;
    Bitboard wonSubBoards = board.wonBoards(c);
    while (wonSubBoards.any())
    {
        eval += 5 * psqt[wonSubBoards.poplsb()];
    }
    return eval;
}

int evaluate(const Board& board)
{
    int eval = 0;
    eval += evaluatePsqt<Color::X>(board) - evaluatePsqt<Color::O>(board);
    eval += evaluateWonSubBoards<Color::X>(board) - evaluateWonSubBoards<Color::O>(board);
    return board.sideToMove() == Color::X ? eval : -eval;
}


}