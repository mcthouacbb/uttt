#include "time_man.h"


TimeMan::TimeMan()
    : m_Limits{}, m_StartTime{}, m_HardLimit{}
{

}

void TimeMan::setLimits(const SearchLimits& limits)
{
    m_Limits = limits;
    m_HardLimit = m_Limits.clock.enabled ? m_Limits.clock.time / 20 : Duration(0);
}

void TimeMan::startSearch()
{
    m_StartTime = std::chrono::steady_clock::now();
}

bool TimeMan::shouldStop() const
{
    Duration time = elapsed();
    if (m_Limits.maxTime > Duration(0) && time > m_Limits.maxTime)
        return true;
    if (m_Limits.clock.enabled && time > m_HardLimit)
        return true;
    return false;
}

Duration TimeMan::elapsed() const
{
    return std::chrono::duration_cast<Duration>(std::chrono::steady_clock::now() - m_StartTime);
}