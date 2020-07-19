#include "System.h"

	System::System()
	{
		m_Input = 0;
		m_Graphics = 0;
	}

	System::System(System&)
	{
	}

	System::~System()
	{
	}

	bool System::Init()
	{
		int ScreenWidth = 0,
			ScreenHeight = 0;
		bool result;
		InitWindows(ScreenWidth, ScreenHeight);

		m_Input = new Input;
		if (!m_Input)
			return false;
		m_Input->Init();


		m_Graphics = new Graphics;
		if (!m_Graphics)
			return false;
		result = m_Graphics->Init(ScreenWidth, ScreenHeight, m_hwnd);

		if (!result)
			return false;
		return true;
	}

	void System::ShutDown()
	{
		if (m_Graphics)
		{
			m_Graphics->ShutDown();
			delete m_Graphics;
			m_Graphics = 0;
		}
		if (m_Input)
		{
			delete m_Input;
			m_Input = 0;
		}
		ShutDownWindows();
		return;
	}

	void System::Run()
	{
		MSG msg;
		bool done = 0,
			result;
		ZeroMemory(&msg, sizeof(MSG));
		while (!done)
		{
			if (PeekMessage(&msg, NULL, 0, 0, 0x001))//0x001 = remove message
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (msg.message == 0x0012)//0x0012 = Quit message
				done = true;
			else
			{
				result = Frame();
				done = !result;
			}

		}
		return;
	}

	LRESULT System::MsgHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		switch (umsg)
		{
		case 0x0100: //0x0100 = Key pressed
		{
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}
		case 0x0101: //0x0101 = Key up
		{
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
		}

		return LRESULT();
	}

	bool System::Frame()
	{
		if (m_Input->isKeyDown(0x1B))//0x1B = Escape
			return false;
		if (m_Input->isKeyDown(0x20))//0x20 = VK_SPACE = space
			m_Graphics->ShaderIndex = !m_Graphics->ShaderIndex;
		return m_Graphics->Frame();
	}

	void System::InitWindows(int& screenWidth, int& screenHeight)
	{
		WNDCLASSEX wc;
		DEVMODE dmScreenSettings;
		int posX, posY;
		AppHandle = this;
		m_hinstance = GetModuleHandle(NULL);
		m_appName = L"3d Engine";
		wc.style = 0x0002 | 0x0001 | 0x0020;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hinstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(2);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = m_appName;
		wc.cbSize = sizeof(WNDCLASSEX);

		RegisterClassEx(&wc);

		screenWidth = GetSystemMetrics(0);
		screenHeight = GetSystemMetrics(1);

		if (FullScreen)
		{
			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
			dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
			dmScreenSettings.dmBitsPerPel = 32;
			dmScreenSettings.dmFields = 0x00040000L | 0x00080000L | 0x00100000L;
			ChangeDisplaySettings(&dmScreenSettings, 0x00000004);
			posX = posY = 0;
		}
		else
		{
			screenWidth = 800;
			screenHeight = 600;
			posX = (GetSystemMetrics(0) - screenWidth) / 2;
			posY = (GetSystemMetrics(1) - screenHeight) / 2;
		}

		m_hwnd = CreateWindowEx(0x00040000L, m_appName, m_appName, 0x02000000L | 0x04000000L | 0x80000000L, posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
		ShowWindow(m_hwnd, 5);
		SetForegroundWindow(m_hwnd);
		SetFocus(m_hwnd);
		ShowCursor(false);
	}

	void System::ShutDownWindows()
	{
		ShowCursor(true);
		if (FullScreen)
			ChangeDisplaySettings(NULL, 0);
		DestroyWindow(m_hwnd);
		UnregisterClass(m_appName, m_hinstance);
		m_hinstance = NULL;
		AppHandle = NULL;
		return;
	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM  wparam, LPARAM lparam)
	{
		switch (msg)
		{
		case 0x0002:// 0x0002 = Destroy	
		{
			PostQuitMessage(0);
			return 0;
		}
		case 0x0010: //0x0010 = Close
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
			return AppHandle->MsgHandler(hwnd, msg, wparam, lparam);
		}

		return LRESULT();
	}
