#pragma once

#include <chrono>
#include <functional>

class Timer
{
private:
    const float m_StartTime;
    float m_CurrTime;
    const std::function<void(void)>& m_Callback;
    const bool m_Repeat;
    bool m_Active;

public:

    Timer(const float& time, std::function<void(void)> callback, bool repeat = false)
        : m_StartTime(time), m_Callback(callback), m_Repeat(repeat)
    {
        m_CurrTime = m_StartTime;
        m_Active = false;
    }
    
    void Update(float frameTime)
    {
        if(!m_Active) return;
        m_CurrTime -= frameTime;
        if(m_CurrTime <= 0.f)
        {
            //# CALLBACK
            m_Callback();

            if(!m_Repeat)
                m_Active = false;
            else
                m_CurrTime = m_StartTime;
        }
    }

    void Start()
    {
        m_Active = true;
    }

    void Stop()
    {
        m_Active = false;
    }
};