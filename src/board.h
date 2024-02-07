#pragma once

#include "types.h"
#include "bitboard.h"
#include <vector>

struct BoardState
{
    Bitboard subBoards[9][2];
    Bitboard won[2];
    Bitboard drawn;
    
    int subBoardIdx;
};

class Board
{
public:
    Board();

    void setToFen(std::string_view fen);

    void makeMove(Move move);
    void unmakeMove(Move move);

    std::string stringRep() const;

    Bitboard subBoard(Color color, int subBoardIdx) const;
    Bitboard wonBoards(Color color) const;
    Bitboard drawnBoards() const;
    Bitboard completedBoards() const;
    Piece pieceAt(int subBoardIdx, int square) const;

    int subBoardIdx() const;
    Color sideToMove() const;
private:
    BoardState& state();
    const BoardState& state() const;

    std::vector<BoardState> m_BoardStates;
    Color m_SideToMove;
};


inline Bitboard Board::subBoard(Color color, int subBoardIdx) const
{
    return state().subBoards[subBoardIdx][static_cast<int>(color)];
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

inline Piece Board::pieceAt(int subBoardIdx, int square) const
{
    Bitboard subBoardX = subBoard(Color::X, subBoardIdx);
    Bitboard subBoardO = subBoard(Color::O, subBoardIdx);

    if ((subBoardX & Bitboard::fromSquare(square)).any())
        return Piece::X;
    if ((subBoardO & Bitboard::fromSquare(square)).any())
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
