#pragma once
#pragma region TechStuff
#ifndef _SYSTEM_H_
#define _SYSTEM_H_
#define WIN32_LEAN_AND_MEAN
#pragma endregion
#pragma region Includes
#include <windows.h>
#include "Graphics.h"
#include "Input.h"
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
		Input* m_Input;
		Graphics* m_Graphics;
#pragma endregion
#pragma region  public Vars

#pragma endregion
	};

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM  wparam, LPARAM lparam);
	static System* AppHandle = 0;

#endif 
