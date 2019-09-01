#ifndef _CLOCK_H
#define _CLOCK_H

#include <chrono>

//TODO: make a clock.cpp file 
namespace noctis
{
class Clock
{
	typedef std::chrono::time_point<std::chrono::system_clock> ClockTime;
public:
	Clock() { m_start = m_curr = m_prev = m_time.now(); }
	void Tick() 
	{ 
		m_prev = m_curr; 
		m_curr = m_time.now();
	}
	float DeltaTime() 
	{
		std::chrono::duration<float> dt = m_curr - m_prev;
		return dt.count();
	}

private:
	std::chrono::system_clock m_time;
	ClockTime m_start;
	ClockTime m_prev;
	ClockTime m_curr;
};
}
#endif //_CLOCK_H