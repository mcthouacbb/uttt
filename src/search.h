#include "board.h"
#include <chrono>

using TimePoint = std::chrono::steady_clock::time_point;
using Duration = std::chrono::milliseconds;

struct SearchLimits
{
    int maxDepth;
    Duration maxTime;
    struct Clock
    {
        Duration time;
        Duration inc;
        bool enabled;
    } clock;
};

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
    SearchResult runSearch(const SearchLimits& limits);
private:
    int search(int depth, int ply);

    // TODO: put time management in separate file
    void initTimeMan(const SearchLimits& limits)
    {
        m_StartTime = std::chrono::steady_clock::now();
        Duration maxTime = limits.maxTime;
        if (limits.maxTime <= Duration(0))
            maxTime = Duration(1000000000);
        if (limits.clock.enabled)
            maxTime = std::min(maxTime, limits.clock.time / 20);
        m_HardLimit = maxTime;
        std::cout << "info string time " << m_HardLimit.count() << std::endl;
    }

    Duration elapsed() const
    {
        return std::chrono::duration_cast<Duration>(std::chrono::steady_clock::now() - m_StartTime);
    }

    bool timeUp() const
    {
        return elapsed() >= m_HardLimit;
    }

    Board m_Board;
    Move m_RootBestMove;

    TimePoint m_StartTime;
    Duration m_HardLimit;
    bool m_ShouldStop;

    uint64_t nodes;
};
