#include "util/static_vector.h"
#include "board.h"

using MoveList = StaticVector<Move, 81>;

inline void genSubBoardMoves(const Board& board, MoveList& moveList, int subBoard)
{
    Bitboard occupied = board.subBoard(Color::X, subBoard) | board.subBoard(Color::O, subBoard);
    Bitboard free = ~occupied & IN_BOARD;
    while (free.any())
    {
        uint32_t subSq = free.poplsb();
        moveList.push_back(Move{Square(static_cast<uint8_t>(subBoard), static_cast<uint8_t>(subSq))});
    }
}

inline void genMoves(const Board& board, MoveList& moveList)
{
    if (board.subBoardIdx() == -1 ||
        (board.completedBoards() & Bitboard::fromSquare(board.subBoardIdx())).any())
    {
        Bitboard remainingBoards = ~board.completedBoards() & IN_BOARD;
        while (remainingBoards.any())
            genSubBoardMoves(board, moveList, remainingBoards.poplsb());
    }
    else
    {
        genSubBoardMoves(board, moveList, board.subBoardIdx());
    }
}
