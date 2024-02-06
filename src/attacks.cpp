#include "attacks.h"
namespace attacks
{

bool boardIsWonSlow(Bitboard board)
{
    if ((board & RANK_1) == RANK_1)
        return true;
    if ((board & RANK_2) == RANK_2)
        return true;
    if ((board & RANK_3) == RANK_3)
        return true;
    if ((board & FILE_A) == FILE_A)
        return true;
    if ((board & FILE_B) == FILE_B)
        return true;
    if ((board & FILE_C) == FILE_C)
        return true;
    if ((board & DIAG_A) == DIAG_A)
        return true;
    if ((board & DIAG_B) == DIAG_B)
        return true;
    return false;
}

AttackData genAttackData()
{
    AttackData data;
    // loop through all possible 3x3 bbs
    for (int i = 0; i < 512; i++)
    {
        Bitboard bb = i;
        data.wonBoards[i] = boardIsWonSlow(bb);
    }
    return data;
}

AttackData attackData = genAttackData();


}
