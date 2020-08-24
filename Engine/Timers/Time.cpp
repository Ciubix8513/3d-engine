#include "Time.h"
#include "Time.h"

Time::Time()
{
}
Time::Time(const Time&)
{
}
Time::~Time()
{
}

bool Time::Init()
{
    QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
    if (m_frequency == 0)
        return false;
    m_ticksPerMs = (float)(m_frequency / 1000);
    QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
    return true;
}

void Time::Frame()
{
    INT64 currentTime;
    float TimeDifference;

    QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
    TimeDifference = (float)(currentTime - m_startTime);
    m_frameTime = TimeDifference / m_ticksPerMs;
    m_startTime = currentTime;

    return;
}

float Time::GetDeltaTime()
{
    return m_frameTime;
}
