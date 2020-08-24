#include "Graphics.h"

	Graphics::Graphics()
	{
		m_D3d = 0;
		m_camera = 0;
		m_colorShader =0;
		m_model = 0;
		m_texShader = 0;
		m_LightShader = 0;
		m_Light = 0;
		m_Coursor = 0;
		m_Text = 0;
		for (size_t i = 0; i < 4; i++)
		{
			sentences[i] = 0;
		}
	}

	Graphics::Graphics(const Graphics&)
	{
	}

	Graphics::~Graphics()
	{
	}

	bool Graphics::Init(int scW, int scH, HWND hwnd)
	{
		bool result;
		XMMATRIX baseView;
		m_D3d = new D3d;
		if (!m_D3d)
			return false;
		m_camera = new Camera;
		if (!m_camera)			
			return false;
		m_camera->SetPosition(0, 0, -1);
		m_camera->Render();
		m_camera->GetViewMatrix(baseView);
		



		m_camera->SetPosition(0, 0, -5);
		m_camera->SetRotation(0, 0, 0);
	
		m_colorShader = new ColorShader;
		if (!m_colorShader)
			return false;
		m_model = new Model;
		if (!m_model)
			return false;
		m_texShader = new TextureShader;
		if (!m_texShader)
			return false;
		m_Text = new Text;
		if (!m_Text)
			return false;


		result = m_D3d->Init(scW, scH, Vsync, hwnd, FullScreen, ScreenDepth, ScreenNear);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize DirectX 11", L"Error", 0x00000000L);//0x00000000L = OK
			return false;
		}
		result = m_model->Init(m_D3d->getDevice(), m_D3d->getDeviceContext(), (char*)"../Engine/Testtex.tga",(char*)"../Engine/data/Sphere.obj");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", 0x00000000L);
			return false;
		}
		result = m_colorShader->Init(m_D3d->getDevice(),hwnd);
		if (!result) 
		{
			MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", 0x00000000L);
			return false;
		}
		result = m_texShader->Init(m_D3d->getDevice(), hwnd);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", 0x00000000L);
			return false;
		}
		m_LightShader = new LightShader;
		if (!m_LightShader)
			return false;
		result = m_LightShader->Init(m_D3d->getDevice(), hwnd);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", 0x00000000L);
			return false;
		}

		m_Coursor = new Bitmap();
		if (!m_Coursor)
			return false;
		result = m_Coursor->Init(m_D3d->getDevice(), scW, scH, (char*)"../Engine/data/Cursor.tga", 32, 32);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", 0x00000000L);
			return false;
		}



		result = m_Text->Init(m_D3d->getDevice(), m_D3d->getDeviceContext(), hwnd, scW, scH, baseView);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the Text object.", L"Error", 0x00000000L);
			return false;
		}


		m_Light = new Light;
		if (!m_Light)
			return false;
		m_Light->SetDiffuseColor(1, 1,1, 1);
		m_Light->SetDirection(1,0,1);
		m_Light->SetAmbientColor(.15f, .15f, .15f, 1.0f);
		m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light->SetSpecularPower(32.0f);

		for (int i = 0; i < 4; i++)
		{
			result = m_Text->InitSentence(&(sentences[i]), 64, m_D3d->getDevice());
			if (!result) 
			{
				return false;
			}
		}



		return true;
	}

	void Graphics::ShutDown()
	{
		if(m_Coursor)
		{
			m_Coursor->ShutDown();
			delete m_Coursor;
			m_Coursor = 0;
		}
		if(m_Light)
		{
			delete m_Light;
			m_Light = 0;
		}
		if (m_LightShader) 
		{
			m_LightShader->ShutDown();
			delete m_LightShader;
			m_LightShader = 0;
		}
		if(m_camera)
		{
			delete m_camera;
			m_camera = 0;
		}
		if(m_colorShader)
		{
			m_colorShader->Shutdown();
			delete m_colorShader;
			m_colorShader = 0;
		}
		if(m_model)
		{
			m_model->Shutdown();
			delete m_model;
			m_model = 0;
		}	
		if (m_D3d)
		{
			m_D3d->ShutDown();
			delete m_D3d;
			m_D3d = 0;
		}
		if(m_texShader)
		{
			m_texShader->ShutDown();
			delete m_texShader;
			m_texShader = 0;
		}
		if(m_Text)
		{
			m_Text->ShutDown();
			delete m_Text;
			m_Text = 0;
		}
		for (size_t i = 0; i < 4; i++)
		{
			Text::Releasesentence(&sentences[i]);		
		}

			

	}

	bool Graphics::Frame(int posX,int posY, int cpu, int fps, float time)
	{
		char tempString[16];
		char Output[16];
		bool result;
		_itoa_s(fps, tempString, 10);//convert int to string
		strcpy_s(Output, "Fps: ");
		strcat_s(Output, tempString);

		result = m_Text->UpdateSentence(sentences[0], Output, 20, 60, 1.0f, 1.0f, 1.0f, m_D3d->getDeviceContext());
		if (!result)
			return false;
		if (cpu != -1) {
			_itoa_s(cpu, tempString, 10);//convert int to string
			strcpy_s(Output, "Cpu: ");
			strcat_s(Output, tempString);
			result = m_Text->UpdateSentence(sentences[1], Output, 20, 40, 1.0f, 1.0f, 1.0f, m_D3d->getDeviceContext());
		}
		else
		{
			result = m_Text->UpdateSentence(sentences[1],(char*) "Cpu percentege is not available", 20, 40, 1.0f, 1.0f, 1.0f, m_D3d->getDeviceContext());

		}
		if (!result)
			return false;
		_itoa_s(((float)time / 1000), tempString, 10);//convert int to string
		strcpy_s(Output, "Time: ");
		strcat_s(Output, tempString);
		result = m_Text->UpdateSentence(sentences[2], Output, 20, 20, 1.0f, 1.0f, 1.0f, m_D3d->getDeviceContext());
		if (!result)
			return false;



		result = m_Text->UpdateSentence(sentences[3], Output, 20, 80, 1.0f, 1.0f, 1.0f, m_D3d->getDeviceContext());
		if (!result)
			return false;




		XMFLOAT3 a = XMFLOAT3(0.0f, .002f * time, 0.0f);
		m_model->SetRotation(XMLoadFloat3(&m_model->GetRotation()) + XMLoadFloat3(&a));

		return Render(posX, posY);

	}

	bool Graphics::Render(int posX, int posY)
	{
		XMMATRIX view, proj, world ,ortho;
		bool result;

		

		m_D3d->BeginScene(0, 0, 0, 0);
		m_camera->Render();
		m_camera->GetViewMatrix(view);
		m_D3d->GetWorldMat(world);
		m_D3d->GetProjectionMat(proj);
		m_D3d->GetOrthoMat(ortho);
		




		m_model->Render(m_D3d->getDeviceContext(), m_D3d->getDevice());
		result = m_LightShader->Render(m_D3d->getDeviceContext(), m_model->GetIndexCount(), world, view, proj, m_model->Getobjectmatrix(), m_model->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), m_camera->GetPosition());
		if (!result)
			return false;

		m_D3d->ToggleZBuffer(false);
		m_D3d->ToggleAlphaBlend(true);

		for (int i = 0; i < 4; i++)
		{
			result = m_Text->RenderSentence(m_D3d->getDeviceContext(), sentences[i], world, ortho);
			if (!result)
				return false;

		}
	
		


		/*result = m_Coursor->Render(m_D3d->getDeviceContext(), posX, -posY);
		if (!result)
			return false;
		result = m_texShader->Render(m_D3d->getDeviceContext(), m_Coursor->GetIndexCount(), world, view, ortho, XMMatrixIdentity(), m_Coursor->GetTexture());
		if (!result)
			return false;*/


		m_D3d->ToggleZBuffer(true);
		m_D3d->ToggleAlphaBlend(false);

		m_D3d->EndScene();


		return true;
	}
