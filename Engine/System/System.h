#pragma once
#pragma region TechStuff
#ifndef _SYSTEM_H_
#define _SYSTEM_H_
#define WIN32_LEAN_AND_MEAN
#pragma endregion
#pragma region Includes
#include <windows.h>
#include "../Graphics/Graphics.h"
#include "../Input/Input.h"
#include "../Sound/Sound.h"
#include "../Timers/Cpu.h"
#include "../Timers/Fps.h"
#include "../Timers/Time.h"
#include "../Movement/FreeMovement.h"
#pragma endregion

	class System
	{
#pragma region  public funcs
	public:
		System();
		System(System&);
		~System();
		bool Init();
		void ShutDown();
		void Run();
		LRESULT CALLBACK MsgHandler(HWND, UINT, WPARAM, LPARAM);
#pragma endregion
#pragma region  private funcs
	private:
		bool Frame();
		void InitWindows(int&, int&);
		void ShutDownWindows();
#pragma endregion 
#pragma region  private Vars
	private:
		LPCWSTR m_appName;
		HINSTANCE m_hinstance;
		HWND m_hwnd;
		D3d* m_D3d;
		bool vsync = true;
		int m_ScreenWidth = 0,
			m_ScreenHeight = 0;
		bool fullScreen = false;
		/*
		Input* m_Input;
		Graphics* m_Graphics;
		Sound* m_sound;
		Fps* m_Fps;
		Time* m_Time;
		Cpu* m_Cpu;
		FreeMovement* m_movement;*/
#pragma endregion
#pragma region  public Vars
	
			
#pragma endregion
	};

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM  wparam, LPARAM lparam);
	static System* AppHandle = 0;

#endif 
