#pragma once
#ifndef _FPS_H_
#define _FPS_H_
#pragma region Includes
#pragma comment(lib,"winmm.lib")
#include <Windows.h>
#include <mmsystem.h>
#pragma endregion
class Fps
{
#pragma region Public Funcs
public:
	Fps();
	Fps(const Fps&);
	~Fps();

	void Init();
	void Frame();
	int GetFps();
#pragma endregion
#pragma region Private vars
private:
	int m_fps, m_count;
	unsigned long m_startTime;
#pragma endregion
};
#endif


