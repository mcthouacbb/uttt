#pragma once

#include "board.h"
#include "time_man.h"

struct SearchResult
{
    int score;
    Move bestMove;
};

class Search
{
public:
    Search();

    void setBoard(const Board& board);
    SearchResult runSearch(const SearchLimits& limits, bool report = true);
    uint64_t runBenchSearch(const SearchLimits& limits);
private:
    int search(int depth, int ply);

    Board m_Board;
    TimeMan m_TimeMan;
    Move m_RootBestMove;

    bool m_ShouldStop;

    uint64_t m_Nodes;
};
