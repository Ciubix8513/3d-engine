#pragma once
#ifndef _SOUND_H_
#define _SOUND_H_

#pragma region includes

#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

#pragma endregion


class Sound
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};
#pragma region Public Funcs
public:
	Sound();
	Sound(const Sound&);
	~Sound();
	bool Init(HWND);
	void Shutdown();
#pragma endregion
#pragma region  Private Funcs
private:
	bool InitDirectSound(HWND);
	void ShutDownDirectSound();
	bool LoadWaveFile(char*, IDirectSoundBuffer8**);
	void ShutDownWaveFile(IDirectSoundBuffer8**);
	bool PlayWaveFile();
#pragma endregion
#pragma region Private Vars
private:
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;
	IDirectSoundBuffer8* m_secondaryBuffer;


#pragma endregion







};
#endif

