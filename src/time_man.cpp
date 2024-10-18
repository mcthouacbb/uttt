#include "time_man.h"


TimeMan::TimeMan()
    : m_Limits{}, m_StartTime{}, m_HardLimit{}
{

}

void TimeMan::setLimits(const SearchLimits& limits)
{
    m_Limits = limits;
    if (m_Limits.clock.enabled)
    {
        m_SoftLimit = m_Limits.clock.time / 34 + m_Limits.clock.inc / 2;
        m_HardLimit = m_Limits.clock.time / 4;
    }
}

void TimeMan::startSearch()
{
    m_StartTime = std::chrono::steady_clock::now();
}

bool TimeMan::shouldStopSoft() const
{
    Duration time = elapsed();
    if (m_Limits.clock.enabled && time > m_SoftLimit)
        return true;
    return false;
}

bool TimeMan::shouldStopHard() const
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