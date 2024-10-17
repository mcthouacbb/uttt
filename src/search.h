#pragma once

#include "board.h"
#include "time_man.h"
#include "tt.h"

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

    void newGame()
    {
        m_TT.reset();
    }
private:
    int search(int alpha, int beta, int depth, int ply);

    Board m_Board;
    TimeMan m_TimeMan;
    Move m_RootBestMove;
    TT m_TT;

    MultiArray<int32_t, 2, 81> m_History;

    bool m_ShouldStop;

    uint64_t m_Nodes;
};
