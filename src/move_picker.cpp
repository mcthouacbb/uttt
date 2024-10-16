#include "move_picker.h"

MovePicker::MovePicker(const Board& board, Move ttMove)
    : m_Board(board), m_TTMove(ttMove), m_CurrIdx(0)
{
    genMoves(m_Board, m_Moves);
    for (int i = 0; i < m_Moves.size(); i++)
        m_MoveScores[i] = scoreMove(m_Moves[i]);
}

Move MovePicker::pickNext()
{
    if (m_CurrIdx >= m_Moves.size())
        return NULL_MOVE;
    int bestIdx = m_CurrIdx;
    for (int i = m_CurrIdx + 1; i < m_Moves.size(); i++)
    {
        if (m_MoveScores[i] > m_MoveScores[bestIdx])
            bestIdx = i;
    }
    std::swap(m_MoveScores[m_CurrIdx], m_MoveScores[bestIdx]);
    std::swap(m_Moves[m_CurrIdx], m_Moves[bestIdx]);
    return m_Moves[m_CurrIdx++];
}

int32_t MovePicker::scoreMove(Move move) const
{
    if (move == m_TTMove)
        return 100000000;
    return 0;
}