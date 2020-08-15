#pragma once
#pragma region TechStuff
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
#pragma endregion
#pragma region includes
#include "D3d.h"
#include "Camera.h"
#include "../Engine/ShaderClasses/ColorShader.h"
#include "Model.h"
#include "../Engine/ShaderClasses/TextureShader.h"
#include "Light.h"
#include "../Engine/ShaderClasses/LightShader.h"
#include "Bitmap.h"
#include "Text.h"
#pragma endregion

#pragma region Globals
	const bool FullScreen = false;
	const bool Vsync = true;
	const float ScreenDepth = 1000.0f;
	const float ScreenNear = .1f;
	
#pragma endregion

	class Graphics
	{
#pragma region Public funcs
	public:
		Graphics();
		Graphics(const Graphics&);
		~Graphics();
		bool Init(int, int, HWND);
		void ShutDown();
		bool Frame(int posX, int posY);

		
#pragma endregion
#pragma region Private funcs
	private:
		bool Render(int posX, int posY);


#pragma endregion
#pragma region Private Vars
	private:
		D3d* m_D3d;
		Camera* m_camera;
		Model* m_model;
		ColorShader* m_colorShader;
		TextureShader* m_texShader;
		LightShader* m_LightShader;
		Light* m_Light;
		Bitmap* m_Bitmap;
		Text* m_Text;
#pragma endregion

	};

#endif