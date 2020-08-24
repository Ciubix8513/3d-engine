#include "Input.h"


	Input::Input()
	{
		m_directInput = 0;
		m_keyboard = 0;
		m_Mouse = 0;
	}

	Input::Input(Input&)
	{
	}

	Input::~Input()
	{
	}

	bool Input::Init(HINSTANCE hinstance,HWND hwnd, int screenW,int screenH)
	{
		HRESULT res;
		m_screenH = screenH;
		m_screenW = screenW;

		m_mouseX = 0;
		m_mouseY = 0;
		m_PrevRealMouseX = m_RealMouseX = 0;
		m_PrevRealMouseY = m_RealMouseY = 0;

		res = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
		if (FAILED(res))
		{
			MessageBox(hwnd, L"Could not create direct  input 8 object", L"Application Initialization error", MB_OK);
			return false;
		}
		res = m_directInput->CreateDevice(GUID_SysKeyboard,&m_keyboard,NULL);
		if (FAILED(res))
		{
			MessageBox(hwnd, L"Could not initialize input object", L"Application Initialization error", MB_OK);
			return false;
		}
		res = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(res))
		{
			MessageBox(hwnd, L"Could not initialize input object", L"Application Initialization error", MB_OK);
			return false;
		}
		res = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if (FAILED(res))
		{
			MessageBox(hwnd, L"Could not initialize input object", L"Application Initialization error", MB_OK);
			return false;
		}
		res = m_keyboard->Acquire();
		if (FAILED(res))
		{
			MessageBox(hwnd, L"Could not initialize input object", L"Application Initialization error", MB_OK);
			return false;
		}
		res = m_directInput->CreateDevice(GUID_SysMouse, &m_Mouse, NULL);
		if (FAILED(res))
		{
			MessageBox(hwnd, L"Could not initialize input object", L"Application Initialization error", MB_OK);
			return false;
		}
		res = m_Mouse->SetDataFormat(&c_dfDIMouse);
		if (FAILED(res))
		{
			MessageBox(hwnd, L"Could not initialize input object", L"Application Initialization error", MB_OK);
			return false;
		}
		res = m_Mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if (FAILED(res))
		{
			MessageBox(hwnd, L"Could not initialize input object", L"Application Initialization error", MB_OK);
			return false;
		}
		res = m_Mouse->Acquire();
		if (FAILED(res))
		{
			MessageBox(hwnd, L"Could not initialize input object", L"Application Initialization error", MB_OK);
			return false;
		}
		return true;
	}

	void Input::ShutDown()
	{
		
		if(m_keyboard)
		{
			m_Mouse->Unacquire();
			m_Mouse->Release();
			m_Mouse = 0;
		}
		if (m_keyboard)
		{
			m_keyboard->Unacquire();
			m_keyboard->Release();
			m_keyboard = 0;
		}
		if(m_directInput)
		{
			m_directInput->Release();
			m_directInput = 0;
		}
		return;
	}

	bool Input::Frame()
	{
		bool res;
		res = ReadKeyboard();
		if (!res)
			return false;
		res = ReadMouse();
		if (!res)
			return false;
		
		ProcessInput();

		return true;
		
	}

	bool Input::ReadKeyboard()
	{
		HRESULT res;

		res = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
		if (FAILED(res))
			if ((res == DIERR_INPUTLOST) || (res == DIERR_NOTACQUIRED))
				m_keyboard->Acquire();
			else
				return false;
		return true;
	}

	bool Input::ReadMouse()
	{
		HRESULT res;

		res = m_Mouse->GetDeviceState(sizeof(m_mouseState), (LPVOID)&m_mouseState);
		if (FAILED(res))
			if ((res == DIERR_INPUTLOST) || (res == DIERR_NOTACQUIRED))
				m_Mouse->Acquire();
			else
				return false;
		return true;
	}

	void Input::ProcessInput()
	{	
		m_PrevRealMouseX = m_RealMouseX;
		m_PrevRealMouseY = m_RealMouseY;


		m_RealMouseX += m_mouseState.lX;
		m_RealMouseY += m_mouseState.lY;

		m_mouseX += m_mouseState.lX;
		m_mouseY += m_mouseState.lY;
		if (m_mouseX < 0) m_mouseX = 0;
		if (m_mouseY < 0) m_mouseY = 0;
		if (m_mouseX > m_screenW) m_mouseX = m_screenW;
		if (m_mouseY > m_screenH) m_mouseY = m_screenH;
		return;
	}

	bool Input::isKeyDown(unsigned int key)
	{
		if (m_keyboardState[key] & 0x80)
			return true;
		return false;
	}

	void Input::GetMousePosition(int& posX, int& posY)
	{
		posX = m_mouseX;
		posY = m_mouseY;
		return;
	}

	void Input::GetMouseDelta(int& x, int& y)
	{
		x = (m_PrevRealMouseX - m_RealMouseX) ;
		y = (m_PrevRealMouseY - m_RealMouseY) ;
	}


	