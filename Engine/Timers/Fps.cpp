#include "Fps.h"

Fps::Fps()
{
}

Fps::Fps(const Fps&)
{
}

Fps::~Fps()
{
}

void Fps::Init()
{
    m_fps = 0;
    m_count = 0;
    m_startTime = timeGetTime();
    return;
}

void Fps::Frame()
{
    m_count++;
    if(timeGetTime()>=(m_startTime+1000))
    {
        m_fps = m_count;
        m_count = 0;
        m_startTime = timeGetTime();
    }
}

int Fps::GetFps()
{
    return m_fps;
}
