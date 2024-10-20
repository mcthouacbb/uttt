#include "evaluate.h"
#include "eval_constants.h"
#include <array>

namespace eval
{

template<Color us, bool Trace>
int evaluatePsqt(const Board& board)
{
    int eval = 0;
    for (int subBoard = 0; subBoard < 9; subBoard++)
    {
        Bitboard bb = board.subBoard(us, subBoard);
        while (bb.any())
        {
            int sq = bb.poplsb();
            eval += psqt[sq];
            if constexpr (Trace)
                TRACE_INC(psqt[sq]);
        }
    }
    return eval;
}

template<Color us, bool Trace>
int evaluateWonSubBoards(const Board& board)
{
    int eval = 0;
    Bitboard wonSubBoards = board.wonBoards(us);
    while (wonSubBoards.any())
    {
        int subBoard = wonSubBoards.poplsb();
        eval += 5 * psqt[subBoard];
        if constexpr (Trace)
            TRACE_ADD(psqt[subBoard], 5);
    }
    return eval;
}

template<bool Trace>
int evaluateImpl(const Board& board)
{
    if constexpr (Trace)
        currTrace = {};

    int eval = 0;
    eval += evaluatePsqt<Color::X, Trace>(board) - evaluatePsqt<Color::O, Trace>(board);
    eval += evaluateWonSubBoards<Color::X, Trace>(board) - evaluateWonSubBoards<Color::O, Trace>(board);
    return board.sideToMove() == Color::X ? eval : -eval;
}

int evaluate(const Board& board)
{
    return evaluateImpl<false>(board);
}

int evalTrace(const Board& board)
{
    return evaluateImpl<true>(board);
}


}