#include "System.h"

	System::System()
	{
		/*
		m_Input = 0;
		m_Graphics = 0;
		m_sound = 0;
		m_Cpu = 0;
		m_Fps = 0;
		m_Time = 0;
		m_movement = 0;
		*/
		m_D3d = 0;
		testScene = 0;
	}

	System::System(System&)
	{
	}

	System::~System()
	{
	}

	bool System::Init()
	{
		
		bool result;
		InitWindows(m_ScreenWidth, m_ScreenHeight);

		m_D3d = new D3d;
		if (!m_D3d)
			return false;
		if (!m_D3d->Init(m_ScreenWidth, m_ScreenHeight, vsync, m_hwnd, fullScreen))
			return false;
		testScene = new Scene(m_D3d);
		InitScene();
		/*
		m_Input = new Input;
		if (!m_Input)
			return false;
		result =  m_Input->Init(m_hinstance,m_hwnd, ScreenWidth,ScreenHeight);
		if (!result) 		
		{
			MessageBox(m_hwnd, L"Could not initialize input object", L"Application Initialization error", MB_OK);
			return false;
		}
		m_Graphics = new Graphics;
		if (!m_Graphics)
			return false;
		result = m_Graphics->Init(ScreenWidth, ScreenHeight, m_hwnd);

		if (!result)
			return false;
		m_sound = new Sound;
		if (!m_sound)
			return false;

		result = m_sound->Init(m_hwnd);
		if (!result)
			return false;

		m_Fps = new Fps;
		if (!m_Fps)
			return false;
		m_Fps->Init();

		m_Cpu = new Cpu;
		if (!m_Cpu)
			return false;
		m_Cpu->Init();
		m_Time = new Time;
		if (!m_Time)
			return false;
		result= m_Time->Init();
		if (!result)
			return false;
		m_movement = new FreeMovement;
		if (!m_movement)
			return false;
		m_movement->Init();
		*/

		return true;
	}

	void System::ShutDown()
	{
		/*
		if(m_Fps)
		{
			delete m_Fps;
			m_Fps = 0;
		}
		if(m_Cpu)
		{
			m_Cpu->ShutDown();
			delete m_Cpu;
			m_Cpu = 0;
		}
		if (m_Time)
		{
			delete m_Time;
			m_Time = 0;
		}
		if (m_Graphics)
		{
			m_Graphics->ShutDown();
			delete m_Graphics;
			m_Graphics = 0;
		}
		if (m_Input)
		{
			m_Input->ShutDown();
			delete m_Input;
			m_Input = 0;
		}
		if(m_movement)
		{
			delete m_movement;
			m_movement = 0;
		}

		if(m_sound)
		{
			m_sound->Shutdown();
			delete m_sound;
			m_sound = 0;
		}*/
		if (m_D3d)
		{
			m_D3d->ShutDown();
			delete m_D3d;
			m_D3d = 0;
		}
		if(testScene)
		{
			testScene->ShutDown();
			delete testScene;
			testScene = 0;
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
			return DefWindowProc(hwnd, umsg, wparam, lparam);
	}

	bool System::Frame()
	{
		/*bool result;
		int mouseX, mouseY;

		m_Fps->Frame();
		m_Cpu->Frame();
		m_Time->Frame();

		result = m_Input->Frame();
		if (!result)
			return false;

		m_Input->GetMousePosition(mouseX, mouseY);
		if (m_Input->isKeyDown(DIK_ESCAPE))
			return false;	

		m_movement->MoveCamera(m_Graphics->m_camera, m_Input, m_Time->GetDeltaTime());
		
		return m_Graphics->Frame(mouseX,mouseY,m_Cpu->GetCpuPercentage(),m_Fps->GetFps(),m_Time->GetDeltaTime());*/

		testScene->Update();
		testScene->RenderSceneFromCameraPtr(camera);
		return true;
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
		ShowCursor(true);
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

	void System::InitScene()
	{
		//Hard coding a scene
		testScene->AddEntity("Entity0");
		testScene->AddEntity("Entity1");

		Entity* e0 = testScene->GetEntityByName("Entity0");
		Entity* e1 = testScene->GetEntityByName("Entity1");

		e0->AddComponent<MeshComponent>();
		e0->AddComponent<MaterialComponent>();

		(*e0->GetComponent<MeshComponent>())->SetMesh(*FileManager::LoadMesh("C:/Users/Zver/Desktop/Engine/Project/Engine/Engine/data/Cube.obj"));
		(*e0->GetComponent<MaterialComponent>())->InitShader((WCHAR*)L"C:/Users/Zver/Desktop/Engine/Project/Engine/Engine/Shaders/ColorVS.hlsl", (WCHAR*)L"C:/Users/Zver/Desktop/Engine/Project/Engine/Engine/Shaders/Color.hlsl", "Color");
		
		e1->AddComponent<CameraComponent>();
		camera = e1->GetComponent<CameraComponent>();
		(*e1->Transform)->Position = Vector3(0, 0, -5);
	
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
