#include "board.h"
#include "tuner/trace.h"

namespace eval
{

inline Trace currTrace;

int evaluate(const Board& board);
int evalTrace(const Board& board);

}