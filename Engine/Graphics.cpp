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
		m_Bitmap = 0;
		m_Text;
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
		result = m_model->Init(m_D3d->getDevice(), m_D3d->getDeviceContext(), (char*)"../Engine/Testtex.tga",(char*)"../Engine/cubeArray.obj");
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

		m_Bitmap = new Bitmap();
		if (!m_Bitmap)
			return false;
		result = m_Bitmap->Init(m_D3d->getDevice(), scW, scH, (char*)"../Engine/data/Cursor.tga", 32, 32);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", 0x00000000L);
			return false;
		}

		int a, b;
		a = scW;
		b = scH;

		result = m_Text->Init(m_D3d->getDevice(), m_D3d->getDeviceContext(), hwnd, a, b, baseView, 95);
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
		return true;
	}

	void Graphics::ShutDown()
	{
		if(m_Bitmap)
		{
			m_Bitmap->ShutDown();
			delete m_Bitmap;
			m_Bitmap = 0;
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
	}

	bool Graphics::Frame(int posX,int posY)
	{
		bool result;

		result = m_Text->SetMousePosition(posX, posY,m_D3d->getDeviceContext());
		if (!result)
			return false;
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
		



		//Rendering 3d model
		m_model->Render(m_D3d->getDeviceContext(), m_D3d->getDevice());
		result = m_LightShader->Render(m_D3d->getDeviceContext(), m_model->GetIndexCount(), world, view, proj, m_model->Getobjectmatrix(), m_model->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Light->GetAmbientColor(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), m_camera->GetPosition());
	

		if (!result)
			return false;




		m_D3d->ToggleZBuffer(false);
		m_D3d->ToggleAlphaBlend(true);
		//Rendering Text object
		if (!result)
			return false;

		result = m_Bitmap->Render(m_D3d->getDeviceContext(), posX, -posY);
		result = m_texShader->Render(m_D3d->getDeviceContext(), m_Bitmap->GetIndexCount(), world, view, ortho, XMMatrixIdentity(), m_Bitmap->GetTexture());


		result = m_Text->Render(m_D3d->getDeviceContext(), world, ortho);

		m_D3d->ToggleZBuffer(true);
		m_D3d->ToggleAlphaBlend(false);

		m_D3d->EndScene();

		XMFLOAT3 a = XMFLOAT3(0.0f,.02f, 0.0f);
		m_model->SetRotation(XMLoadFloat3(&m_model->GetRotation()) + XMLoadFloat3(&a));

		return true;
	}
