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
    m_BoardStates.push_back({});
    // terrible variable name
    int currSq = 72;
    int i = 0;
    for (;;)
    {
        char c = fen[i++];
        if (c == ' ')
            break;
        Square sq(currSq);

        switch (c)
        {
            case 'x':
            {
                state().addPiece(Color::X, sq);
                currSq++;
                break;
            }
            case 'o':
            {
                state().addPiece(Color::O, sq);
                currSq++;
                break;
            }
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                currSq += c - '0';
                break;
            case '/':
                currSq -= 18;
                break;
        }
    }

    m_SideToMove = fen[i] == 'X' ? Color::X : Color::O;
    i += 2;
    while (isdigit(fen[i]))
        i++;
    i++;
    if (fen[i] == '0')
        state().subBoardIdx = -1;
    else
    {
        int sq = 9 * (fen[i + 1] - '1') + (fen[i] - 'a');
        state().subBoardIdx = Square(sq).subSquare();
        state().prevSquare = Square(sq);
    }

    for (int i = 0; i < 9; i++)
    {
        if (attacks::boardIsWon(subBoard(Color::X, i)))
            state().won[static_cast<int>(Color::X)] |= Bitboard::fromSquare(i);

        if (attacks::boardIsWon(subBoard(Color::O, i)))
            state().won[static_cast<int>(Color::O)] |= Bitboard::fromSquare(i);

        if ((subBoard(Color::X, i) | subBoard(Color::O, i)) == IN_BOARD)
            state().drawn |= Bitboard::fromSquare(i);
    }
}

std::string Board::fenStr() const
{
    std::string fen;
    for (int j = 72; j >= 0; j -= 9)
    {
        int lastFile = -1;
        for (int i = j; i < j + 9; i++)
        {
            Piece piece = pieceAt(Square(i));
            if (piece != Piece::NONE)
            {
                int diff = i - j - lastFile;
                if (diff > 1)
                    fen += static_cast<char>((diff - 1) + '0');
                fen += piece == Piece::X ? 'x' : 'o';
                lastFile = i - j;
            }
        }
        int diff = 9 - lastFile;
        if (diff > 1)
            fen += static_cast<char>((diff - 1) + '0');
        if (j != 0)
            fen += '/';
    }

    fen += ' ';

    fen += sideToMove() == Color::X ? 'X' : 'O';

    fen += ' ';
    fen += '0';
    fen += ' ';

    if (state().subBoardIdx == -1)
        fen += "0000";
    else
        fen += state().prevSquare.toString();
    return fen;
}

void Board::makeMove(Move move)
{
    m_BoardStates.push_back(state());
    auto& newState = state();
    newState.addPiece(m_SideToMove, move.to);

    if (attacks::boardIsWon(subBoard(m_SideToMove, move.to.subBoard())))
        newState.won[static_cast<int>(m_SideToMove)] |= Bitboard::fromSquare(move.to.subBoard());
    if ((subBoard(Color::X, move.to.subBoard()) | subBoard(Color::O, move.to.subBoard())) == IN_BOARD)
        newState.drawn |= Bitboard::fromSquare(move.to.subBoard());

    newState.subBoardIdx = move.to.subSquare();
    newState.prevSquare = move.to;

    newState.key.flipSideToMove();

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
                    Piece pce = pieceAt(Square(s, (2 - i) * 3 + k));
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

void Board::unmakeMove()
{
    m_SideToMove = ~m_SideToMove;
    m_BoardStates.pop_back();
}
