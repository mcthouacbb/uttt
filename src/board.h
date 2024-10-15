#pragma once

#include "types.h"
#include "bitboard.h"
#include "attacks.h"
#include "zobrist.h"
#include <vector>

struct BoardState
{
    Bitboard subBoards[2][9];
    Bitboard won[2];
    Bitboard drawn;

    int subBoardIdx;
    ZKey key;

    Square prevSquare;

    void addPiece(Color color, Square square)
    {
        subBoards[static_cast<int>(color)][square.subBoard()] |= Bitboard::fromSquare(square.subSquare());
        key.addPiece(color, square);
    }
};

class Board
{
public:
    Board();

    void setToFen(std::string_view fen);

    void makeMove(Move move);
    void unmakeMove();

    std::string stringRep() const;
    std::string fenStr() const;

    Bitboard subBoard(Color color, int subBoardIdx) const;
    Bitboard wonBoards(Color color) const;
    Bitboard drawnBoards() const;
    Bitboard completedBoards() const;
    Piece pieceAt(Square sq) const;

    int subBoardIdx() const;
    Color sideToMove() const;

    bool isLost() const;
    bool isDrawn() const;
private:
    BoardState& state();
    const BoardState& state() const;

    std::vector<BoardState> m_BoardStates;
    Color m_SideToMove;
};


inline Bitboard Board::subBoard(Color color, int subBoardIdx) const
{
    return state().subBoards[static_cast<int>(color)][subBoardIdx];
}

inline Bitboard Board::wonBoards(Color color) const
{
    return state().won[static_cast<int>(color)];
}

inline Bitboard Board::drawnBoards() const
{
    return state().drawn;
}

inline Bitboard Board::completedBoards() const
{
    return state().won[0] | state().won[1] | state().drawn;
}

inline Piece Board::pieceAt(Square sq) const
{
    Bitboard subBoardX = subBoard(Color::X, sq.subBoard());
    Bitboard subBoardO = subBoard(Color::O, sq.subBoard());

    if ((subBoardX & Bitboard::fromSquare(sq.subSquare())).any())
        return Piece::X;
    if ((subBoardO & Bitboard::fromSquare(sq.subSquare())).any())
        return Piece::O;
    return Piece::NONE;
}

inline int Board::subBoardIdx() const
{
    return state().subBoardIdx;
}

inline Color Board::sideToMove() const
{
    return m_SideToMove;
}

inline BoardState& Board::state()
{
    return m_BoardStates.back();
}

inline const BoardState& Board::state() const
{
    return m_BoardStates.back();
}

inline bool Board::isLost() const
{
    // only last player to move could have won the game
    return attacks::boardIsWon(wonBoards(~m_SideToMove));
}

inline bool Board::isDrawn() const
{
    // does not check for wins, can return false draws
    return completedBoards() == IN_BOARD;
}
