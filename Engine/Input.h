#pragma once
#pragma region TechStuff
#ifndef _INPUT_H_
#define _INPUT_H_
#pragma endregion

	class Input
	{
#pragma region  public funcs
	public:
		Input();
		Input(Input&);
		~Input();
		void Init();
		void KeyDown(unsigned int);
		void KeyUp(unsigned int);
		bool isKeyDown(unsigned int);
#pragma endregion
#pragma region  private vars
	private:
		bool m_keys[256];
		
#pragma endregion
	};

#endif