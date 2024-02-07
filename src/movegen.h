#include "util/static_vector.h"
#include "board.h"

using MoveList = StaticVector<Move, 81>;

void genMoves(const Board& board, MoveList& moveList)
{
    if ((board.completedBoards() & Bitboard::fromSquare(board.subBoardIdx())).any())
    {
        Bitboard remainingBoards = ~board.completedBoards() & IN_BOARD;
        while (remainingBoards)
            genSubBoardMoves(board, moveList, remainingBoards.poplsb());
    }
    else
    {
        genSubBoardMoves(board, moveList, board.subBoardIdx());
    }
}

void genSubBoardMoves(const Board& board, MoveList& moveList, int subBoard)
{
    Bitboard occupied = board.subBoard(Color::X, subBoard) | board.subBoard(Color::O, subBoard);
    Bitboard free = ~occupied & IN_BOARD;
    while (free)
    {
        uint32_t sq = free.poplsb();
        moveList.push_back(Move(subBoard, sq));
    }
}
