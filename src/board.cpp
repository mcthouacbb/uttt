#include "board.h"
#include "attacks.h"

Board::Board()
    : m_SideToMove(Color::X)
{
    m_BoardStates.reserve(128);
    BoardState rootState = {};
    rootState.subBoardIdx = 4;
    m_BoardStates.push_back(rootState);
}

void Board::makeMove(Move move)
{
    m_BoardStates.push_back(state());
    auto& newState = state();
    auto& subBoard = newState.subBoards[move.subBoard][static_cast<int>(m_SideToMove)];
    subBoard |= Bitboard::fromSquare(move.square);
    
    if (attacks::boardIsWon(subBoard))
        newState.won[static_cast<int>(m_SideToMove)] |= Bitboard::fromSquare(move.subBoard);
    if (subBoard == IN_BOARD)
        newState.drawn |= Bitboard::fromSquare(move.subBoard);

    newState.subBoardIdx = move.square;

    m_SideToMove = ~m_SideToMove;
}

std::string Board::stringRep() const
{
    std::string str;
    // outer boards
    for (int j = 2; j >= 0; j--)
    {
        // rows
        for (int i = 0; i < 3; i++)
        {
            // sub boards
            for (int s = 3 * j; s < 3 * j + 3; s++)
            {
                // columns
                for (int k = 0; k < 3; k++)
                {
                    Piece pce = pieceAt(s, (2 - i) * 3 + k);
                    if (pce == Piece::X)
                        str += "X";
                    else if (pce == Piece::O)
                        str += "O";
                    else
                        str += ".";
                }
                if (s != 3 * j + 2)
                    str += '|';
            }
            str += '\n';
        }
        if (j != 0)
            str += "------------";
        str += '\n';
    }
    return str;
}

void Board::unmakeMove(Move move)
{
    m_SideToMove = ~m_SideToMove;
    m_BoardStates.pop_back();
}
