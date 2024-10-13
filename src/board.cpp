#include "board.h"
#include <cctype>
#include <array>

Board::Board()
    : m_SideToMove(Color::X)
{
    m_BoardStates.reserve(128);
    BoardState rootState = {};
    rootState.subBoardIdx = -1;
    m_BoardStates.push_back(rootState);
}

void Board::setToFen(std::string_view fen)
{
    m_BoardStates.clear();
    if (fen == "")
    {
        BoardState rootState = {};
        rootState.subBoardIdx = -1;
        m_BoardStates.push_back(rootState);
        return;
    }
    else
    {
        m_BoardStates.push_back({});
    }
    // terrible variable name
    constexpr std::array<int, 9> ORDER = {6, 7, 8, 3, 4, 5, 0, 1, 2};
    int i = 0;
    int currSubBoard = -1;
    for (int subBoard : ORDER)
    {
        for (int square : ORDER)
        {
            char c = fen[i++];
            if (std::isupper(c))
                currSubBoard = subBoard;
            c = std::tolower(c);
            if (c == 'x')
            {
                state().subBoards[subBoard][static_cast<int>(Color::X)] |= Bitboard::fromSquare(square);
            }
            else if (c == 'o')
            {
                state().subBoards[subBoard][static_cast<int>(Color::O)] |= Bitboard::fromSquare(square);
            }
        }
    }

    for (int i = 0; i < 9; i++)
    {
        if (attacks::boardIsWon(state().subBoards[i][static_cast<int>(Color::X)]))
            state().won[static_cast<int>(Color::X)] |= Bitboard::fromSquare(i);

        if (attacks::boardIsWon(state().subBoards[i][static_cast<int>(Color::O)]))
            state().won[static_cast<int>(Color::O)] |= Bitboard::fromSquare(i);

        if ((state().subBoards[i][0] | state().subBoards[i][1]) == IN_BOARD)
            state().drawn |= Bitboard::fromSquare(i);
    }

    state().subBoardIdx = currSubBoard;
}

void Board::makeMove(Move move)
{
    m_BoardStates.push_back(state());
    auto& newState = state();
    auto& subBoard = newState.subBoards[move.to.subBoard()][static_cast<int>(m_SideToMove)];
    subBoard |= Bitboard::fromSquare(move.to.subSquare());

    if (attacks::boardIsWon(subBoard))
        newState.won[static_cast<int>(m_SideToMove)] |= Bitboard::fromSquare(move.to.subBoard());
    if ((newState.subBoards[move.to.subBoard()][0] | newState.subBoards[move.to.subBoard()][1]) == IN_BOARD)
        newState.drawn |= Bitboard::fromSquare(move.to.subBoard());

    newState.subBoardIdx = move.to.subSquare();

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
            str += "-----------";
        str += '\n';
    }
    return str;
}

void Board::unmakeMove(Move move)
{
    m_SideToMove = ~m_SideToMove;
    m_BoardStates.pop_back();
}
