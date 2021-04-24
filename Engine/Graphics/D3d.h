#pragma once
#pragma region TechStuff
#ifndef _D3D_H_
#define _D3D_H_
#pragma endregion
#pragma region Linking
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma endregion
#pragma region Includes
#include <d3d11.h>
#include "../System/Math/EngineMath.h"
using namespace EngineMath;
#pragma endregion

	class D3d
	{
#pragma region Public funcs
	public:
		D3d();
		D3d(D3d&);
		~D3d();
		bool Init(int screenWidth, int screenHeight, bool Vsync, HWND hwnd, bool fullScreen);//, float screenDepth, float screenNear);
		void ShutDown();
		void BeginScene(float r, float g, float b, float a);
		void BeginScene(int r, int g, int b, float a);
		void EndScene();
		ID3D11Device* getDevice();
		ID3D11DeviceContext* getDeviceContext();
		Matrix4x4 GetProjectionMat();
		Matrix4x4 GetWorldMat();
		Matrix4x4 GetOrthoMat();
		void ToggleAlphaBlend(bool);
		void GetVCardInfo(char*, int&);
		void ToggleZBuffer(bool state);
#pragma endregion
#pragma region private Vars
	private:
		bool m_Vsync;
		int m_VcardMem;
		char m_VCardDescr[128];
		IDXGISwapChain* m_swapChain;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_DeviceContext;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11Texture2D* m_depthStencilBuffer;
		ID3D11DepthStencilState* m_DepthStencilState;
		ID3D11DepthStencilView* m_DepthStencilView;
		ID3D11RasterizerState* m_RasterizerState;
		Matrix4x4 m_projectionMat;
		Matrix4x4 m_orthomat;
		Matrix4x4 m_worldMat;
		ID3D11DepthStencilState* m_depthDisabledStencilState;
		ID3D11BlendState* m_AlphaBlendOn;
		ID3D11BlendState* m_AlphaBlendOff;
#pragma endregion
	};

#endif