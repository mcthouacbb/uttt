#pragma once

#include "movegen.h"

class MovePicker
{
public:
    MovePicker(const Board& board, Move ttMove);

    Move pickNext();
private:
    int32_t scoreMove(Move move) const;

    const Board& m_Board;
    Move m_TTMove;
    MoveList m_Moves;
    int32_t m_MoveScores[81];
    uint32_t m_CurrIdx;
};