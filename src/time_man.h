#pragma once

#include <chrono>

using TimePoint = std::chrono::steady_clock::time_point;
using Duration = std::chrono::milliseconds;

struct SearchLimits
{
    int maxDepth;
    Duration maxTime;
    uint64_t nodes;
    uint64_t softNodes;
    struct Clock
    {
        Duration time;
        Duration inc;
        bool enabled;
    } clock;
};

class TimeMan
{
public:
    TimeMan();

    void setLimits(const SearchLimits& limits);
    void startSearch();

    bool shouldStopSoft(uint64_t nodes) const;
    bool shouldStopHard(uint64_t nodes) const;

    Duration elapsed() const;
private:
    SearchLimits m_Limits;

    TimePoint m_StartTime;
    Duration m_SoftLimit;
    Duration m_HardLimit;
};