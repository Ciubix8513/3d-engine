#include "Input.h"


	Input::Input()
	{
	}

	Input::Input(Input&)
	{
	}

	Input::~Input()
	{
	}

	void Input::Init()
	{
		for (int i = 0; i < 256; i++)
			m_keys[i] = false;
	}

	void Input::KeyDown(unsigned int input)
	{
		m_keys[input] = true;
		return;
	}

	void Input::KeyUp(unsigned int input)
	{
		m_keys[input] = false;
		return;
	}

	bool Input::isKeyDown(unsigned int key)
	{
		return  m_keys[key];
	}