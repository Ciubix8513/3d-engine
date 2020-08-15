#include "D3d.h"

    D3d::D3d()
    {
        m_swapChain = 0;
        m_depthStencilBuffer = 0;
        m_DepthStencilState = 0;
        m_DepthStencilView = 0;
        m_device = 0;
        m_DeviceContext = 0;
        m_RasterizerState = 0;
        m_DepthStencilState = 0;
        m_AlphaBlendOff = 0;
        m_AlphaBlendOn = 0;
    }
    D3d::D3d(D3d&)
    {
    }
    D3d::~D3d()
    {
    }
    bool D3d::Init(int screenWidth, int screenHeight, bool Vsync, HWND hwnd, bool fullScreen, float screenDepth, float screenNear)
    {
        HRESULT result;
        IDXGIFactory* factory;
        IDXGIAdapter* adapter;
        IDXGIOutput* adapterOutput;
        unsigned int numModes, numerator, denominator, stringLength;

        int error;
        DXGI_MODE_DESC* displayModelist;
        DXGI_ADAPTER_DESC adapterDesc;
        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        D3D_FEATURE_LEVEL featureLevel;
        ID3D11Texture2D* backBufferPtr;
        D3D11_TEXTURE2D_DESC depthBufferDesc;
        D3D11_DEPTH_STENCIL_DESC depthStncilDesc;
        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
        D3D11_RASTERIZER_DESC rasterizerDesc;
        D3D11_DEPTH_STENCIL_DESC depthDisableStateDesc;
        D3D11_BLEND_DESC blendDesc;


        D3D11_VIEWPORT viewport;
        float FOV, screenAspect;
        m_Vsync = Vsync;

        result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
        if (FAILED(result))
        {            
            MessageBox(hwnd, L"Could not Create DXGIFactory", L"Eror", MB_OK);//0x00000000L = OK
            return false;
        }
        
        result = factory->EnumAdapters(0, &adapter);
        if (FAILED(result))
        {
            MessageBox(hwnd, L"Could not Create adapter", L"Eror", MB_OK);//0x00000000L = OK
            return false;
        }
        result = adapter->EnumOutputs(0, &adapterOutput);
        if (FAILED(result))
        {
            MessageBox(hwnd, L"Could not Create adapter output", L"Eror", MB_OK);//0x00000000L = OK
            return false;
        }
        result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
        if (FAILED(result))
        {
            MessageBox(hwnd, L"Could not Get display mode list", L"Eror", MB_OK);//0x00000000L = OK
            return false;
        }

        displayModelist = new DXGI_MODE_DESC[numModes];
        if (!displayModelist)
        {
            MessageBox(hwnd, L"Could not Create display mode list", L"Eror", MB_OK);//0x00000000L = OK
            return false;
        }

        result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModelist);
        if (FAILED(result))
        {
            MessageBox(hwnd, L"Could not Get display mode list1 ", L"Eror", MB_OK);//0x00000000L = OK
            return false;
        }

        for (int i = 0; i < numModes; i++)
        {
            if(displayModelist[i].Width == (unsigned int)screenWidth)
                if (displayModelist[i].Height == (unsigned int)screenHeight)
                {
                    numerator = displayModelist[i].RefreshRate.Numerator;
                    denominator = displayModelist[i].RefreshRate.Denominator;
                }
        }
        result = adapter->GetDesc(&adapterDesc);
        if(FAILED(result))
        {
            MessageBox(hwnd, L"Could not get adapter description", L"Eror", MB_OK);//0x00000000L = OK
            return false;
        }
        m_VcardMem = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
      //  error = wcstombs_s()





        error = wcstombs_s((size_t*)&stringLength, m_VCardDescr, (size_t)128, adapterDesc.Description, 128);
       
        if (error != 0)
        {
            MessageBox(hwnd, L"Could not coppy video card description", L"Eror", MB_OK);//0x00000000L = OK
            return false;
        }
        
        delete[] displayModelist;
        displayModelist = 0;
        adapterOutput->Release();
        adapterOutput = 0;
        adapter->Release();
        adapter = 0;
        factory->Release();
        factory = 0;

        ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Width = screenWidth;
        swapChainDesc.BufferDesc.Height = screenHeight;
        swapChainDesc.BufferDesc.Format =DXGI_FORMAT_R8G8B8A8_UNORM;
        if(m_Vsync)
        {
            swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
            swapChainDesc.BufferDesc.RefreshRate.Denominator= denominator;
        }
        else
        {
            swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
            swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        }

        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = hwnd;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;

        swapChainDesc.Windowed =!fullScreen;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.Flags = 0;
        featureLevel = D3D_FEATURE_LEVEL_11_0;
        
        result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_DeviceContext);
        if (FAILED(result))
        {
            MessageBox(hwnd, L"Could not Create Swap chain", L"Eror", 0x00000000L);//0x00000000L = OK
            return false;
        }
        result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
        if (FAILED(result))
        {
            MessageBox(hwnd, L"Could not get back buffer", L"Eror", 0x00000000L);//0x00000000L = OK
            return false;
        }
        result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
        if (FAILED(result))
        {
            MessageBox(hwnd, L"Could not Create render target view", L"Eror", 0x00000000L);//0x00000000L = OK
            return false;
        }
        backBufferPtr->Release();
        backBufferPtr = 0;

        ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

        
        depthBufferDesc.Width = screenWidth;
        depthBufferDesc.Height = screenHeight;
        depthBufferDesc.MipLevels = 1;
        depthBufferDesc.ArraySize = 1;
        depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthBufferDesc.SampleDesc.Count = 1;
        depthBufferDesc.SampleDesc.Quality = 0;
        depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthBufferDesc.CPUAccessFlags = 0;
        depthBufferDesc.MiscFlags = 0;
        result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
        if (FAILED(result))
        {
            MessageBox(hwnd, L"Could not Create depth texture", L"Eror", 0x00000000L);//0x00000000L = OK

            return false;
        }
        

        ZeroMemory(&depthStncilDesc,sizeof(depthStncilDesc));
        depthStncilDesc.DepthEnable = true;
        depthStncilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStncilDesc.DepthFunc = D3D11_COMPARISON_LESS;

        depthStncilDesc.StencilEnable = true;
        depthStncilDesc.StencilReadMask = 0xFF;
        depthStncilDesc.StencilWriteMask = 0xFF;
        depthStncilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthStncilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        depthStncilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthStncilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        depthStncilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthStncilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        depthStncilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthStncilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        result = m_device->CreateDepthStencilState(&depthStncilDesc, &m_DepthStencilState);
        if (FAILED(result))
        {
            
            MessageBox(hwnd, L"Could not Create depth stencil state", L"Eror", 0x00000000L);//0x00000000L = OK
            return false;
        }

        m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);
        ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
        
        depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc.Texture2D.MipSlice = 0;       
        result = m_device->CreateDepthStencilView(m_depthStencilBuffer,&depthStencilViewDesc, &m_DepthStencilView);
        if (FAILED(result))
        {
            MessageBox(hwnd, L"Could not Create depth stencil view", L"Eror", 0x00000000L);//0x00000000L = OK
            return false;
        }


        m_DeviceContext->OMSetRenderTargets(1,&m_renderTargetView, m_DepthStencilView);
        rasterizerDesc.AntialiasedLineEnable = false;
        rasterizerDesc.CullMode = D3D11_CULL_BACK;
        rasterizerDesc.DepthBias = 0;
        rasterizerDesc.DepthBiasClamp = 0.0f;
        rasterizerDesc.FillMode = D3D11_FILL_SOLID;
        rasterizerDesc.FrontCounterClockwise = false;
        rasterizerDesc.MultisampleEnable = false;
        rasterizerDesc.ScissorEnable = false;
        rasterizerDesc.SlopeScaledDepthBias = 0.0f;

        result = m_device->CreateRasterizerState(&rasterizerDesc, &m_RasterizerState);
        if (FAILED(result))
        {
            MessageBox(hwnd, L"Could not Create rasterizer state", L"Eror", MB_OK);//0x00000000L = OK
            return false;
        }
        m_DeviceContext->RSSetState(m_RasterizerState);

        viewport.Width = (float)screenWidth;
        viewport.Height = (float)screenHeight;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;

        m_DeviceContext->RSSetViewports(1, &viewport);
        FOV = (float)XM_PI / 3.0f; //FOV of 60 degrees
        screenAspect = (float)screenWidth / (float)screenHeight;
        m_projectionMat =  XMMatrixPerspectiveFovLH(FOV, screenAspect, screenNear, screenDepth);
        m_worldMat = XMMatrixIdentity();
        m_orthomat = XMMatrixOrthographicLH((float)screenWidth, (float) screenHeight, screenNear, screenDepth);

        ZeroMemory(&depthDisableStateDesc, sizeof(depthDisableStateDesc));

        depthDisableStateDesc.DepthEnable = false;
        depthDisableStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthDisableStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
        depthDisableStateDesc.StencilEnable = true;
        depthDisableStateDesc.StencilReadMask = 0XFF;
        depthDisableStateDesc.StencilWriteMask = 0XFF;
        depthDisableStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthDisableStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        depthDisableStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthDisableStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        depthDisableStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthDisableStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        depthDisableStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthDisableStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        result = m_device->CreateDepthStencilState(&depthDisableStateDesc, &m_depthDisabledStencilState);
        if (FAILED(result))
            return false;

        ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

        blendDesc.RenderTarget[0].BlendEnable = TRUE;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

        result = m_device->CreateBlendState(&blendDesc, &m_AlphaBlendOn);
        if (FAILED(result))
            return false;
        blendDesc.RenderTarget[0].BlendEnable = FALSE;
        result = m_device->CreateBlendState(&blendDesc, &m_AlphaBlendOff);
        if (FAILED(result))
            return false;

        return true;
    }

    void D3d::ShutDown()
    {
        if (m_swapChain) 
        {
           m_swapChain->SetFullscreenState(false, NULL);
           m_swapChain->Release();
           m_swapChain = 0;
        }
        if (m_depthDisabledStencilState)
        {            
            m_depthDisabledStencilState->Release();
            m_depthDisabledStencilState = 0;
        }
        if (m_RasterizerState)
        {
            m_RasterizerState->Release();
            m_RasterizerState = 0;
        }
        if (m_DepthStencilView)
        {
            m_DepthStencilView->Release();
            m_DepthStencilView = 0;
        }
        if (m_DepthStencilState)
        {
            m_DepthStencilState->Release();
            m_DepthStencilState = 0;
        }
        if (m_depthStencilBuffer)
        {
            m_depthStencilBuffer->Release();
            m_depthStencilBuffer = 0;
        }
        if (m_renderTargetView)
        {
            m_renderTargetView->Release();
            m_renderTargetView = 0;
        }
        if (m_DeviceContext)
        {
            m_DeviceContext->Release();
            m_DeviceContext = 0;
        }
        if (m_device)
        {
            m_device->Release();
            m_device = 0;
        }
        if(m_AlphaBlendOff)
        {
            m_AlphaBlendOff->Release();
            m_AlphaBlendOff = 0;
        }
        if (m_AlphaBlendOn)
        {
            m_AlphaBlendOn->Release();
            m_AlphaBlendOn = 0;
        }
        return;
    }

    void D3d::BeginScene(float r, float g, float b, float a) 
    {
        float color[4];
        color[0] = r;
        color[1] = g;
        color[2] = b;
        color[3] = a;

        m_DeviceContext->ClearRenderTargetView(m_renderTargetView, color);
        m_DeviceContext->ClearDepthStencilView(m_DepthStencilView,D3D11_CLEAR_DEPTH,1.0f,0);
        return;

    }

    void D3d::EndScene()
    {
        if (m_Vsync)
            m_swapChain->Present(1, 0);
        else        
            m_swapChain->Present(0, 0);
        return;
    }
    ID3D11Device* D3d::getDevice()
    {
        return m_device;
    }
    ID3D11DeviceContext* D3d::getDeviceContext()
    {
        return  m_DeviceContext;
    }
    void D3d::GetProjectionMat(XMMATRIX& matrix)
    {
        matrix = m_projectionMat;
        return;
    }
    void D3d::GetWorldMat(XMMATRIX& matrix)
    {
        matrix = m_worldMat;
        return;

    }
    void D3d::GetOrthoMat(XMMATRIX& matrix)
    {
        matrix = m_orthomat;
        return;
    }
    void D3d::ToggleAlphaBlend(bool state)
    {
        float blendFactor[4];
        blendFactor[0] = 0.0f;
        blendFactor[1] = 0.0f;
        blendFactor[2] = 0.0f;
        blendFactor[3] = 0.0f;

        if(state)
        {            
            //turn on
            m_DeviceContext->OMSetBlendState(m_AlphaBlendOn, blendFactor, 0xffffffff);
        }
        else
        {
            //turn off
            m_DeviceContext->OMSetBlendState(m_AlphaBlendOff, blendFactor, 0xffffffff);

        }
        return;
    }
    void D3d::GetVCardInfo(char* cardname, int& memory)
    {
        strcpy_s(cardname, 128, m_VCardDescr);
        memory = m_VcardMem;
        return;
    }

    void D3d::ToggleZBuffer(bool state)
    {
        if(state)
        {
            m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);
            return;
        }
        else
        {
            m_DeviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
            return;
        }
    }
