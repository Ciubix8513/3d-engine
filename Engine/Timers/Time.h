#pragma once
#ifndef _TIME_H_
#define _TIME_H_
#pragma region includes
#include <Windows.h>
#pragma endregion

class Time
{
#pragma region public funcs
public:
	Time();
	Time(const Time&);
	~Time();

	bool Init();
	void Frame();
	float GetDeltaTime();

#pragma endregion
#pragma region Private Vars
private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;
#pragma endregion

};
#endif

