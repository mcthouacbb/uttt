#include "bitboard.h"

namespace attacks
{

struct AttackData
{
    bool wonBoards[512];
};

extern AttackData attackData;

inline bool boardIsWon(Bitboard board)
{
    return attackData.wonBoards[board.value()];
}


}
