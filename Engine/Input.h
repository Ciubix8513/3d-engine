#pragma once
#pragma region TechStuff
#ifndef _INPUT_H_
#define _INPUT_H_
#pragma endregion
#pragma region includes
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma endregion




	class Input
	{
#pragma region  public funcs
	public:
		Input();
		Input(Input&);
		~Input();
		bool Init(HINSTANCE hinstance, HWND hwnd, int screenW, int screenH);
		void ShutDown();
		bool Frame();
		bool ReadKeyboard();
		bool ReadMouse();
		void ProcessInput();
		bool isKeyDown(unsigned int);
		void GetMousePosition(int&,int&);
#pragma endregion
#pragma region  private vars
	private:

		IDirectInput8* m_directInput;
		IDirectInputDevice8* m_keyboard;
		IDirectInputDevice8* m_Mouse;
		unsigned char m_keyboardState[256];
		DIMOUSESTATE m_mouseState;
		int m_screenW, m_screenH;
		int m_mouseX, m_mouseY;
#pragma endregion
	};

#endif